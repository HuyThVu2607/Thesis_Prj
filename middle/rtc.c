/**
 * @file rtc.c
 * @brief Implementation of DS3231 RTC functions.
 */

#include "rtc.h"
#include "process.h"

//static I2C_HandleTypeDef *&hi2c1 = NULL;  // Local handle to use I2C
extern I2C_HandleTypeDef hi2c1;
// Convert from BCD to Decimal
static uint8_t BCD_to_Dec(uint8_t val) {
    return (val >> 4) * 10 + (val & 0x0F);
}

// Convert from Decimal to BCD
static uint8_t Dec_to_BCD(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

// Write a value to a DS3231 register
static void RTC_Write(uint8_t reg, uint8_t value) {
    HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, reg, 1, &value, 1, 100);
}

// Read a value from a DS3231 register
static uint8_t RTC_Read(uint8_t reg) {
    uint8_t value;
    HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, reg, 1, &value, 1, 100);
    return value;
}

// Public functions

void RTC_Init(I2C_HandleTypeDef *hi2c) {
    //&hi2c1 = hi2c;
}

void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds,
                 uint8_t day, uint8_t date, uint8_t month, uint8_t year) {
    RTC_Write(REG_SECONDS, Dec_to_BCD(seconds));
    RTC_Write(REG_MINUTES, Dec_to_BCD(minutes));
    RTC_Write(REG_HOURS, Dec_to_BCD(hours));
    RTC_Write(REG_DAY, Dec_to_BCD(day));
    RTC_Write(REG_DATE, Dec_to_BCD(date));
    RTC_Write(REG_MONTH, Dec_to_BCD(month));
    RTC_Write(REG_YEAR, Dec_to_BCD(year));
}

void RTC_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds,
                 uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year) {
    *seconds = BCD_to_Dec(RTC_Read(REG_SECONDS));
    *minutes = BCD_to_Dec(RTC_Read(REG_MINUTES));
    *hours   = BCD_to_Dec(RTC_Read(REG_HOURS));
    *day     = BCD_to_Dec(RTC_Read(REG_DAY));
    *date    = BCD_to_Dec(RTC_Read(REG_DATE));
    *month   = BCD_to_Dec(RTC_Read(REG_MONTH));
    *year    = BCD_to_Dec(RTC_Read(REG_YEAR));
}
