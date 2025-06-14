#ifndef __ADS1115__ 
#define __ADS1115__

/*
 * Library Name: 	ADS1115 STM32 Single-Ended, Single-Shot, PGA & Data Rate Enabled HAL Library
 * Written By:		Ahmet Batuhan Günaltay
 * Date Written:	02/04/2021 (DD/MM/YYYY)
 * Last Modified:	03/04/2021 (DD/MM/YYYY)
 * Description:		STM32F4 HAL-Based ADS1115 Library
 * References:
 * 	- https://www.ti.com/lit/gpn/ADS1113 [Datasheet]
 *
 * Copyright (C) 2021 - Ahmet Batuhan Günaltay
 *
	This software library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  */

/* Definitions */
#define ADS1115_OS        (1 << 7)     // 0x80

#define ADS1115_MUX_AIN0  (0x04 << 4)  // 0x40: AIN0 to GND
#define ADS1115_MUX_AIN1  (0x05 << 4)  // 0x50: AIN1 to GND
#define ADS1115_MUX_AIN2  (0x06 << 4)  // 0x60: AIN2 to GND
#define ADS1115_MUX_AIN3  (0x07 << 4)  // 0x70: AIN3 to GND

#define ADS1115_PGA_TWOTHIRDS           0x0000  // 2/3x ±6.144V
#define ADS1115_PGA_ONE                 0x0200  // 1x   ±4.096V
#define ADS1115_PGA_TWO                 0x0400  // 2x   ±2.048V
#define ADS1115_PGA_FOUR                0x0600  // 4x   ±1.024V
#define ADS1115_PGA_EIGHT               0x0800  // 8x   ±0.512V
#define ADS1115_PGA_SIXTEEN             0x0A00  // 16x  ±0.256V

#define ADS1115_MODE (0x1) // Default

#define ADS1115_DATA_RATE_8     (0x00 << 5)   // 8 SPS
#define ADS1115_DATA_RATE_16    (0x01 << 5)   // 16 SPS
#define ADS1115_DATA_RATE_32    (0x02 << 5)
#define ADS1115_DATA_RATE_64    (0x03 << 5)
#define ADS1115_DATA_RATE_128   (0x04 << 5)
#define ADS1115_DATA_RATE_250   (0x05 << 5)
#define ADS1115_DATA_RATE_475   (0x06 << 5)
#define ADS1115_DATA_RATE_860   (0x07 << 5)

#define ADS1115_COMP_MODE       (0x00 << 4)
#define ADS1115_COMP_POL        (0x00 << 3)
#define ADS1115_COMP_LAT        (0x00 << 2)
#define ADS1115_COMP_QUE        (0x03)        // 0b11 = 3
/* ADS1115 register configurations */
#define ADS1115_CONVER_REG 0x0
#define ADS1115_CONFIG_REG 0x1

// ========================
// Channel bitmask defines
// ========================

// ADS1115 - IC0 (Device 0)
#define ADS1115_IC0_CH0    (1 << 0)  // Enable reading Channel 0 of ADS1115 IC0 (AIN0)
#define ADS1115_IC0_CH1    (1 << 1)  // Enable reading Channel 1 of ADS1115 IC0 (AIN1)
#define ADS1115_IC0_CH2    (1 << 2)  // Enable reading Channel 2 of ADS1115 IC0 (AIN2)
#define ADS1115_IC0_CH3    (1 << 3)  // Enable reading Channel 3 of ADS1115 IC0 (AIN3)

// ADS1115 - IC1 (Device 1)
#define ADS1115_IC1_CH0    (1 << 4)  // Enable reading Channel 0 of ADS1115 IC1 (AIN0)
#define ADS1115_IC1_CH1    (1 << 5)  // Enable reading Channel 1 of ADS1115 IC1 (AIN1)
#define ADS1115_IC1_CH2    (1 << 6)  // Enable reading Channel 2 of ADS1115 IC1 (AIN2)
#define ADS1115_IC1_CH3    (1 << 7)  // Enable reading Channel 3 of ADS1115 IC1 (AIN3)

// All channels (IC0 + IC1)
#define ADS1115_ALL_CHANNELS  0xFF  // Enable all 8 channels

/* TIMEOUT */
#define ADS1115_TIMEOUT 1 // Timeout for HAL I2C functions.

/* Function prototypes. */
HAL_StatusTypeDef ADS1115_Init_C0(I2C_HandleTypeDef *handler, uint16_t setDataRate, uint16_t setPGA);
HAL_StatusTypeDef ADS1115_Init_C1(I2C_HandleTypeDef *handler, uint16_t setDataRate, uint16_t setPGA);
HAL_StatusTypeDef ADS1115_readSingleEnded_C0(uint16_t muxPort, float *voltage);
HAL_StatusTypeDef ADS1115_readSingleEnded_C1(uint16_t muxPort, float *voltage);
void ads1115_read(uint8_t control);




#endif /* __ADS1115__ */