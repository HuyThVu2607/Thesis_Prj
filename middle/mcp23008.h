#ifndef MCP23008_H
#define MCP23008_H

#include "main.h"  

// Adress 
#define MCP23008_ADDRESS     (0x20 << 1) 


#define MCP23008_IODIR       0x00
#define MCP23008_GPIO        0x09
#define MCP23008_GPPU        0x06
#define MCP23008_OLAT        0x0A

typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t address;
} MCP23008_t;

void MCP23008_Init(MCP23008_t *dev, I2C_HandleTypeDef *hi2c, uint8_t address);
uint8_t MCP23008_ReadGPIO(MCP23008_t *dev);
void MCP23008_WriteGPIO(MCP23008_t *dev, uint8_t value);
void MCP23008_PinMode(MCP23008_t *dev, uint8_t pin, uint8_t mode);
uint8_t MCP23008_DigitalRead(MCP23008_t *dev, uint8_t pin);
void MCP23008_DigitalWrite(MCP23008_t *dev, uint8_t pin, uint8_t value);
void MCP23008_PullUp(MCP23008_t *dev, uint8_t pin, uint8_t enable);

#endif
