//Include library
#include "UI_process.h"

void UI_Process_Init(void){
    
}

void UI_Process_Run(void){
    uart_ctrl_receive();
    uart_handle_mess();
}