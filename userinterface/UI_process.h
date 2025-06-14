#ifndef __UI_PROCESS__
#define __UI_PROCESS__

//Include library
#include "process.h"
#include "string.h"
#include "stdbool.h"
#include "uart_ctrl.h"
//Define
#define UART_RX_BUFFER_SIZE 128

void UI_Process_Init(void);
void UI_Process_Run(void);


#endif /* __UI_PROCESS__ */