/**
 * @file ScheduleValidator.h
 * @brief Validacion de franjas horarias de acceso via RTC.
 *
 * Requisito [CONFIRMADO: RF-05] "Establecer franjas horarias de
 * acceso". El material del curso no incluye RTC; se propone DS3231
 * por bus I2C (Wire).
 *
 * TODO [PENDIENTE D-05]: confirmar modulo RTC y si las franjas se
 * definen por perfil o globalmente.
 */

#ifndef SCHEDULE_VALIDATOR_H
#define SCHEDULE_VALIDATOR_H

#include <Arduino.h>

/** Franja horaria permitida (hora local). */
struct TimeWindow {
    uint8_t startHour;   ///< Hora inicio (0-23).
    uint8_t startMinute; ///< Minuto inicio.
    uint8_t endHour;     ///< Hora fin (0-23).
    uint8_t endMinute;   ///< Minuto fin.
    uint8_t daysMask;    ///< Bitmask de dias permitidos (bit0=lun..bit6=dom).
};

/**
 * @class ScheduleValidator
 * @brief Decide si el momento actual esta dentro de una franja
 *        horaria permitida.
 *
 * Responsabilidad unica: consulta al RTC y comparacion con la tabla
 * de franjas. No conoce perfiles ni credenciales.
 */
class ScheduleValidator {
public:
    /**
     * @brief Constructor.
     */
    ScheduleValidator();

    /**
     * @brief Inicializa el bus I2C y el modulo RTC.
     *        TODO [PENDIENTE D-05]: driver DS3231.
     */
    void begin();

    /**
     * @brief Actualiza la hora leida del RTC.
     *        Llamar periodicamente desde task_schedule.
     */
    void update();

    /**
     * @brief Indica si la hora actual esta dentro de alguna franja.
     * @return true si el acceso esta permitido por horario.
     */
    bool isAccessAllowed() const;

    /**
     * @brief Registra una franja horaria permitida.
     * @param window Franja a agregar.
     */
    void addWindow(const TimeWindow& window);

private:
    // TODO [PENDIENTE D-05]: objeto RTC, hora actual cacheada y
    // arreglo de franjas configurables.
};

#endif /* SCHEDULE_VALIDATOR_H */
