#ifndef SHT31_H
#define SHT31_H

#include "process.h"
#include <stdbool.h>
#include <stdint.h>

extern I2C_HandleTypeDef hi2c2;

#define SHT31_ADDRESS 0x44 << 1  

#define SHT31_MEAS_HIGHREP 0x2400     /**< Địa chỉ mặc định của SHT31 */
#define SHT31_MEAS_HIGHREP 0x2400     /**< Lệnh đo độ chính xác cao (không clock stretching) */
#define SHT31_READSTATUS 0xF32D       /**< Lệnh đọc trạng thái */
#define SHT31_SOFTRESET 0x30A2        /**< Lệnh reset mềm */
#define SHT31_HEATEREN 0x306D         /**< Lệnh bật heater */
#define SHT31_HEATERDIS 0x3066        /**< Lệnh tắt heater */
#define SHT31_REG_HEATER_BIT 0x0d     /**< Bit heater trong thanh ghi trạng thái */

void SHT31_SendCommand1(uint16_t command);
uint8_t SHT31_ReadData(uint8_t *data, uint8_t length);
void SHT31_GetTempAndHumidity(float *temperature, float *humidity);


#endif // SHT31_H
