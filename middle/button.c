#include "button.h"


extern SystemState systemState;
extern StateUSB stateUSB;
//volatile uint32_t delayCounter	=	0;

//void EXTI15_10_IRQHandler(void)
//{
//  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
//
//  /* USER CODE END EXTI15_10_IRQn 0 */
//  uint16_t debounceCounter = 0;
//  uint16_t eventProcessed = 0;
//    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
//    
//    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_SET){
//        stateUSB = USB_ON;
//        //CDC_Transmit_FS((uint8_t*)"USB Connect!\r\n", 20);
//    }
//		
//		
//    /* USER CODE BEGIN EXTI15_10_IRQn 0 */
//    // Power ON!
//    while(systemState == SYSTEM_OFF && eventProcessed == 0)
//    {
//        while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
//        {
//            debounceCounter++;
//            if(debounceCounter > 4)
//            {
//                //CDC_Transmit_FS((uint8_t*)"Power ON!\r\n", 11);
//                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
//                systemState = SYSTEM_ON;
//                uint8_t toggleLoop = 0;
//
//                while(toggleLoop < LED_BLINK_COUNT)
//                {
//                    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_4);
//                    delayCounter = 0;
//                    while(delayCounter < SHORT_DELAY_ITR_MS)
//                    {
//                        delayCounter++;
//                    }
//                    toggleLoop++;
//                }
//            }
//            delayCounter = 0;
//            while(delayCounter < LONG_DELAY_ITR_MS)
//            {
//                delayCounter++;
//            }
//        }
//        eventProcessed = 1;
//    }
//    
//    // Power OFF!
//    while(systemState == SYSTEM_ON && eventProcessed == 0)
//    {
//        while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
//        {
//            debounceCounter++;
//            if(debounceCounter > 4)
//            {
//                //CDC_Transmit_FS((uint8_t*)"Power ON!\r\n", 11);
//                //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
//                systemState = SYSTEM_ON;
//                uint8_t toggleLoop = 0;
//
//                while(toggleLoop < LED_BLINK_COUNT)
//                {
//                    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_4);
//                    delayCounter = 0;
//                    while(delayCounter < SHORT_DELAY_ITR_MS)
//                    {
//                        delayCounter++;
//                    }
//                    toggleLoop++;
//                }
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
//            }
//            delayCounter = 0;
//            while(delayCounter < LONG_DELAY_ITR_MS)
//            {
//                delayCounter++;
//            }
//        }
//        eventProcessed = 1;
//    }
//
//  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
//  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
//  /* USER CODE BEGIN EXTI15_10_IRQn 1 */
//
//  /* USER CODE END EXTI15_10_IRQn 1 */
//}