//Include library
#include "UI_process.h"
#include "snow_tiger.h"

extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi1;

char buffer_lcd[128];
int fps = 37;
float temperature = 28.5;
int gCounter =4;
//call extern variable
extern uint8_t hours, minutes, seconds, day, date, month, year;
extern float g_voltage[8];
extern float gTemperature, gHumidity;
void UI_Process_Init(void){
     ILI9341_Init(); 
     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
}

void UI_Process_Run(void){
    //uart_ctrl_receive();
    //uart_handle_mess();
    gCounter++;
    if (gCounter ==5)
    {
        ILI9341_Fill_Screen(BLACK);
		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
		ILI9341_Draw_Text("VNU_HCM", 10, 10, BLUE, 1, BLACK);
        ILI9341_Draw_Text("Ho Chi Minh City University of Technology", 10, 20, BLUE, 1, BLACK);

        // ILI9341_Draw_Text("Faculty of Electrical and Electronics Engineering", 10, 30, WHITE, 1, BLACK);
        // ILI9341_Draw_Text("Electronics Engineering Department", 10, 40, WHITE, 1, BLACK);

        //Display Real time
        snprintf(buffer_lcd, sizeof(buffer_lcd), "Time: %02d:%02d:%02d %d/%d/%d ",hours,minutes,seconds,date,month,year+2000);
        ILI9341_Draw_Text(buffer_lcd, 10, 30, WHITE, 1, BLACK);	


        //Display Analog chanel
        ILI9341_Draw_Text("Analog Display :", 10, 40, WHITE, 1, BLACK);
        for (int i = 0; i < 4; i++)
        {
            int address=0;
            if(g_voltage[i]!=-1){
                address = 50 + i*10;
                snprintf(buffer_lcd, sizeof(buffer_lcd), "Chanel %d : %.3f", i+1 ,g_voltage[i]);
                ILI9341_Draw_Text(buffer_lcd, 10, address, GREEN, 1, BLACK);	

            }else{
                address = 50 + i*10;
                snprintf(buffer_lcd, sizeof(buffer_lcd), "Chanel %d : NoConnect", i+1);
                ILI9341_Draw_Text(buffer_lcd, 10, address, GREEN, 1, BLACK);	
            }
        }

        for (int i = 4; i < 8; i++)
        {
            int address=0;
            if(g_voltage[i]!=-1){
                address = 50 + (i-4)*10;
                snprintf(buffer_lcd, sizeof(buffer_lcd), "Chanel %d : %.3f", i+1 ,g_voltage[i]);
                ILI9341_Draw_Text(buffer_lcd, 140, address, GREEN, 1, BLACK);	

            }else{
                address = 50 + (i-4)*10;
                snprintf(buffer_lcd, sizeof(buffer_lcd), "Chanel %d : NoConnect", i+1);
                ILI9341_Draw_Text(buffer_lcd, 140, address, GREEN, 1, BLACK);	
            }
        }

        //Display Digital chanel
        ILI9341_Draw_Text("Digital Display :", 10, 90, WHITE, 1, BLACK);
        for (int i = 0; i < 2; i++)
        {
            int address=0;
            if(0){
                address = 100 + i*10;
                snprintf(buffer_lcd, sizeof(buffer_lcd), "Chanel %d : NoConnect", i+1);
                ILI9341_Draw_Text(buffer_lcd, 10, address, YELLOW, 1, BLACK);	

            }else{
                address = 100 + i*10;
                snprintf(buffer_lcd, sizeof(buffer_lcd), "Chanel %d : NoConnect", i+1);
                ILI9341_Draw_Text(buffer_lcd, 10, address, YELLOW, 1, BLACK);	
            }
        }

        //Display GPS
        ILI9341_Draw_Text("GPS location :", 10, 120, WHITE, 1, BLACK);
        sprintf(buffer_lcd, "Longitude: %d", fps);
        ILI9341_Draw_Text(buffer_lcd, 10, 130, RED, 1, BLACK);
        sprintf(buffer_lcd, "Latitude: %d", fps);
        ILI9341_Draw_Text(buffer_lcd, 150, 130, RED, 1, BLACK);

        //Display Temp and Hum gTemperature , gHumidity; 
        snprintf(buffer_lcd, sizeof(buffer_lcd), "Tempeture: %.2f *C",gTemperature);
        ILI9341_Draw_Text(buffer_lcd, 10, 140, YELLOW, 1, BLACK);
        snprintf(buffer_lcd, sizeof(buffer_lcd), "Humidity: %.2f % ",gHumidity);
        ILI9341_Draw_Text(buffer_lcd, 150, 140, YELLOW, 1, BLACK);
        
        //MicroSD Display
		ILI9341_Draw_Text("MicroSD :", 10, 150, WHITE, 1, BLACK);
        ILI9341_Draw_Text("Free Space : 5612611 byte", 10, 160, ORANGE, 1, BLACK);
        
        //BAT Status Display
		ILI9341_Draw_Text("BAT : 70%", 10, 170, WHITE, 1, BLACK);
        ILI9341_Draw_Text("Charge Status : Fast!", 10, 180, ORANGE, 1, BLACK);
        
        //Source Power
		ILI9341_Draw_Text("Source Power : 12DC Input", 10, 190, WHITE, 1, BLACK);
        gCounter =0;		
    }
}