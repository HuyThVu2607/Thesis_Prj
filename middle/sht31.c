#include "sht31.h"
#include <math.h>

void SHT31_SendCommand1(uint16_t command) {
    uint8_t cmd[2];
    cmd[0] = (command >> 8) & 0xFF;
    cmd[1] = command & 0xFF;
    HAL_I2C_Master_Transmit(&hi2c2, SHT31_ADDRESS, cmd, 2, 100);
}


uint8_t SHT31_ReadData(uint8_t *data, uint8_t length) {
    return HAL_I2C_Master_Receive(&hi2c2, SHT31_ADDRESS, data, length, 100);
}


void SHT31_GetTempAndHumidity(float *temperature, float *humidity) {
    uint8_t data[6];
    SHT31_SendCommand1(SHT31_MEAS_HIGHREP);
    HAL_Delay(15);  // 

    if (SHT31_ReadData(data, 6) == HAL_OK) {
        uint16_t rawTemp = (data[0] << 8) | data[1];
        uint16_t rawHumidity = (data[3] << 8) | data[4];

        *temperature = -45 + (175 * (float)rawTemp / 65535.0);
        *humidity = 100 * (float)rawHumidity / 65535.0;
    } else {
        *temperature = *humidity = 0;  
    }
}
