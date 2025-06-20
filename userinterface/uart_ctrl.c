#include "uart_ctrl.h"

extern UART_HandleTypeDef huart2;


//Volatile Variable
volatile    bool    bStatusADC  = true;
volatile    bool    bStatusGPS  = true;
volatile    bool    bStatusRTC  = true;
volatile    bool    bStatusTEMP = true;
volatile    bool    bStatusLCD  = true;

//Global Variable
static char gUartLineBuffer[128];
static bool bDataInProcess = false;
static uint8_t Index = 0;

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
//        bDataInProcess = true;
        char ack[] = "OK\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)ack, strlen(ack), 10);

    }
}


void uart_handle_mess(void){
    if(bDataInProcess){
        if (strncmp(gUartLineBuffer, "AT+", 3) == 0){
            AT_Command_t StatusCommnad = identify_command(gUartLineBuffer);
            switch (StatusCommnad){
            case CMD_ENAADC:
                bStatusADC  = true;
                bDataInProcess = false;
                break;

            case CMD_DISADC:
                bStatusADC  = false;
                bDataInProcess = false;
                break;

            case CMD_CONFIG:
                uart_proc_config();

                break;

            default:
                bDataInProcess = false;
                break;
            }


        }else{
            bDataInProcess = false;
        }   
    }

}

void uart_proc_config(void){


    bDataInProcess = false;
}

AT_Command_t identify_command(const char *cmd)
{
    if      (strcmp(cmd, "AT+ENAADC")  == 0) return CMD_ENAADC;
    else if (strcmp(cmd, "AT+DISADC")  == 0) return CMD_DISADC;
    else if (strcmp(cmd, "ENAGPS")  == 0) return CMD_ENAGPS;
    else if (strcmp(cmd, "DISGPS")  == 0) return CMD_DISGPS;
    else if (strcmp(cmd, "ENARTC")  == 0) return CMD_ENARTC;
    else if (strcmp(cmd, "DISRTC")  == 0) return CMD_DISRTC;
    else if (strcmp(cmd, "ENALCD")  == 0) return CMD_ENALCD;
    else if (strcmp(cmd, "DISLCD")  == 0) return CMD_DISLCD;
    else if (strcmp(cmd, "ENATEMP") == 0) return CMD_ENATEMP;
    else if (strcmp(cmd, "DISTEMP") == 0) return CMD_DISTEMP;
    else if (strcmp(cmd, "CONFIG")  == 0) return CMD_CONFIG;
    else if (strcmp(cmd, "EXIT")    == 0) return CMD_EXIT;
    else if (strcmp(cmd, "STATUS")  == 0) return CMD_STATUS;
    else if (strncmp(cmd, "SET=", 4)== 0) return CMD_SET;
    return CMD_NONE;
}
