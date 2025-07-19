#include "uart_ctrl.h"

extern UART_HandleTypeDef huart2;


//Volatile Variable
volatile    bool    bStatusADC  = true;
volatile    bool    bStatusGPS  = true;
volatile    bool    bStatusRTC  = true;
volatile    bool    bStatusTEMP = true;
volatile    bool    bStatusLCD  = true;

extern uint8_t Data_RX[128];
extern uint8_t gSelectChADC;
//Global Variable
static char     gUartLineBuffer[12];
static bool     bDataInProcess = false;
volatile        uint8_t Index = 0;
extern          uint8_t gByte;
char            gMessRespone[100];
extern bool     bLiveData;

//Control
extern bool bAnalogEnable              ;
extern bool bGPSEnable                 ;
extern bool bDigitalEnable             ;
extern bool bTempHumEnable             ;
extern bool bRTCEnable                 ;
extern bool bLogDataToMicroSD          ;

extern uint16_t gLowThresHold[8];
extern uint16_t gHighThresHold[8];
extern uint8_t  gUnit[8];
uint16_t        gAddressEE;
uint8_t         BufferSave[2];  
uint8_t         ChanelSelect =0;
bool            bIsConfig = false;
bool            bIsSelect = false;

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

void uart_proc_config(void) {
    char resp[128];
    bLiveData = false;
    bIsConfig = true;
    sprintf(resp, ">>Please select channel config\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)resp, strlen(resp), HAL_MAX_DELAY);
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
                sscanf((char*)Data_RX, "AT+ENAADC=%hhu", &gSelectChADC);
                sprintf(gMessRespone, ">>ADC IS ENABLE!\r\n");
                HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                bAnalogEnable  = true;
                bDataInProcess = false;
                break;

            case CMD_DISADC:
                bStatusADC  = false;
                sprintf(gMessRespone, ">>ADC IS DISBLE!\r\n");
                HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                bDataInProcess = false;
                break;

            case CMD_LIVEDATA:
                bLiveData = true;
                sprintf(gMessRespone, ">>LIVEDATA IS ENABLE!\r\n");
                HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                bDataInProcess = false;              
                break; 

            case CMD_DISLIVEDATA:
                bLiveData = false;
                sprintf(gMessRespone, ">>LIVEDATA IS DISBLE!\r\n");
                HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                bDataInProcess = false;              
                break; 

            case CMD_CONFIG:
                uart_proc_config();

                break;

            case CMD_SELECT: 
                if(bIsConfig){
                    sscanf((char*)Data_RX, "AT+SELECT=%d", &ChanelSelect);
                    if (ChanelSelect < 1 || ChanelSelect > 8 ) {
                        sprintf(gMessRespone, ">>FAIL\r\n");
                    } else {
                        sprintf(gMessRespone, ">>Your select is chanel %d\r\n >>Please set range according A-->B!\r\n",ChanelSelect);
                        ChanelSelect = ChanelSelect - 1;
                    }
                    HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                    bIsSelect = true;
                }
                bDataInProcess = false;
                break;
            
            case CMD_SETA:
                if(bIsSelect){
                    sscanf((char*)Data_RX, "AT+SETA=%d", &gLowThresHold[ChanelSelect]);
                    // gAddressEE = Get_Address_EEPROM(ChanelSelect);
                    // BufferSave[0] = (gLowThresHold[ChanelSelect] >> 8) & 0xFF;           // High byte   
                    // BufferSave[1] = gLowThresHold[ChanelSelect] & 0xFF;                  // High byte 
                    // prj_eeprom_24lc256_write_bytes(gAddressEE, BufferSave, strlen((char*)BufferSave));
                    sprintf(gMessRespone, ">>Config Chanel %d A = %d\r\n",ChanelSelect, gLowThresHold[ChanelSelect]);
                    HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                }
                bDataInProcess = false;
                break;

            case CMD_SETB:
                if(bIsSelect){
                    sscanf((char*)Data_RX, "AT+SETB=%d", &gHighThresHold[ChanelSelect]);
                    // gAddressEE = Get_Address_EEPROM(ChanelSelect)+2;
                    // BufferSave[0] = (gHighThresHold[ChanelSelect] >> 8) & 0xFF;           // High byte   
                    // BufferSave[1] = gHighThresHold[ChanelSelect] & 0xFF;                  // High byte 
                    // prj_eeprom_24lc256_write_bytes(gAddressEE, BufferSave, strlen((char*)BufferSave));
                    sprintf(gMessRespone, ">>Config Chanel %d B = %d\r\n",ChanelSelect, gHighThresHold[ChanelSelect]);
                    HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                    bIsSelect = false;
                }
                bDataInProcess = false;
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
    else if (strncmp(cmd, "AT+ENALIVE",10) == 0) return CMD_LIVEDATA;
    else if (strncmp(cmd, "AT+DISLIVE",10) == 0) return CMD_DISLIVEDATA;
    else if (strncmp(cmd, "AT+CONFIG",9)  == 0) return CMD_CONFIG;
    else if (strncmp(cmd, "AT+EXIT",7)    == 0) return CMD_EXIT;
    else if (strncmp(cmd, "AT+STATUS",8)  == 0) return CMD_STATUS;
    else if (strncmp(cmd, "AT+SET=",7)== 0) return CMD_SET;
    else if (strncmp(cmd, "AT+SETA=",8)== 0) return CMD_SETA;
    else if (strncmp(cmd, "AT+SETB=",8)== 0) return CMD_SETB;
    else if (strncmp(cmd, "AT+SELECT=",10)== 0) return CMD_SELECT;
    return CMD_NONE;
}

uint16_t Get_Address_EEPROM(uint8_t Select){
    switch (Select)
    {
    case 0:
        return 0x10;
        break;
    case 1:
        return 0x20;
        break;
    case 2:
        return 0x30;
        break;
    case 3:
        return 0x40;
        break;
    case 4:
        return 0x50;
        break;
    case 5:
        return 0x60;
        break;
    case 6:
        return 0x70;
        break;
    case 7:
        return 0x80;
        break;       
    
    default:
        return 0x00;
        break;
    }

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
