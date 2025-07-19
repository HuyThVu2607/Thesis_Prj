/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "process.h"
#include "fatfs.h"

int main(void)
{
    Process_Init();
    UI_Process_Init();
    while (1)
    {
    /* USER CODE END WHILE */
        Process_Run();
        UI_Process_Run();
       
    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}