#include "uart_ctrl.h"
#include <math.h>

extern UART_HandleTypeDef huart2;

//Volatile Variable
volatile    bool    bStatusADC  = true;
volatile    bool    bStatusGPS  = true;
volatile    bool    bStatusRTC  = true;
volatile    bool    bStatusTEMP = true;
volatile    bool    bStatusLCD  = true;

extern uint8_t Data_RX[128];
extern uint8_t gSelectChADC;

extern float g_voltage[8];
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

ChannelConfig_t gChannelConfig[8];
static int gSelectedConfigCh = -1;           // Track which channel is being configured

float apply_formula(float x, const char* formula) {
    float y = 0.0f;
    float a = 1.0f, b = 0.0f;

    if (sscanf(formula, "y=x*%f+%f", &a, &b) == 2)
        y = x * a + b;
    else if (sscanf(formula, "y=%fx+%f", &a, &b) == 2)
        y = a * x + b;
    else if (sscanf(formula, "y=x/%f+%f", &a, &b) == 2)
        y = x / a + b;
    else
        y = x;

    return y;
}

void uart_display_live_data(void) {
    char resp[128];
    for (int i = 0; i < 8; ++i) {
        if ((gSelectChADC & (1 << i))) {
            float val = apply_formula(g_voltage[8], gChannelConfig[i].formula);
            sprintf(resp, "CH%d = %.2f%s\r\n", i + 1, val,
                gChannelConfig[i].unit[0] ? gChannelConfig[i].unit : "");
            HAL_UART_Transmit(&huart2, (uint8_t*)resp, strlen(resp), HAL_MAX_DELAY);
        }
    }
}

void uart_show_config(void) {
    char resp[128];
    for (int i = 0; i < 8; i++) {
        if (gSelectChADC & (1 << i)) {
            if (gChannelConfig[i].configured) {
                sprintf(resp, "CH%d = %s [%s]\r\n", i + 1, gChannelConfig[i].formula, gChannelConfig[i].unit);
            } else {
                sprintf(resp, "CH%d = Not Configured\r\n", i + 1);
            }
            HAL_UART_Transmit(&huart2, (uint8_t*)resp, strlen(resp), HAL_MAX_DELAY);
        }
    }
}

void uart_proc_config(void) {
    char resp[128];
    bLiveData = false;
    sprintf(resp, ">>Please select channel config\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)resp, strlen(resp), HAL_MAX_DELAY);

    for (int i = 0; i < 8; ++i) {
        sprintf(resp, "CH%d : %s\r\n", i + 1,
                gSelectChADC & (1 << i) ?
                (gChannelConfig[i].configured ? "Configured" : "Enable") : "Non Active");
        HAL_UART_Transmit(&huart2, (uint8_t*)resp, strlen(resp), HAL_MAX_DELAY);
    }
}



void uart_ctrl_receive(void){
    if(!bDataInProcess){
        bDataInProcess = true;
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

            case CMD_SELECT: {
                int ch;
                sscanf((char*)Data_RX, "AT+SELECT=%d", &ch);
                if (ch < 1 || ch > 8 || !(gSelectChADC & (1 << (ch - 1)))) {
                    sprintf(gMessRespone, ">>FAIL\r\n");
                } else {
                    gSelectedConfigCh = ch - 1;
                    sprintf(gMessRespone, ">>OK\r\n>>Please Apply your formular with \"x\" is adc variable example  y = ax +b\r\n");
                }
                HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                bDataInProcess = false;
                break;
            }

            case CMD_FORMULER:
                if (gSelectedConfigCh >= 0 && gSelectedConfigCh < 8) {
                    char* eq = strchr((char*)Data_RX, '=');
                    if (eq && strlen(eq + 1) < sizeof(gChannelConfig[gSelectedConfigCh].formula)) {
                        strcpy(gChannelConfig[gSelectedConfigCh].formula, eq + 1);
                        gChannelConfig[gSelectedConfigCh].configured = true;
                        sprintf(gMessRespone, ">>OK\r\n>>your formula is %s\r\n>>Please enter your unit if dont have please enter 0\r\n", gChannelConfig[gSelectedConfigCh].formula);
                    } else {
                        sprintf(gMessRespone, ">>INVALID FORMULA\r\n");
                    }
                    HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                }
                bDataInProcess = false;
                break;

            case CMD_UNIT:
                if (gSelectedConfigCh >= 0 && gSelectedConfigCh < 8) {
                    char* eq = strchr((char*)Data_RX, '=');
                    if (eq) {
                        if (strcmp(eq + 1, "0") == 0) gChannelConfig[gSelectedConfigCh].unit[0] = '\0';
                        else strncpy(gChannelConfig[gSelectedConfigCh].unit, eq + 1, sizeof(gChannelConfig[gSelectedConfigCh].unit));

                        sprintf(gMessRespone, ">>Your unit enter is : %s\r\n", gChannelConfig[gSelectedConfigCh].unit);
                    } else {
                        sprintf(gMessRespone, ">>INVALID UNIT\r\n");
                    }
                    HAL_UART_Transmit(&huart2, (uint8_t*)gMessRespone, strlen(gMessRespone), HAL_MAX_DELAY);
                }
                bDataInProcess = false;

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
    else if (strncmp(cmd, "AT+SELECT=",10) == 0) return CMD_SELECT;
    else if (strncmp(cmd, "AT+FORMULER=",12) == 0) return CMD_FORMULER;
    else if (strncmp(cmd, "AT+UNIT=",8) == 0) return CMD_UNIT;
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
