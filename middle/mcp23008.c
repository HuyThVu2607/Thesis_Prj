#include "mcp23008.h"

static void MCP23008_WriteRegister(MCP23008_t *dev, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    HAL_I2C_Master_Transmit(dev->hi2c, dev->address, data, 2, 100);
}

static uint8_t MCP23008_ReadRegister(MCP23008_t *dev, uint8_t reg) {
    uint8_t value;
    HAL_I2C_Master_Transmit(dev->hi2c, dev->address, &reg, 1, 100);
    HAL_I2C_Master_Receive(dev->hi2c, dev->address, &value, 1, 100);
    return value;
}

void MCP23008_Init(MCP23008_t *dev, I2C_HandleTypeDef *hi2c, uint8_t address) {
    dev->hi2c = hi2c;
    dev->address = address;

    // Setup input standard
    MCP23008_WriteRegister(dev, MCP23008_IODIR, 0xFF);
}

uint8_t MCP23008_ReadGPIO(MCP23008_t *dev) {
    return MCP23008_ReadRegister(dev, MCP23008_GPIO);
}

void MCP23008_WriteGPIO(MCP23008_t *dev, uint8_t value) {
    MCP23008_WriteRegister(dev, MCP23008_GPIO, value);
}

void MCP23008_PinMode(MCP23008_t *dev, uint8_t pin, uint8_t mode) {
    uint8_t iodir = MCP23008_ReadRegister(dev, MCP23008_IODIR);
    if (mode) {
        iodir |= (1 << pin);  // INPUT
    } else {
        iodir &= ~(1 << pin); // OUTPUT
    }
    MCP23008_WriteRegister(dev, MCP23008_IODIR, iodir);
}

uint8_t MCP23008_DigitalRead(MCP23008_t *dev, uint8_t pin) {
    return (MCP23008_ReadGPIO(dev) >> pin) & 0x01;
}

void MCP23008_DigitalWrite(MCP23008_t *dev, uint8_t pin, uint8_t value) {
    uint8_t gpio = MCP23008_ReadGPIO(dev);
    if (value)
        gpio |= (1 << pin);
    else
        gpio &= ~(1 << pin);
    MCP23008_WriteGPIO(dev, gpio);
}

void MCP23008_PullUp(MCP23008_t *dev, uint8_t pin, uint8_t enable) {
    uint8_t gppu = MCP23008_ReadRegister(dev, MCP23008_GPPU);
    if (enable)
        gppu |= (1 << pin);
    else
        gppu &= ~(1 << pin);
    MCP23008_WriteRegister(dev, MCP23008_GPPU, gppu);
}
