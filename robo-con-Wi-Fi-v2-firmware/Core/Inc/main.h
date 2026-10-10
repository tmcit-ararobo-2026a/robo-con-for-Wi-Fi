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
#define LP_STICK_Pin GPIO_PIN_13
#define LP_STICK_GPIO_Port GPIOC
#define LEDWL_Pin GPIO_PIN_14
#define LEDWL_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOC
#define LEDWLRX_Pin GPIO_PIN_2
#define LEDWLRX_GPIO_Port GPIOC
#define LEDWLTX_Pin GPIO_PIN_3
#define LEDWLTX_GPIO_Port GPIOC
#define R1_BUTTON_Pin GPIO_PIN_7
#define R1_BUTTON_GPIO_Port GPIOA
#define R2_BUTTON_Pin GPIO_PIN_4
#define R2_BUTTON_GPIO_Port GPIOC
#define R3_BUTTON_Pin GPIO_PIN_5
#define R3_BUTTON_GPIO_Port GPIOC
#define R4_BUTTON_Pin GPIO_PIN_0
#define R4_BUTTON_GPIO_Port GPIOB
#define RP_STICK_Pin GPIO_PIN_1
#define RP_STICK_GPIO_Port GPIOB
#define R_PUSH_Pin GPIO_PIN_2
#define R_PUSH_GPIO_Port GPIOB
#define R_PUSH_HOME_Pin GPIO_PIN_10
#define R_PUSH_HOME_GPIO_Port GPIOB
#define LEDLiPo_Pin GPIO_PIN_12
#define LEDLiPo_GPIO_Port GPIOB
#define BT_ON_Pin GPIO_PIN_15
#define BT_ON_GPIO_Port GPIOB
#define WL_ON_Pin GPIO_PIN_6
#define WL_ON_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_8
#define LED0_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOA
#define LEDUSB_Pin GPIO_PIN_3
#define LEDUSB_GPIO_Port GPIOB
#define L8_BUTTON_Pin GPIO_PIN_4
#define L8_BUTTON_GPIO_Port GPIOB
#define L7_BUTTON_Pin GPIO_PIN_5
#define L7_BUTTON_GPIO_Port GPIOB
#define L6_BUTTON_Pin GPIO_PIN_6
#define L6_BUTTON_GPIO_Port GPIOB
#define L5_BUTTON_Pin GPIO_PIN_7
#define L5_BUTTON_GPIO_Port GPIOB
#define L_PUSH_HOME_Pin GPIO_PIN_8
#define L_PUSH_HOME_GPIO_Port GPIOB
#define L_PUSH_Pin GPIO_PIN_9
#define L_PUSH_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
