/**
 * @file NvmStorage.h
 * @brief Acceso persistente a EEPROM del ATmega2560.
 *
 * EEPROM del Mega: 4 KB. Se usa para usuarios, perfiles, contadores
 * de uso de claves y configuracion [RF-13]. Las escrituras EEPROM
 * tienen desgaste: se espacia su frecuencia y se usa estructura
 * con versionado/CRC.
 *
 * TODO [PENDIENTE D-07]: confirmar alcance exacto de lo persistido
 * (solo claves? usuarios completos? historial de eventos?).
 */

#ifndef NVM_STORAGE_H
#define NVM_STORAGE_H

#include <Arduino.h>

/**
 * @class NvmStorage
 * @brief Wrapper sobre EEPROM con interfaz por bloques.
 */
class NvmStorage {
public:
    /**
     * @brief Constructor.
     */
    NvmStorage();

    /**
     * @brief Verifica si la NVM contiene datos validos (magic+CRC).
     * @return true si hay datos persistidos validos.
     */
    bool isValid() const;

    /**
     * @brief Escribe un bloque de bytes en una direccion EEPROM.
     * @param addr Direccion base en EEPROM.
     * @param data Puntero a datos.
     * @param len Numero de bytes.
     * @return true si la escritura se completo.
     */
    bool writeBlock(uint16_t addr, const void* data, uint16_t len);

    /**
     * @brief Lee un bloque de bytes desde EEPROM.
     * @param addr Direccion base.
     * @param data Puntero destino.
     * @param len Numero de bytes.
     * @return true si la lectura fue valida (CRC).
     */
    bool readBlock(uint16_t addr, void* data, uint16_t len) const;

    /**
     * @brief Inicializa NVM con valores por defecto si esta vacia.
     */
    void begin();

private:
    // TODO [PENDIENTE D-07]: mapa de direcciones EEPROM y calculo CRC.
};

#endif /* NVM_STORAGE_H */
