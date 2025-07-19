/**
  ******************************************************************************
  * @file    24lc256.h
  * @author  Ulad Shumeika
  * @version v1.0
  * @date    09 June 2023
  * @brief   Header file of EEPROM 24LC256 driver.
  *
  ******************************************************************************
  */

#ifndef __24lc256_h
#define __24lc256_h

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "process.h"

//---------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------


#define PRJ_24LC256_MSG_SIZE					 	 (93U)

#define PRJ_24LC256_WP_ENABLED						 (0U)

#if(PRJ_24LC256_WP_ENABLED == 1U)
	#define PRJ_24LC256_WP_PORT						 GPIOD
	#define PRJ_24LC256_WP_PIN						 GPIO_PIN_7
#endif

//---------------------------------------------------------------------------
// Types
//---------------------------------------------------------------------------

/*!
 * @brief dma handlers' pointers structure definition.
 * @note  This structure is needed to store pointers to dma handlers.
 */
/**
 * @brief Test connection with the EEPROM device
 * @retval 0 if successful, 1 if failed
 */
uint32_t prj_eeprom_24lc256_connect_test(void);

/**
 * @brief Write data to the static memory region (starting at 0x0000, max 64 bytes)
 * @param[in] data Pointer to the data buffer
 * @param[in] data_size Number of bytes to write
 * @retval 0 if successful, 1 if failed
 */
uint32_t prj_eeprom_24lc256_write_static(void* data, uint8_t data_size);

/**
 * @brief Read data from the static memory region (starting at 0x0000, max 64 bytes)
 * @param[out] data Pointer to the buffer to store the read data
 * @param[in] data_size Number of bytes to read
 * @retval 0 if successful, 1 if failed
 */
uint32_t prj_eeprom_24lc256_read_static(void* data, uint8_t data_size);

/**
 * @brief Write data to a specific address in EEPROM
 * @param[in] mem_address 16-bit memory address (0x0000–0x7FFF)
 * @param[in] data Pointer to the data buffer
 * @param[in] size Number of bytes to write
 * @retval 0 if successful, 1 if failed
 */
uint32_t prj_eeprom_24lc256_write_bytes(uint16_t mem_address, uint8_t* data, uint16_t size);

/**
 * @brief Read data from a specific address in EEPROM
 * @param[in] mem_address 16-bit memory address (0x0000–0x7FFF)
 * @param[out] data Pointer to the buffer to store the read data
 * @param[in] size Number of bytes to read
 * @retval 0 if successful, 1 if failed
 */
uint32_t prj_eeprom_24lc256_read_bytes(uint16_t mem_address, uint8_t* data, uint16_t size);

/**
 * @brief Erase the entire EEPROM (fills memory with 0xFF)
 * @retval 0 if successful, 1 if failed
 */
uint32_t prj_eeprom_24lc256_erase_memory(void);

#endif /* __24lc256_h */