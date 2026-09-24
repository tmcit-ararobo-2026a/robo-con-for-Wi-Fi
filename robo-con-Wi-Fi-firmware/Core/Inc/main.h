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
#define L0_LEVER_Pin GPIO_PIN_2
#define L0_LEVER_GPIO_Port GPIOE
#define L2_LEVER_Pin GPIO_PIN_3
#define L2_LEVER_GPIO_Port GPIOE
#define L1_LEVER_Pin GPIO_PIN_4
#define L1_LEVER_GPIO_Port GPIOE
#define L3_LEVER_Pin GPIO_PIN_5
#define L3_LEVER_GPIO_Port GPIOE
#define L4_LEVER_Pin GPIO_PIN_6
#define L4_LEVER_GPIO_Port GPIOE
#define L5_BUTTON_Pin GPIO_PIN_13
#define L5_BUTTON_GPIO_Port GPIOC
#define L6_BUTTON_Pin GPIO_PIN_14
#define L6_BUTTON_GPIO_Port GPIOC
#define L7_BUTTON_Pin GPIO_PIN_15
#define L7_BUTTON_GPIO_Port GPIOC
#define L8_BUTTON_Pin GPIO_PIN_0
#define L8_BUTTON_GPIO_Port GPIOC
#define LY_STICK_Pin GPIO_PIN_4
#define LY_STICK_GPIO_Port GPIOA
#define LX_STICK_Pin GPIO_PIN_5
#define LX_STICK_GPIO_Port GPIOA
#define LP_STICK_Pin GPIO_PIN_6
#define LP_STICK_GPIO_Port GPIOA
#define LiPo_OBSERVE_Pin GPIO_PIN_5
#define LiPo_OBSERVE_GPIO_Port GPIOC
#define RX_STICK_Pin GPIO_PIN_0
#define RX_STICK_GPIO_Port GPIOB
#define RY_STICK_Pin GPIO_PIN_1
#define RY_STICK_GPIO_Port GPIOB
#define RP_STICK_Pin GPIO_PIN_11
#define RP_STICK_GPIO_Port GPIOB
#define R1_BUTTON_Pin GPIO_PIN_12
#define R1_BUTTON_GPIO_Port GPIOB
#define R2_BUTTON_Pin GPIO_PIN_13
#define R2_BUTTON_GPIO_Port GPIOB
#define R3_BUTTON_Pin GPIO_PIN_14
#define R3_BUTTON_GPIO_Port GPIOB
#define R4_BUTTON_Pin GPIO_PIN_15
#define R4_BUTTON_GPIO_Port GPIOB
#define R4_LEVER_Pin GPIO_PIN_10
#define R4_LEVER_GPIO_Port GPIOD
#define R3_LEVER_Pin GPIO_PIN_13
#define R3_LEVER_GPIO_Port GPIOD
#define R0_LEVER_Pin GPIO_PIN_14
#define R0_LEVER_GPIO_Port GPIOD
#define R2_LEVER_Pin GPIO_PIN_15
#define R2_LEVER_GPIO_Port GPIOD
#define R1_LEVER_Pin GPIO_PIN_6
#define R1_LEVER_GPIO_Port GPIOC
#define WL_ON_Pin GPIO_PIN_5
#define WL_ON_GPIO_Port GPIOD
#define BT_ON_Pin GPIO_PIN_6
#define BT_ON_GPIO_Port GPIOD
#define LED0_Pin GPIO_PIN_3
#define LED0_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_5
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_6
#define LED3_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_7
#define LED4_GPIO_Port GPIOB
#define LEDWL_Pin GPIO_PIN_8
#define LEDWL_GPIO_Port GPIOB
#define LEDUSB_Pin GPIO_PIN_9
#define LEDUSB_GPIO_Port GPIOB
#define LEDLiPo_GOOD_Pin GPIO_PIN_0
#define LEDLiPo_GOOD_GPIO_Port GPIOE
#define LEDLiPo_BAD_Pin GPIO_PIN_1
#define LEDLiPo_BAD_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
