#ifndef __UART_CTRL__
#define __UART_CTRL__

//Include library
#include "process.h"
#include "string.h"
#include "stm32f4xx_it.h"
#include "stdbool.h"
#include "stdio.h"
//Define
#define UART_RX_BUFFER_SIZE 128

//Enum
typedef enum {
    CMD_NONE            ,
    CMD_ENAADC          ,
    CMD_DISADC          ,
    CMD_ENAGPS          ,
    CMD_DISGPS          ,
    CMD_ENARTC          ,
    CMD_DISRTC          ,
    CMD_ENALCD          ,
    CMD_DISLCD          ,
    CMD_ENATEMP         ,
    CMD_DISTEMP         ,
    CMD_CONFIG          ,       
    CMD_EXIT            ,
    CMD_STATUS          ,
    CMD_LIVEDATA        ,
    CMD_DISLIVEDATA     ,
    CMD_GETDATA         ,
    CMD_SET
} AT_Command_t;

//Function

void uart_ctrl_receive(void);
void uart_handle_mess(void);
void uart_proc_config(void);
AT_Command_t identify_command(const char *cmd);



#endif /* __UART_CTRL__ */