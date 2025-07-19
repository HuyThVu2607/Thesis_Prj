#include "24lc256.h"
#include "stm32f4xx_hal.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

// EEPROM config
#define EEPROM_I2C_ADDR_8BIT        (0x50 << 1)      // 0xA0
#define EEPROM_TOTAL_SIZE           (0x8000)         // 32KB = 0x0000 - 0x7FFF
#define EEPROM_PAGE_SIZE            (64U)
#define EEPROM_TIMEOUT              (1000U)

uint32_t prj_eeprom_24lc256_write_bytes(uint16_t mem_address, uint8_t* data, uint16_t size)
{
    if (data == NULL || (mem_address + size) > EEPROM_TOTAL_SIZE)
        return 1;

    HAL_StatusTypeDef res = HAL_I2C_Mem_Write(
        &hi2c1,
        EEPROM_I2C_ADDR_8BIT,
        mem_address,
        I2C_MEMADD_SIZE_16BIT,
        data,
        size,
        EEPROM_TIMEOUT
    );
    //for (int i = 0; i < 100000; i++){}
    
    HAL_Delay(40);  // Wait for EEPROM internal write cycle
    return (res == HAL_OK) ? 0 : 1;
}

uint32_t prj_eeprom_24lc256_read_bytes(uint16_t mem_address, uint8_t* data, uint16_t size)
{
    if (data == NULL || (mem_address + size) > EEPROM_TOTAL_SIZE)
        return 1;

    HAL_StatusTypeDef res = HAL_I2C_Mem_Read(
        &hi2c1,
        EEPROM_I2C_ADDR_8BIT,
        mem_address,
        I2C_MEMADD_SIZE_16BIT,
        data,
        size,
        EEPROM_TIMEOUT
    );

    for (int i = 0; i < 4000000; i++){}

    return (res == HAL_OK) ? 0 : 1;
}
