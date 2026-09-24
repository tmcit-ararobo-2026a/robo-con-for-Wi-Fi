/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LEVER_L1_Pin GPIO_PIN_2
#define LEVER_L1_GPIO_Port GPIOE
#define LEVER_L2_Pin GPIO_PIN_3
#define LEVER_L2_GPIO_Port GPIOE
#define LEVER_L0_Pin GPIO_PIN_4
#define LEVER_L0_GPIO_Port GPIOE
#define LEVER_L4_Pin GPIO_PIN_5
#define LEVER_L4_GPIO_Port GPIOE
#define LEVER_L3_Pin GPIO_PIN_6
#define LEVER_L3_GPIO_Port GPIOE
#define STICK_Y_L_Pin GPIO_PIN_0
#define STICK_Y_L_GPIO_Port GPIOA
#define STICK_X_L_Pin GPIO_PIN_1
#define STICK_X_L_GPIO_Port GPIOA
#define LED_USB_Pin GPIO_PIN_4
#define LED_USB_GPIO_Port GPIOA
#define LED_Wi_Fi_Pin GPIO_PIN_5
#define LED_Wi_Fi_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_6
#define LED4_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_7
#define LED3_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOC
#define LIPO_OBSERVER_Pin GPIO_PIN_5
#define LIPO_OBSERVER_GPIO_Port GPIOC
#define STICK_X_R_Pin GPIO_PIN_0
#define STICK_X_R_GPIO_Port GPIOB
#define STICK_Y_R_Pin GPIO_PIN_1
#define STICK_Y_R_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOB
#define LED0_Pin GPIO_PIN_7
#define LED0_GPIO_Port GPIOE
#define STICK_PUSH_R_Pin GPIO_PIN_8
#define STICK_PUSH_R_GPIO_Port GPIOE
#define BUTTON_R5_Pin GPIO_PIN_13
#define BUTTON_R5_GPIO_Port GPIOE
#define BUTTON_R6_Pin GPIO_PIN_14
#define BUTTON_R6_GPIO_Port GPIOE
#define BUTTON_R7_Pin GPIO_PIN_15
#define BUTTON_R7_GPIO_Port GPIOE
#define STICK_PUSH_L_Pin GPIO_PIN_10
#define STICK_PUSH_L_GPIO_Port GPIOB
#define LIPO_STATE_Pin GPIO_PIN_11
#define LIPO_STATE_GPIO_Port GPIOB
#define BUTTON_L1_Pin GPIO_PIN_12
#define BUTTON_L1_GPIO_Port GPIOB
#define BUTTON_L2_Pin GPIO_PIN_13
#define BUTTON_L2_GPIO_Port GPIOB
#define BUTTON_L3_Pin GPIO_PIN_14
#define BUTTON_L3_GPIO_Port GPIOB
#define BUTTON_L4_Pin GPIO_PIN_15
#define BUTTON_L4_GPIO_Port GPIOB
#define LEVER_R4_Pin GPIO_PIN_13
#define LEVER_R4_GPIO_Port GPIOD
#define LEVER_R3_Pin GPIO_PIN_14
#define LEVER_R3_GPIO_Port GPIOD
#define LEVER_R0_Pin GPIO_PIN_15
#define LEVER_R0_GPIO_Port GPIOD
#define LEVER_R2_Pin GPIO_PIN_6
#define LEVER_R2_GPIO_Port GPIOC
#define LEVER_R1_Pin GPIO_PIN_9
#define LEVER_R1_GPIO_Port GPIOA
#define BT_ON_Pin GPIO_PIN_6
#define BT_ON_GPIO_Port GPIOB
#define WL_ON_Pin GPIO_PIN_7
#define WL_ON_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
