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
#include "mcp23008.h"
#include "rtc.h"
#include "button.h"
#include "24lc256.h"
#include "convert.h"
//Define

//COMMAND INIT ERROR
#define     ERROR_INIT_ADC_CH0          0x1
#define     ERROR_INIT_ADC_CH1          0x2
#define     ERROR_INIT_RTC              0x3
#define     ERROR_INIT_GPS              0x4
#define     ERROR_INIT_TEMP             0x5
#define     ERROR_INIT_RTX              0x6
#define     ERROR_INIT_EXPAND_IO        0x7

//Adress EEPROM CONFIG
#define     A_CHANEL1                   0x10
#define     A_CHANEL2                   0x20
#define     A_CHANEL3                   0x30
#define     A_CHANEL4                   0x40
#define     A_CHANEL5                   0x50
#define     A_CHANEL6                   0x60
#define     A_CHANEL7                   0x70
#define     A_CHANEL8                   0x80

//Function

typedef enum{
    CMD_NO_ACTION       ,
    CMD_LIVE_END        ,
    CMD_LIVE_DIS        
}eControlSys;

typedef enum {
    SYSTEM_OFF = 0,
    SYSTEM_ON  = 1
} SystemState;

typedef enum {
    USB_OFF = 0,
    USB_ON  = 1
} StateUSB;

extern SystemState systemState;
extern StateUSB stateUSB;

void Process_Init(void);
void Process_Run(void);


#endif /* __PROCESS__ */