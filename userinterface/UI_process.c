//Include library
#include "UI_process.h"
#include "snow_tiger.h"

extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi1;

char buffer_lcd[64];
int fps = 37;
float temperature = 28.5;


void UI_Process_Init(void){
     ILI9341_Init(); 
     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
}

void UI_Process_Run(void){
    //uart_ctrl_receive();
    //uart_handle_mess();
        ILI9341_Fill_Screen(BLACK);
		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
		ILI9341_Draw_Text("FPS TEST, 40 loop 2 screens", 10, 10, WHITE, 1, BLACK);
        
		ILI9341_Draw_Text("Randomly placed and sized", 10, 20, WHITE, 1, BLACK);
        
		ILI9341_Draw_Text("Filled Rectangles", 10, 30, WHITE, 1, BLACK);
        
		ILI9341_Draw_Text("RGB Picture", 10, 40, WHITE, 1, BLACK);
		ILI9341_Draw_Text("TIGER", 10, 50, WHITE, 1, BLACK);
        
        sprintf(buffer_lcd, "FPS: %d", fps);
        ILI9341_Draw_Text(buffer_lcd, 10, 60, WHITE, 1, BLACK);
        
        sprintf(buffer_lcd, "Temp: %.1f C", temperature);
        ILI9341_Draw_Text(buffer_lcd, 10, 70, WHITE, 1, BLACK);
		HAL_Delay(5000);			

}