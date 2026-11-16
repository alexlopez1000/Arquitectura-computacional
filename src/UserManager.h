/**
 * @file UserManager.h
 * @brief Gestion de usuarios, perfiles, claves y persistencia.
 *
 * Requisitos cubiertos [CONFIRMADO: ARQ_Proyecto.pdf]:
 *  - Perfiles: Seguridad, Operarios, Coordinadores, Gerentes [RF-11]
 *  - Claves de >=4 digitos, cambio tras 4 usos, sin repeticion [RF-12]
 *  - Alta de usuarios y copia de credenciales [RF-08, RF-09]
 *  - Persistencia en EEPROM via NvmStorage [RF-13]
 */

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <Arduino.h>
#include "config.h"

/** Roles de la empresa [CONFIRMADO: enunciado]. */
enum class Profile : uint8_t {
    SECURITY    = 0, ///< Seguridad.
    OPERATORS   = 1, ///< Operarios.
    COORDINATORS= 2, ///< Coordinadores.
    MANAGERS    = 3  ///< Gerentes.
};

/** Registro de un usuario del sistema. */
struct UserRecord {
    bool     active;                 ///< Usuario habilitado.
    Profile  profile;                ///< Rol asignado.
    uint8_t  rfidUid[4];             ///< Credencial RFID (si aplica).
    char     key[MAX_KEY_LENGTH+1];  ///< Clave numerica vigente.
    uint8_t  keyUses;                ///< Usos acumulados de la clave.
    char     prevKey[MAX_KEY_LENGTH+1]; ///< Ultima clave (anti-repeticion).
};

/**
 * @class UserManager
 * @brief Repositorio de usuarios con reglas de claves.
 *
 * Responsabilidad unica: alta/baja/consulta de usuarios y validacion
 * de reglas de claves (4 usos, no repeticion). La lectura/escritura
 * EEPROM se delega a NvmStorage.
 */
class UserManager {
public:
    /**
     * @brief Constructor.
     */
    UserManager();

    /**
     * @brief Carga usuarios persistidos desde NVM.
     */
    void begin();

    /**
     * @brief Registra un nuevo usuario [RF-08].
     * @param user Datos del usuario.
     * @return true si se creo correctamente.
     */
    bool addUser(const UserRecord& user);

    /**
     * @brief Busca usuario por credencial RFID.
     * @param uid NUID de 4 bytes.
     * @return Indice del usuario o -1 si no existe.
     */
    int findByRfid(const uint8_t uid[4]) const;

    /**
     * @brief Busca usuario por clave numerica.
     * @param key Cadena de digitos.
     * @return Indice del usuario o -1 si no existe.
     */
    int findByKey(const char* key) const;

    /**
     * @brief Copia credencial de un usuario a otro [RF-09].
     * @param src Indice origen.
     * @param dst Indice destino.
     * @return true si la copia fue valida.
     */
    bool cloneCredential(int src, int dst);

    /**
     * @brief Registra uso exitoso de la clave de un usuario.
     *        Si alcanza KEY_MAX_USES marca rotacion pendiente [RF-12].
     * @param index Indice del usuario.
     * @return true si requiere cambio de clave.
     */
    bool registerKeyUse(int index);

    /**
     * @brief Cambia la clave de un usuario validando no-repeticion.
     * @param index Indice del usuario.
     * @param newKey Nueva clave (>= MIN_KEY_LENGTH digitos).
     * @return true si el cambio fue aceptado.
     */
    bool changeKey(int index, const char* newKey);

    /**
     * @brief Persiste el estado actual en NVM.
     */
    void save();

    /**
     * @brief Acceso de solo lectura al arreglo de usuarios.
     * @return Puntero al arreglo y cantidad via count().
     */
    const UserRecord* records() const;

    /**
     * @brief Numero de usuarios registrados.
     * @return Cantidad de registros.
     */
    uint8_t count() const;

private:
    UserRecord users_[MAX_USERS]; ///< Almacen de usuarios en RAM.
    uint8_t    userCount_;        ///< Registros ocupados.
    // TODO [PENDIENTE D-07]: esquema EEPROM exacto y CRC.
};

#endif /* USER_MANAGER_H */
