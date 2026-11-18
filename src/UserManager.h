/**
 * @file UserManager.h
 * @brief Repositorio de los 3 usuarios del sistema (UID RFID,
 *        perfil y contrasena individual) persistidos en EEPROM.
 */

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <Arduino.h>
#include "config.h"
#include "NvmStorage.h"

/** Perfiles de la empresa; cada uno de los 3 usuarios tiene uno distinto. */
enum class Profile : uint8_t {
    SECURITY    = 0, ///< Seguridad.
    OPERATOR    = 1, ///< Operario.
    COORDINATOR = 2, ///< Coordinador.
    MANAGER     = 3  ///< Gerente.
};

/** Registro persistido de un usuario (tamano fijo, sin memoria dinamica). */
struct UserRecord {
    uint8_t rfidUid[4];                      ///< NUID de la tarjeta RFID.
    Profile profile;                         ///< Perfil asignado.
    char    password[MAX_PASSWORD_LENGTH + 1]; ///< Contrasena numerica (terminada en '\\0').
};

/**
 * @class UserManager
 * @brief Busqueda de usuarios por UID y verificacion de contrasena.
 *
 * Al arrancar carga la imagen de EEPROM; si no es valida (primer
 * arranque, cambio de version o CRC incorrecto) escribe los usuarios
 * por defecto. La lectura/escritura fisica se delega a NvmStorage.
 */
class UserManager {
public:
    /**
     * @brief Constructor.
     * @param storage Almacenamiento EEPROM inyectado.
     */
    explicit UserManager(NvmStorage& storage);

    /**
     * @brief Carga los usuarios desde EEPROM o restaura los de fabrica.
     * @return true si se cargo una imagen valida; false si se restauraron valores por defecto.
     */
    bool begin();

    /**
     * @brief Busca un usuario por UID RFID.
     * @param uid NUID de 4 bytes.
     * @return Indice del usuario (0..NUM_USERS-1) o -1 si no existe.
     */
    int8_t findByUid(const uint8_t uid[4]) const;

    /**
     * @brief Compara la contrasena del usuario indicado.
     * @param index Indice del usuario.
     * @param password Cadena digitada.
     * @return true si coincide.
     */
    bool checkPassword(int8_t index, const char* password) const;

    /**
     * @brief Cambia la contrasena de un usuario y persiste la imagen.
     * @param index Indice del usuario.
     * @param newPassword Nueva contrasena (MIN..MAX_PASSWORD_LENGTH digitos).
     * @return true si la contrasena es valida y se guardo.
     */
    bool setPassword(int8_t index, const char* newPassword);

    /**
     * @brief Perfil de un usuario.
     * @param index Indice del usuario.
     * @return Perfil asignado.
     */
    Profile profileOf(int8_t index) const;

    /**
     * @brief Nombre corto del perfil para el LCD.
     * @param profile Perfil.
     * @return Cadena constante (max. 11 caracteres).
     */
    static const char* profileName(Profile profile);

    /**
     * @brief Restaura y persiste los usuarios por defecto.
     */
    void restoreDefaults();

private:
    NvmStorage& storage_;            ///< Acceso a EEPROM.
    UserRecord  users_[NUM_USERS];   ///< Copia en RAM de los usuarios.
};

#endif /* USER_MANAGER_H */
