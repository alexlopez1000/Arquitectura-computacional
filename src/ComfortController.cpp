/**
 * @file ComfortController.cpp
 * @brief Implementacion del calculo PMV (Fanger) y control de confort.
 */

#include "ComfortController.h"
#include <math.h>
#include "config.h"

ComfortController::ComfortController()
    : action_{false, 0.0f, 5.0f, false, false} {}

float ComfortController::computePmv(float ta, float tr, float vel, float rh,
                                    float met, float clo, float wme) {
    // Presion parcial de vapor de agua (Pa).
    const float pa = rh * 10.0f * expf(16.6536f - 4030.183f / (ta + 235.0f));

    const float icl = 0.155f * clo;     // aislamiento de la ropa (m2K/W)
    const float m   = met * 58.15f;     // tasa metabolica (W/m2)
    const float w   = wme * 58.15f;     // trabajo externo (W/m2)
    const float mw  = m - w;            // produccion interna de calor

    // Factor de area de la ropa.
    const float fcl = (icl <= 0.078f) ? 1.0f + 1.29f * icl : 1.05f + 0.645f * icl;

    const float hcf  = 12.1f * sqrtf(vel);  // conveccion forzada
    const float taa  = ta + 273.0f;
    const float tra  = tr + 273.0f;
    const float tcla = taa + (35.5f - ta) / (3.5f * icl + 0.1f);

    const float p1 = icl * fcl;
    const float p2 = p1 * 3.96f;
    const float p3 = p1 * 100.0f;
    const float p4 = p1 * taa;
    const float tra4 = powf(tra / 100.0f, 4.0f);
    const float p5 = 308.7f - 0.028f * mw + p2 * tra4;

    // Iteracion para la temperatura superficial de la ropa (tcl).
    float xn = tcla / 100.0f;
    float xf = tcla / 50.0f;
    float hc = hcf;
    const float eps = 0.00015f;
    for (uint8_t n = 0; n < 150 && fabsf(xn - xf) > eps; n++) {
        xf = (xf + xn) / 2.0f;
        const float hcn = 2.38f * powf(fabsf(100.0f * xf - taa), 0.25f);
        hc = (hcf > hcn) ? hcf : hcn;
        xn = (p5 + p4 * hc - p2 * powf(xf, 4.0f)) / (100.0f + p3 * hc);
    }
    const float tcl = 100.0f * xn - 273.0f;

    // Componentes de perdida de calor (W/m2).
    const float hl1 = 3.05e-3f * (5733.0f - 6.99f * mw - pa);          // difusion piel
    const float hl2 = (mw > 58.15f) ? 0.42f * (mw - 58.15f) : 0.0f;     // sudoracion
    const float hl3 = 1.7e-5f * m * (5867.0f - pa);                    // resp. latente
    const float hl4 = 0.0014f * m * (34.0f - ta);                      // resp. sensible
    const float hl5 = 3.96f * fcl * (powf(xn, 4.0f) - tra4);           // radiacion
    const float hl6 = fcl * hc * (tcl - ta);                           // conveccion

    const float ts = 0.303f * expf(-0.036f * m) + 0.028f;
    return ts * (mw - hl1 - hl2 - hl3 - hl4 - hl5 - hl6);
}

float ComfortController::computePpd(float pmv) {
    const float p2 = pmv * pmv;
    return 100.0f - 95.0f * expf(-0.03353f * p2 * p2 - 0.2179f * p2);
}

ComfortAction ComfortController::evaluate(const EnvSnapshot& snap) {
    action_.valid = snap.dhtValid;
    if (snap.dhtValid) {
        action_.pmv = computePmv(snap.temperatureC, snap.temperatureC,
                                 PMV_AIR_SPEED, snap.humidityPct,
                                 PMV_MET, PMV_CLO, PMV_EXTERNAL_WORK);
        action_.ppd = computePpd(action_.pmv);
        if (action_.pmv > PMV_FAN_ON) {
            action_.fanOn = true;
        } else if (action_.pmv <= PMV_FAN_OFF) {
            action_.fanOn = false;
        }
    }

    if (snap.lightLevel < LIGHT_ON_BELOW_ADC) {
        action_.lightOn = true;
    } else if (snap.lightLevel > LIGHT_OFF_ABOVE_ADC) {
        action_.lightOn = false;
    }
    return action_;
}

ComfortAction ComfortController::lastAction() const { return action_; }
