/**
 * @file rtc.h
 * @brief Interface for DS3231 Real-Time Clock (RTC) module using I2C.
 */

#ifndef __RTC_H__
#define __RTC_H__

#include "stm32f4xx_hal.h"  // Sửa lại theo dòng MCU của bạn (ví dụ: stm32f4xx_hal.h)

// I2C address of DS3231 (7-bit address shifted for HAL)
#define DS3231_ADDRESS     (0x68 << 1)

// Register addresses
#define REG_SECONDS        0x00
#define REG_MINUTES        0x01
#define REG_HOURS          0x02
#define REG_DAY            0x03
#define REG_DATE           0x04
#define REG_MONTH          0x05
#define REG_YEAR           0x06

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize RTC module with the I2C handle.
 * @param hi2c Pointer to HAL I2C handle
 */
static uint8_t Dec_to_BCD(uint8_t val);
static uint8_t BCD_to_Dec(uint8_t val);
void RTC_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief Set the current time and date to DS3231.
 * @param hours   Hours (0-23)
 * @param minutes Minutes (0-59)
 * @param seconds Seconds (0-59)
 * @param day     Day of week (1=Mon ... 7=Sun)
 * @param date    Date (1-31)
 * @param month   Month (1-12)
 * @param year    Year (0-99), offset from 2000
 */
void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds,
                 uint8_t day, uint8_t date, uint8_t month, uint8_t year);

/**
 * @brief Read the current time and date from DS3231.
 * @param hours   Pointer to store hours
 * @param minutes Pointer to store minutes
 * @param seconds Pointer to store seconds
 * @param day     Pointer to store day of week
 * @param date    Pointer to store date
 * @param month   Pointer to store month
 * @param year    Pointer to store year
 */
void RTC_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds,
                 uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year);

#ifdef __cplusplus
}
#endif

#endif // __RTC_H__
