#ifndef SHT31_H
#define SHT31_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#define SHT31_DEFAULT_ADDR 0x44       /**< Địa chỉ mặc định của SHT31 */
#define SHT31_MEAS_HIGHREP 0x2400     /**< Lệnh đo độ chính xác cao (không clock stretching) */
#define SHT31_READSTATUS 0xF32D       /**< Lệnh đọc trạng thái */
#define SHT31_SOFTRESET 0x30A2        /**< Lệnh reset mềm */
#define SHT31_HEATEREN 0x306D         /**< Lệnh bật heater */
#define SHT31_HEATERDIS 0x3066        /**< Lệnh tắt heater */
#define SHT31_REG_HEATER_BIT 0x0d     /**< Bit heater trong thanh ghi trạng thái */

// Cấu trúc dữ liệu để lưu trạng thái cảm biến
typedef struct {
    float temperature;
    float humidity;
    uint8_t i2c_addr;
} SHT31;

// Khởi tạo cảm biến với địa chỉ I2C
bool SHT31_begin(SHT31 *sht, uint8_t i2c_addr);

// Đọc nhiệt độ
float SHT31_readTemperature(SHT31 *sht, I2C_HandleTypeDef *hi2c);

// Đọc độ ẩm
float SHT31_readHumidity(SHT31 *sht, I2C_HandleTypeDef *hi2c);

// Đọc cả nhiệt độ và độ ẩm
bool SHT31_readBoth(SHT31 *sht, I2C_HandleTypeDef *hi2c, float *temperature, float *humidity);

// Đọc trạng thái cảm biến
uint16_t SHT31_readStatus(SHT31 *sht, I2C_HandleTypeDef *hi2c);

// Reset cảm biến
void SHT31_reset(SHT31 *sht, I2C_HandleTypeDef *hi2c);

// Bật hoặc tắt heater
void SHT31_heater(SHT31 *sht, I2C_HandleTypeDef *hi2c, bool enable);

// Kiểm tra trạng thái của heater
bool SHT31_isHeaterEnabled(SHT31 *sht, I2C_HandleTypeDef *hi2c);

#endif // SHT31_H
