#include "SHT31.h"
#include <math.h>

// Hàm tính CRC8 để kiểm tra dữ liệu
static uint8_t crc8(const uint8_t *data, int len) {
    const uint8_t POLYNOMIAL = 0x31;
    uint8_t crc = 0xFF;

    for (int j = len; j; --j) {
        crc ^= *data++;
        for (int i = 8; i; --i) {
            crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
        }
    }
    return crc;
}

// Khởi tạo cảm biến
bool SHT31_begin(SHT31 *sht, uint8_t i2c_addr) {
    sht->i2c_addr = i2c_addr;
    return true;
}

// Gửi lệnh tới cảm biến
static bool SHT31_writeCommand(I2C_HandleTypeDef *hi2c, SHT31 *sht, uint16_t command) {
    uint8_t cmd[2] = {command >> 8, command & 0xFF};
    return HAL_I2C_Master_Transmit(hi2c, sht->i2c_addr << 1, cmd, 2, HAL_MAX_DELAY) == HAL_OK;
}

// Đọc trạng thái cảm biến
uint16_t SHT31_readStatus(SHT31 *sht, I2C_HandleTypeDef *hi2c) {
    if (!SHT31_writeCommand(hi2c, sht, SHT31_READSTATUS)) {
        return 0xFFFF;
    }

    uint8_t data[3];
    if (HAL_I2C_Master_Receive(hi2c, sht->i2c_addr << 1, data, 3, HAL_MAX_DELAY) != HAL_OK) {
        return 0xFFFF;
    }

    uint16_t status = (data[0] << 8) | data[1];
    return status;
}

// Reset cảm biến
void SHT31_reset(SHT31 *sht, I2C_HandleTypeDef *hi2c) {
    SHT31_writeCommand(hi2c, sht, SHT31_SOFTRESET);
    HAL_Delay(10);
}

// Bật hoặc tắt heater
void SHT31_heater(SHT31 *sht, I2C_HandleTypeDef *hi2c, bool enable) {
    if (enable) {
        SHT31_writeCommand(hi2c, sht, SHT31_HEATEREN);
    } else {
        SHT31_writeCommand(hi2c, sht, SHT31_HEATERDIS);
    }
    HAL_Delay(1);
}

// Kiểm tra trạng thái của heater
bool SHT31_isHeaterEnabled(SHT31 *sht, I2C_HandleTypeDef *hi2c) {
    uint16_t status = SHT31_readStatus(sht, hi2c);
    return (status != 0xFFFF) && ((status & (1 << SHT31_REG_HEATER_BIT)) != 0);
}

// Đọc cả nhiệt độ và độ ẩm
bool SHT31_readBoth(SHT31 *sht, I2C_HandleTypeDef *hi2c, float *temperature, float *humidity) {
    uint8_t data[6];

    if (!SHT31_writeCommand(hi2c, sht, SHT31_MEAS_HIGHREP)) {
        return false;
    }

    HAL_Delay(20);

    if (HAL_I2C_Master_Receive(hi2c, sht->i2c_addr << 1, data, 6, HAL_MAX_DELAY) != HAL_OK) {
        return false;
    }

    if (data[2] != crc8(data, 2) || data[5] != crc8(data + 3, 2)) {
        return false;
    }

    int32_t stemp = (data[0] << 8) | data[1];
    stemp = ((4375 * stemp) >> 14) - 4500;
    *temperature = (float)stemp / 100.0f;

    uint32_t shum = (data[3] << 8) | data[4];
    shum = (625 * shum) >> 12;
    *humidity = (float)shum / 100.0f;

    return true;
}

// Đọc nhiệt độ
float SHT31_readTemperature(SHT31 *sht, I2C_HandleTypeDef *hi2c) {
    float temperature, humidity;
    if (SHT31_readBoth(sht, hi2c, &temperature, &humidity)) {
        return temperature;
    }
    return NAN;
}

// Đọc độ ẩm
float SHT31_readHumidity(SHT31 *sht, I2C_HandleTypeDef *hi2c) {
    float temperature, humidity;
    if (SHT31_readBoth(sht, hi2c, &temperature, &humidity)) {
        return humidity;
    }
    return NAN;
}
