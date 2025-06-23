#ifndef __PROCESS__
#define __PROCESS__

//Include library
#include "stm32f4xx_hal.h"
#include "main.h"
#include "ads1115.h"
#include "sht31.h"
#include "fatfs.h"
#include "ff.h"
#include <stdio.h>
#include <string.h>
//Define

//COMMAND INIT ERROR
#define     ERROR_INIT_ADC_CH0          0x1
#define     ERROR_INIT_ADC_CH1          0x2
#define     ERROR_INIT_RTC              0x3
#define     ERROR_INIT_GPS              0x4
#define     ERROR_INIT_TEMP             0x5
#define     ERROR_INIT_RTX              0x6
#define     ERROR_INIT_EXPAND_IO        0x7

//Function

typedef enum{
    CMD_NO_ACTION       ,
    CMD_LIVE_END        ,
    CMD_LIVE_DIS        
}eControlSys;

void Process_Init(void);
void Process_Run(void);


#endif /* __PROCESS__ */