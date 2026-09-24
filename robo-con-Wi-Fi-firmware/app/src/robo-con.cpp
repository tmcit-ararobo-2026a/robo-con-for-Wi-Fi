/**
 * @file robo-con.cpp
 * @author your name (you@domain.com)
 * @brief RTOSないで動く関数など
 * @version 0.1
 * @date 2026-09-02
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "app/robo-con.hpp"

#include "app/printf.h"
#include "app/robo-con.h"
#include "app/robo-con_typedf.hpp"
//

#include "app/printf.h"
//

#include <cstdint>
//

#include "main.h"
//
#include "adc.h"
#include "dma.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
//

#include "cyhal_gpio.h"
#include "cyhal_system.h"
#include "stm32_cyhal_gpio_ex.h"
#include "stm32_cyhal_sdio_ex.h"
#include "wifi_bt_if.h"
//

__attribute__RAM_D1__ static uint16_t ADC1_Buffer[3] = {1,2,3};
// IN8  LiPo
// IN16 LY
// IN17 LX
__attribute__RAM_D1__ static uint16_t ADC2_Buffer[2];
// IN5 RY
// IN9 RX

__attribute__DTCM__ joystick_state_t joystick;
__attribute__DTCM__ LiPo_state_e LiPo_State;
__attribute__DTCM__ button_state_t button;
__attribute__DTCM__ lever_pin_state_t lever_pin;

__attribute__DTCM__ static bool LiPo_Toggle_LED = false;

__attribute__DTCM__ static TX_THREAD robo_con_Thread;

__attribute__ITCM__ static void robo_con_thread(ULONG thred_input);

#define ROBO_CON_APP_STACK_SIZE                  256
#define ROBO_CON_APP_THREAD_PRIO                 9
#define ROBO_CON_APP_THREAD_PREEMPTION_THRESHOLD ROBO_CON_APP_THREAD_PRIO
#define ROBO_CON_APP_THREAD_TIME_SLICE           TX_NO_TIME_SLICE
#define ROBO_CON_APP_THREAD_AUTO_START           TX_AUTO_START
#define ROBO_CON_APP_THREAD_DONT_START           TX_DONT_START

/***
 * rx_buffer_pool_memory
 * tx_buffer_pool_memory
 * これバカ頭おかしい
 */

UINT robo_con_init(VOID* memory_ptr)
{
    TX_BYTE_POOL* byte_pool = (TX_BYTE_POOL*)memory_ptr;

    CHAR* pointer;

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC1_Buffer, 3);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t*)ADC2_Buffer, 2);

    if (tx_byte_allocate(byte_pool, (VOID**)&pointer, ROBO_CON_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS) {
        printf("robo-con pool allocate error\r\n");
        return ROBO_CON_POOLERROR;
    }

    if (tx_thread_create(
            &robo_con_Thread,
            (CHAR*)"robo-con Thread",
            robo_con_thread,
            0,
            pointer,
            ROBO_CON_APP_STACK_SIZE,
            ROBO_CON_APP_THREAD_PRIO,
            ROBO_CON_APP_THREAD_PREEMPTION_THRESHOLD,
            ROBO_CON_APP_THREAD_TIME_SLICE,
            ROBO_CON_APP_THREAD_AUTO_START
        )) {
        printf("robo-con thread initializing error\r\n");
        return ROBO_CON_THREAD_ERROR;
    }

    mcu_wake_led();

    HAL_TIM_Base_Start_IT(&htim7);

    return ROBO_CON_SUCCSES;
}

static void robo_con_thread(ULONG thred_input)
{
    static uint8_t tick_count;

    while (1) {
        __enable_irq();

        if (tick_count >= 10) {
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            tick_count = 0;
        }
        tick_count++;

        update_joystick(joystick);
        update_LiPo_State(LiPo_State);
        update_button(button);
        get_lever_pin(lever_pin);

        printf("LIPO -> %d : ",ADC1_Buffer[0]);
        printf("LY -> %d : ",ADC1_Buffer[1]);
        printf("LX -> %d : ",ADC1_Buffer[2]);
        printf("RY -> %d : ",ADC2_Buffer[0]);
        printf("RX -> %d\r\n",ADC2_Buffer[1]);

        tx_thread_sleep(10);
    }
}

void update_joystick(joystick_state_t& hjoystick)
{
    hjoystick.LX     = (uint8_t)ADC1_Buffer[2];
    hjoystick.LY     = (uint8_t)ADC1_Buffer[1];
    hjoystick.L_Push = HAL_GPIO_ReadPin(STICK_PUSH_L_GPIO_Port, STICK_PUSH_L_Pin);
    hjoystick.RX     = (uint8_t)ADC2_Buffer[1];
    hjoystick.RY     = (uint8_t)ADC2_Buffer[0];
    hjoystick.R_Push = HAL_GPIO_ReadPin(STICK_PUSH_R_GPIO_Port, STICK_PUSH_R_Pin);

    return;
}

void update_LiPo_State(LiPo_state_e& hLiPo)
{
    static float voltage;
    voltage = (float)ADC1_Buffer[0] / 255.0f * 6.6f;
    if (voltage >= 4.0f) {
        hLiPo           = FULL;
        LiPo_Toggle_LED = false;
        HAL_GPIO_WritePin(LIPO_STATE_GPIO_Port, LIPO_STATE_Pin, GPIO_PIN_RESET);
    } else if (voltage <= 3.8f) {
        hLiPo = EMPTY;
        HAL_GPIO_WritePin(LIPO_STATE_GPIO_Port, LIPO_STATE_Pin, GPIO_PIN_SET);
        LiPo_Toggle_LED = false;
    } else {
        hLiPo           = LITTLE;
        LiPo_Toggle_LED = true;
    }

    return;
}

void update_button(button_state_t& hbutton)
{
    hbutton.L1 = HAL_GPIO_ReadPin(BUTTON_L1_GPIO_Port, BUTTON_L1_Pin);
    hbutton.L2 = HAL_GPIO_ReadPin(BUTTON_L2_GPIO_Port, BUTTON_L2_Pin);
    hbutton.L3 = HAL_GPIO_ReadPin(BUTTON_L3_GPIO_Port, BUTTON_L3_Pin);
    hbutton.L4 = HAL_GPIO_ReadPin(BUTTON_L4_GPIO_Port, BUTTON_L4_Pin);
    hbutton.R5 = HAL_GPIO_ReadPin(BUTTON_R5_GPIO_Port, BUTTON_R5_Pin);
    hbutton.R6 = HAL_GPIO_ReadPin(BUTTON_R6_GPIO_Port, BUTTON_R6_Pin);
    hbutton.R7 = HAL_GPIO_ReadPin(BUTTON_R7_GPIO_Port, BUTTON_R7_Pin);

    return;
}

void get_lever_pin(lever_pin_state_t& hlever_pin)
{
    hlever_pin.L0 = HAL_GPIO_ReadPin(LEVER_L0_GPIO_Port, LEVER_L0_Pin);
    hlever_pin.L1 = HAL_GPIO_ReadPin(LEVER_L1_GPIO_Port, LEVER_L1_Pin);
    hlever_pin.L2 = HAL_GPIO_ReadPin(LEVER_L2_GPIO_Port, LEVER_L2_Pin);
    hlever_pin.L3 = HAL_GPIO_ReadPin(LEVER_L3_GPIO_Port, LEVER_L3_Pin);
    hlever_pin.L4 = HAL_GPIO_ReadPin(LEVER_L4_GPIO_Port, LEVER_L4_Pin);
    hlever_pin.R0 = HAL_GPIO_ReadPin(LEVER_R0_GPIO_Port, LEVER_R0_Pin);
    hlever_pin.R1 = HAL_GPIO_ReadPin(LEVER_R1_GPIO_Port, LEVER_R1_Pin);
    hlever_pin.R2 = HAL_GPIO_ReadPin(LEVER_R2_GPIO_Port, LEVER_R2_Pin);
    hlever_pin.R3 = HAL_GPIO_ReadPin(LEVER_R3_GPIO_Port, LEVER_R3_Pin);
    hlever_pin.R4 = HAL_GPIO_ReadPin(LEVER_R4_GPIO_Port, LEVER_R4_Pin);

    return;
}

void update_lever(lever_state_t& hlever)
{
    static lever_pin_state_t lever_pin;
    get_lever_pin(lever_pin);

    if (lever_pin.L0) {
        hlever.Left = LeverPosition::PUSH;
    } else if (lever_pin.L1) {
        if (lever_pin.L2) {
            hlever.Left = LeverPosition::LEFT_DEEP;
        } else {
            hlever.Left = LeverPosition::LEFT;
        }
    } else if (lever_pin.L3) {
        if (lever_pin.L4) {
            hlever.Left = LeverPosition::RIGHT_DEEP;
        } else {
            hlever.Left = LeverPosition::RIGHT;
        }
    } else {
        hlever.Left = LeverPosition::FRONT;
    }

    if (lever_pin.R0) {
        hlever.Right = LeverPosition::PUSH;
    } else if (lever_pin.R1) {
        if (lever_pin.R2) {
            hlever.Right = LeverPosition::LEFT_DEEP;
        } else {
            hlever.Right = LeverPosition::LEFT;
        }
    } else if (lever_pin.R3) {
        if (lever_pin.R4) {
            hlever.Right = LeverPosition::RIGHT_DEEP;
        } else {
            hlever.Right = LeverPosition::RIGHT;
        }
    } else {
        hlever.Right = LeverPosition::FRONT;
    }

    return;
}

void mcu_wake_led()
{
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED_Wi_Fi_GPIO_Port, LED_Wi_Fi_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED_USB_GPIO_Port, LED_USB_Pin, GPIO_PIN_SET);
    HAL_Delay(100);

    HAL_Delay(100);
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED_Wi_Fi_GPIO_Port, LED_Wi_Fi_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED_USB_GPIO_Port, LED_USB_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);

    return;
}

extern "C" {
void tim_lib(TIM_HandleTypeDef* htim)
{
    // 10Hz
    if (htim == &htim7) {
        if (LiPo_Toggle_LED) {
            HAL_GPIO_TogglePin(LIPO_STATE_GPIO_Port, LIPO_STATE_Pin);
        }
        HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
    }

    return;
}
}
