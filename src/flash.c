#include "flash.h"

int8_t flash_unlock_eeprom()
{
    if ((FLASH_IAPSR & FLASH_IAPSR_DUL) == 0)
    {
        FLASH_DUKR = FLASH_EEPROM_UNLOCK_KEY_1;
        FLASH_DUKR = FLASH_EEPROM_UNLOCK_KEY_2;
    }

    // Check DUL again. if DUL = 1, unlock successful. Else failed.
    if (FLASH_IAPSR & FLASH_IAPSR_DUL)
        return 0;
    else
        return -1;
}

int8_t flash_lock_eeprom()
{
    if (FLASH_IAPSR & FLASH_IAPSR_DUL)
    {
        // set DUL bit to 0
        FLASH_IAPSR &= ~FLASH_IAPSR_DUL;
    }

    // Check DUL again. if DUL = 0, lock successful. Else failed.
    if ((FLASH_IAPSR & FLASH_IAPSR_DUL) == 0)
        return 0;
    else
        return -1;
}

uint16_t flash_write_eeprom(uint16_t address, void *data, uint8_t data_size)
{
    uint8_t *addr = (uint8_t *)(EEPROM_BASE_ADDRESS + address);
    uint8_t *d = data;

    while (data_size--)
    {
        *addr++ = *d++;
    }

    return ((uint16_t)addr - EEPROM_BASE_ADDRESS);
}

uint16_t flash_read_eeprom(uint16_t address, void *data, uint8_t data_size)
{
    uint8_t *addr = (uint8_t *)(EEPROM_BASE_ADDRESS + address);
    uint8_t *d = data;

    while (data_size--)
    {
        *d++ = *addr++;
    }

    return ((uint16_t)addr - EEPROM_BASE_ADDRESS);
}