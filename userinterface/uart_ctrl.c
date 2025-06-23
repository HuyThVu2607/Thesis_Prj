#include "uart_ctrl.h"

extern UART_HandleTypeDef huart2;


//Volatile Variable
volatile    bool    bStatusADC  = true;
volatile    bool    bStatusGPS  = true;
volatile    bool    bStatusRTC  = true;
volatile    bool    bStatusTEMP = true;
volatile    bool    bStatusLCD  = true;

extern uint8_t Data_RX[128];
//Global Variable
static char gUartLineBuffer[12];
static bool bDataInProcess = false;
volatile uint8_t Index = 0;
extern uint8_t gByte;
char    gMessRespone[100];

void uart_ctrl_receive(void){
    if(!bDataInProcess){
//        uint8_t lUartDataReceive;
//        if(HAL_UART_Receive(&huart2, &lUartDataReceive, 1, 5) == HAL_OK){
//            if ((lUartDataReceive == '\n' || lUartDataReceive == '\r')){
//                gUartLineBuffer[Index] = '\0';
//                Index = 0;
//                bDataInProcess = true;
//                char ack[] = "OK\r\n";
//                HAL_UART_Transmit(&huart2, (uint8_t *)ack, strlen(ack), HAL_MAX_DELAY);
//            }else if(Index < UART_RX_BUFFER_SIZE - 1){
//                gUartLineBuffer[Index++] = lUartDataReceive;
//            }
//        }
//        char *Clean = gUartLineBuffer;
//        while (*Clean != '\0') {
//            if (*Clean == '\r' || *Clean == '\n') {
//                *Clean = '\0';  
//                break;
//            }
//            Clean++;
//        }
        bDataInProcess = true;
//        char ack[] = "OK\r\n";
//        HAL_UART_Transmit(&huart2, (uint8_t *)ack, sizeof(ack), 10);

    }
}


void uart_handle_mess(void){
    if(bDataInProcess){
        if (strncmp(Data_RX, "AT+", 3) == 0){
            char ack[] = ">>OK\r\n";
            HAL_UART_Transmit(&huart2, (uint8_t *)ack, strlen(ack), 10);
            AT_Command_t StatusCommnad = identify_command(Data_RX);
            switch (StatusCommnad){
            case CMD_ENAADC:
                bStatusADC  = true;
                sprintf(gMessRespone, ">>ADC IS ENABLE!\r\n");
                HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                bDataInProcess = false;
                break;

            case CMD_DISADC:
                bStatusADC  = false;
                sprintf(gMessRespone, ">>ADC IS DISBLE!\r\n");
                HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                bDataInProcess = false;
                break;

            case CMD_CONFIG:
                uart_proc_config();

                break;

            default:
                sprintf(gMessRespone, ">>INVALID COMMAND!\r\n");
                HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                bDataInProcess = false;
                break;
            }
            return;


        }else{
//            sprintf(gMessRespone, "WRONG KEY!\r\n");
//            HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
            bDataInProcess = false;
            return;
        }   
    }

}

void uart_proc_config(void){


    bDataInProcess = false;
}

AT_Command_t identify_command(const char *cmd)
{
    if      (strncmp(cmd, "AT+ENAADC",9)  == 0) return CMD_ENAADC;
    else if (strncmp(cmd, "AT+DISADC",9)  == 0) return CMD_DISADC;
    else if (strncmp(cmd, "AT+ENAGPS",9)  == 0) return CMD_ENAGPS;
    else if (strncmp(cmd, "AT+DISGPS",9)  == 0) return CMD_DISGPS;
    else if (strncmp(cmd, "AT+ENARTC",9)  == 0) return CMD_ENARTC;
    else if (strncmp(cmd, "AT+DISRTC",9)  == 0) return CMD_DISRTC;
    else if (strncmp(cmd, "AT+ENALCD",9)  == 0) return CMD_ENALCD;
    else if (strncmp(cmd, "AT+DISLCD",9)  == 0) return CMD_DISLCD;
    else if (strncmp(cmd, "AT+ENATEMP",10) == 0) return CMD_ENATEMP;
    else if (strncmp(cmd, "AT+DISTEMP",10) == 0) return CMD_DISTEMP;
    else if (strncmp(cmd, "AT+CONFIG",9)  == 0) return CMD_CONFIG;
    else if (strncmp(cmd, "AT+EXIT",7)    == 0) return CMD_EXIT;
    else if (strncmp(cmd, "AT+STATUS",8)  == 0) return CMD_STATUS;
    else if (strncmp(cmd, "AT+SET=",7)== 0) return CMD_SET;
    return CMD_NONE;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if (huart->Instance == huart2.Instance)
  {
    //HAL_UART_Receive_IT(&huart2, Data_RX, 128);
        if (gByte == '\r' || gByte == '\n') {
        Data_RX[Index] = '\0'; 
        Index = 0;
        bDataInProcess = true;
        uart_handle_mess();
        }
        else {
            if (Index < UART_RX_BUFFER_SIZE - 1) {
                Data_RX[Index++] = gByte;
            }
        }
        HAL_UART_Receive_IT(&huart2, &gByte, 1);
  }
  
}
