/**
 * @file flash.h
 * @author Avra Mitra
 * @brief stm8s flash operation. 
 * So far, only eeprom operations are implemented. That too partially.
 * @date 2021-12-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _FLASH_H
#define _FLASH_H

#include <stdint.h>
#include "stm8s.h"

#define EEPROM_BASE_ADDRESS 0x4000

/**
 * @brief Write data into eeprom. Supports any datatype.
 * @param address Address offset off EEPROm, starts from 0.
 * @param data pointer to the data for writing.
 * @param data_size Size of the data (get using sizeof()).
 * @return uint16_t returns next address offset for writing.
 */
uint16_t flash_write_eeprom(uint16_t address, void *data, uint8_t data_size);
/**
 * @brief Read any data from eeprom. Supports any datatype.
 * @param address Address offset off EEPROm, starts from 0.
 * @param data pointer to the data for reading.
 * @param data_size Size of the data (get using sizeof()).
 * @return uint16_t returns next address offset for reading.
 */
uint16_t flash_read_eeprom(uint16_t address, void *data, uint8_t data_size);
/**
 * @brief Unlock eeprom so we can write.
 * @return int8_t returns 0 if successful, -1 if failed.
 */
int8_t flash_unlock_eeprom();
/**
 * @brief Lock eeprom so we don't write accidentally.
 * @return int8_t returns 0 if successful, -1 if failed.
 */
int8_t flash_lock_eeprom();

#endif // _FLASH_H