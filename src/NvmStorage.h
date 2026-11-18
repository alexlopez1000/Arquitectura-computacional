/**
 * @file NvmStorage.h
 * @brief Persistencia en EEPROM del ATmega2560 con version y CRC.
 *
 * Imagen almacenada en EEPROM_BASE_ADDR:
 *
 * | Campo   | Tamano | Descripcion                          |
 * |---------|--------|--------------------------------------|
 * | magic   | 2 B    | EEPROM_MAGIC (firma de imagen valida) |
 * | version | 1 B    | EEPROM_LAYOUT_VERSION                 |
 * | length  | 2 B    | Bytes de datos que siguen             |
 * | data    | N B    | Carga util (registros de usuario)     |
 * | crc     | 2 B    | CRC-16/CCITT de version+length+data   |
 *
 * La escritura usa EEPROM.update() (solo escribe bytes que cambian)
 * para reducir el desgaste de la memoria.
 */

#ifndef NVM_STORAGE_H
#define NVM_STORAGE_H

#include <Arduino.h>

/**
 * @class NvmStorage
 * @brief Lectura/escritura de una imagen versionada y protegida con CRC.
 *
 * Responsabilidad unica: integridad y acceso a EEPROM. No conoce el
 * significado de los datos (lo define UserManager).
 */
class NvmStorage {
public:
    /**
     * @brief Constructor.
     * @param baseAddr Direccion EEPROM donde inicia la imagen.
     */
    explicit NvmStorage(uint16_t baseAddr);

    /**
     * @brief Carga la imagen si magic, version, longitud y CRC son validos.
     * @param data Buffer destino.
     * @param len Tamano esperado de la carga util en bytes.
     * @return true si la imagen es valida y se copio en data.
     */
    bool load(void* data, uint16_t len) const;

    /**
     * @brief Escribe la imagen completa (cabecera + datos + CRC).
     * @param data Carga util a persistir.
     * @param len Tamano de la carga util en bytes.
     * @return true si la verificacion posterior a la escritura es correcta.
     */
    bool save(const void* data, uint16_t len);

    /**
     * @brief CRC-16/CCITT (poly 0x1021) incremental.
     * @param crc Valor previo (0xFFFF para iniciar).
     * @param data Bytes a procesar.
     * @param len Numero de bytes.
     * @return CRC actualizado.
     */
    static uint16_t crc16(uint16_t crc, const uint8_t* data, uint16_t len);

private:
    uint16_t baseAddr_; ///< Direccion base de la imagen.
};

#endif /* NVM_STORAGE_H */
