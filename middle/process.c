#include "process.h"
#include "stdbool.h"

//Global Variable
volatile bool gCheckInit = true;
volatile uint32_t delayCounter	=	0;
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);

//Init system
FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char buffer[100];
uint8_t gByte;
uint8_t Data_RX[128];
uint8_t gSelectChADC=255;
float g_voltage[8];
uint32_t gDigital[4];
bool bLiveData = true;


//Config
uint16_t gLowThresHold[8];
uint16_t gHighThresHold[8];

uint16_t gLowThresHoldApply[8];
uint16_t gHighThresHoldApply[8];


uint8_t  gUnit[8];

//Varible control State 
bool bAnalogEnable       = true;
bool bGPSEnable          = false;
bool bDigitalEnable      = false;
bool bTempHumEnable      = true;
bool bRTCEnable          = true;
bool bLogDataToMicroSD   = false;

bool bIsSave             = false;
//Struct MCP23008
MCP23008_t mcp;
uint8_t gpio_values=0;
uint8_t gCheckPin=0;
//Variable for RTC 
uint8_t hours, minutes, seconds, day, date, month, year;

//Variable for SHT31
float gTemperature, gHumidity;

//Variable for EEPROM
uint8_t write_data[] = "Hello EADCOM";
uint8_t read_data[300] = {0};
uint8_t gCheckData[8];
uint8_t gGetData[2];
uint16_t gAddressGet;
//Power system
SystemState systemState = SYSTEM_ON;
StateUSB stateUSB = USB_OFF;

//Send data to esp32
char uart_buf_esp32[256];
uint8_t gCounterSendEsp32 =0;

void Process_Init(void){
    //Init peripheral
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_I2C2_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    MX_FATFS_Init();
    HAL_Delay(500);

    //For test SD card
    /* Mount SD Card */
//	fres =f_mount(&fs, "", 0);
//
//	/* Open file to write */
//	fres = f_open(&fil, "first.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
//
//	/* Check freeSpace space */
//	fres = f_getfree("", &fre_clust, &pfs);
//
//	totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
//	freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);
//
//	/* free space is less than 1kb */
//	if(freeSpace < 1)
//
//	/* Writing text */
//	f_puts("STM32 SD Card I/O Example via SPI\n", &fil);
//	f_puts("Save the world!!!", &fil);
//
//	/* Close file */
//	fres = f_close(&fil);
//
//	/* Open file to read */
//	fres = f_open(&fil, "first.txt", FA_READ);
//
//	while(f_gets(buffer, sizeof(buffer), &fil))
//	{
//		/* SWV output */
//		printf("%s", buffer);
//		fflush(stdout);
//	}
//
//	/* Close file */
//	if(f_close(&fil) != FR_OK)
//		//_Error_Handler(__FILE__, __LINE__);
//
//	/* Unmount SDCARD */
//	if(f_mount(NULL, "", 1) != FR_OK)
//		//_Error_Handler(__FILE__, __LINE__);

  //End
    char errMsg[] = "BEGIN TEST!\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)errMsg, strlen(errMsg), HAL_MAX_DELAY);
        
    //Init interrupt receive uart2
    HAL_UART_Receive_IT(&huart2, &gByte, 1);
    //Init External ADC
    if(ADS1115_Init_C0(&hi2c2, ADS1115_DATA_RATE_128, ADS1115_PGA_ONE) != HAL_OK){
         gCheckInit = false;
    }
    if(ADS1115_Init_C1(&hi2c2, ADS1115_DATA_RATE_128, ADS1115_PGA_ONE) != HAL_OK){
         gCheckInit = false;
    }
    //Init ExpandIO MCP23008
    MCP23008_Init(&mcp, &hi2c2, MCP23008_ADDRESS);
    //Set mcp23008 mode input
    MCP23008_PinMode(&mcp, 3, 1);  // 1 = INPUT
    MCP23008_PullUp(&mcp, 3, 1);   //pull up


      //Init RTC
//    uint8_t set_hour = 1;
//    uint8_t set_minute = 15;
//    uint8_t set_second = 23;
//    uint8_t set_date = 8;
//    uint8_t set_day = 13;
//    uint8_t set_month = 7;
//    uint8_t set_year = 25;

    RTC_Init(&hi2c1);
    //RTC_SetTime(set_hour, set_minute, set_second, set_date, set_day, set_month, set_year);
    //Init
    
    //Init EEPROM
//     uint8_t erease = 0xFF;
//     for(int i = 0; i<400; i++){
//         prj_eeprom_24lc256_write_bytes(i, &erease, 1);
//     }
//    prj_eeprom_24lc256_read_bytes(0x0000, read_data, 300);
    for (int i = 0; i < 8; i++)
    {
        gAddressGet = 10 + 10*i;
        prj_eeprom_24lc256_read_bytes(gAddressGet, gGetData, 2);
        gLowThresHoldApply[i] = ((uint16_t)gGetData[0] << 8) | gGetData[1];
        HAL_Delay(10);
        gAddressGet = gAddressGet + 2;
        prj_eeprom_24lc256_read_bytes(gAddressGet, gGetData, 2);
        gHighThresHoldApply[i] = ((uint16_t)gGetData[0] << 8) | gGetData[1];
        HAL_Delay(10);
    }
    
    
}

uint16_t TestBuffer=2212;
uint8_t  gBuffer[2];
uint8_t  gBuffer1[2];
uint16_t  AddressCH;
uint8_t  Results;
//Middle system run
void Process_Run(void){
    //Get real time clock
    //prj_eeprom_24lc256_write_bytes(AddressTest, gBuffer, strlen((char*)gBuffer));
    for(int i=0; i<8; i++){
          if(gLowThresHold[i]!=0){
                gBuffer[0] = (gLowThresHold[i] >> 8) & 0xFF;                  // High byte   
                gBuffer[1] = gLowThresHold[i] & 0xFF;                         // High byte 
                gLowThresHold[i] =0;
                AddressCH = 10 +i*10;
                prj_eeprom_24lc256_write_bytes(AddressCH, gBuffer, 2);
          }
          
          if(gHighThresHold[i]!=0){
                gBuffer1[0] = (gHighThresHold[i] >> 8) & 0xFF;                  // High byte   
                gBuffer1[1] = gHighThresHold[i] & 0xFF;                         // High byte 
                gHighThresHold[i] =0;
                AddressCH = 12 +i*10;
                prj_eeprom_24lc256_write_bytes(AddressCH, gBuffer1, 2);
          }
    }
//    prj_eeprom_24lc256_read_bytes(0x10, gCheckData, 8);
//    Results = gCheckData[0];
    if(bRTCEnable){
      //Function get RTC
      RTC_GetTime(&hours, &minutes, &seconds, &day, &date, &month, &year);
    }
    //Get data of analog chanel
    if(bAnalogEnable){
      //Function get Analog
      gpio_values = MCP23008_ReadGPIO(&mcp);
      ads1115_read(gSelectChADC);

    }

    //Get data og digital chanel
    if(bDigitalEnable){
      //Function get Digital

    }

    //Get GPS location
    if(bGPSEnable){
      //Function get GPS
    }

    // Get Tempeture and Humidity
    if(bTempHumEnable){
      //Function get Tempeture and Humidity
        SHT31_GetTempAndHumidity(&gTemperature, &gHumidity);
    }

    //Save data for micro SD card
    if (bLogDataToMicroSD)
    {
      //Function save data to micro SD card

    }
    

    if(bLiveData){
        char LogMsg[100];
        sprintf(LogMsg, ">>------------------------------------------------<<\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
        sprintf(LogMsg, ">>ADC LIVE DATA!\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
        
        //Show RTC
        if (bRTCEnable)
        {
          //Function Show RTC
          snprintf(LogMsg, sizeof(LogMsg), ">>Time: %02d:%02d:%02d %d/%d/%d  \r\n",hours,minutes,seconds,date,month,year);
		      HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), 100);

        }else{
          sprintf(LogMsg, ">>Real Time Clock None Active!\r\n");
          HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
        }

        //Show ADC value of chanel
        if (bAnalogEnable)
        {  
          gCheckPin = gpio_values & 0x08;
          if (gCheckPin == 0) {
              g_voltage[0]=g_voltage[0]/1000*2.5-2.5+0.34;
              if (gLowThresHoldApply[0]!=0xFFFF && gHighThresHoldApply[0]!=0xFFFF)
              {
                g_voltage[0] = Convert_To_Engineering_Value(SIGNAL_0_10V, g_voltage[0], gLowThresHoldApply[0], gHighThresHoldApply[0]);
              }       
              sprintf(LogMsg, ">>ADC CHANEL0 = %.4f V\r\n",g_voltage[0]);
              HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
          }else{
              g_voltage[0]= -1;
              sprintf(LogMsg, ">>ADC CHANEL0 No Input\r\n");
              HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
          }
          gCheckPin = gpio_values & 0x04;
          
          if (gCheckPin == 0) {
              g_voltage[1]=g_voltage[1]/1000*2.5-2.5+0.09;
              if (gLowThresHoldApply[1]!=0xFFFF && gHighThresHoldApply[1]!=0xFFFF)
              {
                g_voltage[0] = Convert_To_Engineering_Value(SIGNAL_0_10V, g_voltage[1], gLowThresHoldApply[1], gHighThresHoldApply[1]);
              }    
              sprintf(LogMsg, ">>ADC CHANEL1 = %.4f V\r\n",g_voltage[1]);
              HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
          }else{
              g_voltage[1]= -1;
              sprintf(LogMsg, ">>ADC CHANEL1 No Input\r\n");
              HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
          }
           
          for(int i = 2; i < 4; i++){
              g_voltage[i]=g_voltage[i]/1000+0.1;
              if (gLowThresHoldApply[i]!=0xFFFF && gHighThresHoldApply[i]!=0xFFFF)
              {
                g_voltage[i] = Convert_To_Engineering_Value(SIGNAL_0_5V, g_voltage[i], gLowThresHoldApply[i], gHighThresHoldApply[i]);
              }   
              sprintf(LogMsg, ">>ADC CHANEL%d = %.4f V\r\n",i,g_voltage[i]);
              HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
            }

          for(int i = 4; i < 8; i++){
              g_voltage[i]=g_voltage[i]/250;
              if (gLowThresHoldApply[i]!=0xFFFF && gHighThresHoldApply[i]!=0xFFFF)
              {
                g_voltage[i] = Convert_To_Engineering_Value(SIGNAL_4_20mA, g_voltage[i], gLowThresHoldApply[i], gHighThresHoldApply[i]);
              }   
              sprintf(LogMsg, ">>ADC CHANEL%d = %.4f mA\r\n",i,g_voltage[i]);
              HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
            }   
          //Chanel 8 Error  
//          sprintf(LogMsg, ">>ADC CHANEL7 = None Active\r\n");
//          HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY); 
          
        }else{
          sprintf(LogMsg, ">>Analog None Active!\r\n");
          HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
        }

        //Show Data of digital chanel
        if (bDigitalEnable)
        {
          //Function Show digital

        }else{
          sprintf(LogMsg, ">>Digital None Active!\r\n");
          HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
        }
        
        //Show GPS location
        if (bGPSEnable)
        {
          //Function Show GPS

        }else{
          sprintf(LogMsg, ">>GPS None Active!\r\n");
          HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
        }
        
        //Show Temp and Hum
        if (bTempHumEnable)
        {
          //Function Show Tempeture and Humidity
          sprintf(LogMsg, ">>Tempeture = %.2f*C -- Humidity = %.2f % \r\n",gTemperature, gHumidity);
          HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);          

        }else{
          sprintf(LogMsg, ">>Tempeture and humidity None Active!\r\n");
          HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);
        }
        sprintf(LogMsg, ">>------------------------------------------------<<\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*)LogMsg, strlen(LogMsg), HAL_MAX_DELAY);  
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_4);  
    }
    if(gCounterSendEsp32==5){
        sprintf(uart_buf_esp32,
        "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%d,%.1f,%.1f,%d,%d,%d,%d,%d,%d\n",
        g_voltage[0], g_voltage[1], g_voltage[2], g_voltage[3], g_voltage[4], g_voltage[5], g_voltage[6], g_voltage[7],
        gDigital[0], gDigital[1], gDigital[2], gDigital[3],
        gTemperature, gHumidity,
        year+2000, month, date, hours, minutes, seconds);
        HAL_UART_Transmit(&huart3, (uint8_t*)uart_buf_esp32, strlen(uart_buf_esp32), HAL_MAX_DELAY);
        gCounterSendEsp32=0;
        //HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf_esp32, strlen(uart_buf_esp32), HAL_MAX_DELAY);
    }
    gCounterSendEsp32++;
    HAL_Delay(1000);

}


void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  uint16_t debounceCounter = 0;
  uint16_t eventProcessed = 0;
  char messPWR[40];
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_SET){
        stateUSB = USB_ON;
        //CDC_Transmit_FS((uint8_t*)"USB Connect!\r\n", 20);
    }
		
		
    /* USER CODE BEGIN EXTI15_10_IRQn 0 */
    // Power ON!
    while(systemState == SYSTEM_OFF && eventProcessed == 0)
    {
        while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
        {
            debounceCounter++;
            if(debounceCounter > 4)
            {
                //CDC_Transmit_FS((uint8_t*)"Power ON!\r\n", 11);
                sprintf(messPWR, ">>POWER ON!\r\n");
                HAL_UART_Transmit(&huart2, (uint8_t*)messPWR, strlen(messPWR), HAL_MAX_DELAY);
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
                systemState = SYSTEM_ON;
                uint8_t toggleLoop = 0;

                while(toggleLoop < LED_BLINK_COUNT)
                {
                    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_4);
                    delayCounter = 0;
                    while(delayCounter < SHORT_DELAY_ITR_MS)
                    {
                        delayCounter++;
                    }
                    toggleLoop++;
                }
            }
            delayCounter = 0;
            while(delayCounter < LONG_DELAY_ITR_MS)
            {
                delayCounter++;
            }
        }
        eventProcessed = 1;
    }
    
    // Power OFF!
    while(systemState == SYSTEM_ON && eventProcessed == 0)
    {
        while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
        {
            debounceCounter++;
            if(debounceCounter > 4)
            {
                //CDC_Transmit_FS((uint8_t*)"Power ON!\r\n", 11);
                //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
                sprintf(messPWR, ">>POWER OFF!\r\n");
                HAL_UART_Transmit(&huart2, (uint8_t*)messPWR, strlen(messPWR), HAL_MAX_DELAY);
                systemState = SYSTEM_ON;
                uint8_t toggleLoop = 0;

                while(toggleLoop < LED_BLINK_COUNT)
                {
                    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_4);
                    delayCounter = 0;
                    while(delayCounter < SHORT_DELAY_ITR_MS)
                    {
                        delayCounter++;
                    }
                    toggleLoop++;
                }
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
            }
            delayCounter = 0;
            while(delayCounter < LONG_DELAY_ITR_MS)
            {
                delayCounter++;
            }
        }
        eventProcessed = 1;
    }

  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}



//All function config system
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                          , GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PE7 PE8 PE9 PE10
                           PE0 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                          |GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD10 PD11 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PC6 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


