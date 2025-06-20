/*
 *  microsd.c
 *
 *  Created on: 20-June-2025
 *      Author: Huy Vu
 */
#include "microsd.h"

extern SPI_HandleTypeDef   hspi1;

void deselectSDcard(){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);}   
void selectSDcard(){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, RESET);}

static void sdPowerUp(){
	uint8_t Data [] = {0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,} ;
	uint8_t dummyByte = 0xff ;
	deselectSDcard();
	// delay some time
	for (uint16_t i = 0;  i < 1000; i++) {}
	// sending 80 clock cycles
	uint8_t R1_Response = 0x0f; // store the R1 response for CMD 0

    HAL_SPI_Transmit(&hspi1, Data, 10, HAL_MAX_DELAY);

	deselectSDcard();
    HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

	// send CMD0 with CRC
	Data[0] = 0x40 ;
	Data[1] = 0x00 ;
	Data[2] = 0x00 ;
	Data[3] = 0x00 ;
	Data[4] = 0x00 ;
	Data[5] = 0x95 ;

        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
		// delay some time
		for (uint16_t i = 0;  i < 1000; i++) {}
		selectSDcard();
        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

	// send CMD0 wait for 1 byte R1 response
    HAL_SPI_Transmit(&hspi1, Data, 6, HAL_MAX_DELAY);
	// send some clock to get the R1 response towards master

	while (R1_Response != 0x01) {
    HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

    HAL_SPI_Receive(&hspi1, &R1_Response, 1, HAL_MAX_DELAY);

#if (SD_DEV_MODE)
	printf("%d\n" , R1_Response) ;
#endif
	}

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	// delay some time
	for (uint16_t i = 0;  i < 1000; i++) {}
	deselectSDcard();
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
		printf("CMD0 r1 ideal \n") ;
#endif
}

static void sdInitSeq(){
	uint8_t Data [] = {0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,} ;
	uint8_t dummyByte = 0xff ;
	uint8_t res7 = 0 ;
#if (SD_DEV_MODE)
	printf("CMD8 \n ") ;
#endif
	Data[0] = 0x48 ;
	Data[1] = 0x00 ;
	Data[2] = 0x00 ;
	Data[3] = 0x01 ;
	Data[4] = 0xAA ;
	Data[5] = (0x86 | 0x01) ;

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	// delay some time
	for (uint16_t i = 0;  i < 1000; i++) {}
	selectSDcard() ;
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

	HAL_SPI_Transmit(&hspi1, Data, 6, HAL_MAX_DELAY);

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &res7, 1, HAL_MAX_DELAY);

#if (SD_DEV_MODE)
	printf("1 %p \n" , res7) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res7, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("2 %p \n" , res7) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res7, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("3 %p \n" , res7) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res7, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("4 %p \n" , res7) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res7, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("5 %p \n" , res7) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res7, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("6 %p \n" , res7) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res7, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("7 %p \n" , res7) ;
#endif
	// delay some time
	for (uint16_t i = 0;  i < 1000; i++) {}
	deselectSDcard();
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

}

static void readOCR() {
	uint8_t Data [] = {0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,} ;
	uint8_t dummyByte = 0xff ;

uint8_t res3 ;
#if (SD_DEV_MODE)
	printf("CMD58 \n ") ;
#endif
	Data[0] = 0x7A ;
	Data[1] = 0x00 ;
	Data[2] = 0x00 ;
	Data[3] = 0x00 ;
	Data[4] = 0x00 ;
	Data[5] = (0x00 | 0x01) ;

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	// delay some time
	for (uint16_t i = 0;  i < 1000; i++) {}
	selectSDcard() ;
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

	HAL_SPI_Transmit(&hspi1, Data, 6, HAL_MAX_DELAY);

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res3, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("1 %p \n" , res3) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res3, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("2 %p \n" , res3) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res3, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("3 %p \n" , res3) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res3, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("4 %p \n" , res3) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res3, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("5 %p \n" , res3) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res3, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("6 %p \n" , res3) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res3, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("7 %p \n" , res3) ;
#endif
	// delay some time
	for (uint16_t i = 0;  i < 1000; i++) {}
	deselectSDcard();
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

}

static void sd_final_Init(){
	uint8_t Data [] = {0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,} ;
	uint8_t dummyByte = 0xff ;
    deselectSDcard();
    HAL_SPI_DeInit(&hspi1);
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;  // low speed
    HAL_SPI_Init(&hspi1);
    uint32_t timeout = 10000;
    uint8_t res1 = 0xff ;
    while(res1 != 0x00 && timeout--){
        #if (SD_DEV_MODE)
        printf("CMD55 \n") ;
        #endif
        Data[0] = 	0x77 ;
        Data[1] = 	0x00 ;
        Data[2] = 	0x00 ;
        Data[3] = 	0x00 ;
        Data[4] = 	0x00 ;
        Data[5] =   (0x00 | 0x01);

        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
        // delay some time
        for (uint16_t i = 0;  i < 1000; i++) {}
        selectSDcard() ;
        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

        //ACMD55
        HAL_SPI_Transmit(&hspi1, Data, 6, HAL_MAX_DELAY);

        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi1, &res1, 1, HAL_MAX_DELAY);
        #if (SD_DEV_MODE)
        printf("1 %p \n" , res1) ;
        #endif
        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi1, &res1, 1, HAL_MAX_DELAY);
        #if (SD_DEV_MODE)
        printf("1 %p \n" , res1) ;
        #endif
        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
        // delay some time
        for (uint16_t i = 0;  i < 1000; i++) {}
        deselectSDcard();
        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
        #if (SD_DEV_MODE)
        printf("ACMD41 \n") ;
        #endif
        Data[0] = 	0x69 ;
        Data[1] = 	0x40 ;
        Data[2] = 	0x00 ;
        Data[3] = 	0x00 ;
        Data[4] = 	0x00 ;
        Data[5] =   (0x00 | 0x01);

        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
        // delay some time
        for (uint16_t i = 0;  i < 1000; i++) {}
        selectSDcard() ;
        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

        //ACMD41
        HAL_SPI_Transmit(&hspi1, Data, 6, HAL_MAX_DELAY);

        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi1, &res1, 1, HAL_MAX_DELAY);
        #if (SD_DEV_MODE)
        printf("1 %p \n" , res1) ;
        #endif
        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
        // delay some time
        for (uint16_t i = 0;  i < 1000; i++) {}
        deselectSDcard();
        HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
    }
    if (timeout == 0) {
        return; // error
    }
#if (SD_DEV_MODE)
printf("Init Success \n ") ;
#endif
}

static void prepReadWrite(){
    HAL_SPI_DeInit(&hspi1);
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;  // low speed
    HAL_SPI_Init(&hspi1);
}

void readBlockSingle(uint32_t blockIndex , uint8_t *buffAddr){
	uint8_t Data [] = {0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,} ;
	uint8_t dummyByte = 0xff ;
	uint8_t dummyReadByte = 0xff ;
uint8_t res1 = 0xff ;
#if (SD_DEV_MODE)
	printf("CMD17\n") ;
#endif
	Data[0] = 	0x51 ;
	Data[1] = 	(blockIndex >> 24) ;
	Data[2] = 	(blockIndex >> 16) ;
	Data[3] = 	(blockIndex >> 8) ;
	Data[4] = 	(blockIndex >> 0) ;
	Data[5] =   (0x00 | 0x01);

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	// delay some time
	for (uint16_t i = 0;  i < 1000; i++) {}
	selectSDcard() ;
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

	HAL_SPI_Transmit(&hspi1, Data, 6, HAL_MAX_DELAY);

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res1, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("1 %p \n" , res1) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res1, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("1 %p \n" , res1) ;
#endif
	for (uint16_t j = 0; j < 515; j++) {
		HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi1, &dummyReadByte, 1, HAL_MAX_DELAY);
		if (dummyReadByte == 0xFE) {
			for (uint16_t i = 0;  i < 514; i++) {
		HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi1, buffAddr, 1, HAL_MAX_DELAY);
		if (i == 510) {
#if (SD_DEV_MODE)
			printf("1 signature is %p \n" , *buffAddr) ;
#endif
		}else if (i == 511) {
#if (SD_DEV_MODE)
			printf("2 signature is %p \n" , *buffAddr) ;
#endif
		}else if (i == 512) {
#if (SD_DEV_MODE)
			printf("2 CRC is %p \n" , *buffAddr) ;
#endif
		}else if (i == 513) {
#if (SD_DEV_MODE)
			printf("2 CRC is %p \n" ,*buffAddr) ;
#endif
			return ;
		}

		buffAddr++ ;
			}
	    }
	}

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	// delay some time
	for (uint16_t i = 0;  i < 1000; i++) {}
	deselectSDcard();
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
}


void writeBlockSingle(uint32_t blockIndex , uint8_t *buffAddr){
	uint8_t Data [] = {0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,} ;
	uint8_t dummyByte = 0xff ;
	uint8_t dummyReadByte = 0xff ;
	uint8_t res1 = 0xff ;
#if (SD_DEV_MODE)
	printf("CMD24\n") ;
#endif
	Data[0] = 	0x58 ;
	Data[1] = 	(blockIndex >> 24) ;
	Data[2] = 	(blockIndex >> 16) ;
	Data[3] = 	(blockIndex >> 8) ;
	Data[4] = 	(blockIndex >> 0) ;
	Data[5] =   (0x00 | 0x01);

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	// delay some time
	for (uint16_t i = 0;  i < 1000; i++) {}
	selectSDcard() ;
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

	HAL_SPI_Transmit(&hspi1, Data, 6, HAL_MAX_DELAY);

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res1, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("1 %p \n" , res1) ;
#endif
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &res1, 1, HAL_MAX_DELAY);
#if (SD_DEV_MODE)
	printf("1 %p \n" , res1) ;
#endif
		if (res1 == 0) {
		dummyByte = 0xFE ;
		HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
		dummyByte = 0xff ;
		}
			for (uint16_t i = 0;  i < 512; i++) {
        HAL_SPI_Transmit(&hspi1, buffAddr, 1, HAL_MAX_DELAY);
		buffAddr++;
			}
			HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
            HAL_SPI_Receive(&hspi1, &dummyReadByte, 1, HAL_MAX_DELAY);

			while((dummyReadByte & 0x0F) != 0x05){
			HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
			HAL_SPI_Receive(&hspi1, &dummyReadByte, 1, HAL_MAX_DELAY);
			}
			   HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
						HAL_SPI_Receive(&hspi1, &dummyReadByte, 1, HAL_MAX_DELAY);

			while(dummyReadByte == 0x00 ){
		    HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
			HAL_SPI_Receive(&hspi1, &dummyReadByte, 1, HAL_MAX_DELAY);
			}

	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);
	// delay some time
	for (uint16_t i = 0;  i < 1000; i++) {}
	deselectSDcard();
	HAL_SPI_Transmit(&hspi1, &dummyByte, 1, HAL_MAX_DELAY);

}

void SD_init(){
	// start up sequence of sdcard
	sdPowerUp();

	// sd card init sequence like voltage verification
	sdInitSeq();

	// read OCR CCS field
	readOCR();

	// send ACMD41
	sd_final_Init(1);

	// read OCR CCS field again
	readOCR();

	// perepare for read and write
	prepReadWrite();    
}