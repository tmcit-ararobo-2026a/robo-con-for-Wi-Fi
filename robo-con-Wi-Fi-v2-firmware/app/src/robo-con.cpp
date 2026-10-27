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
#include "tim.h"
#include "usart.h"
//

#include "cyhal_gpio.h"
#include "cyhal_system.h"
#include "stm32_cyhal_gpio_ex.h"
#include "stm32_cyhal_sdio_ex.h"
#include "wifi_bt_if.h"
//

__attribute__RAM_D2__BSS__ static uint16_t ADC1_Buffer[3];
// IN3  LiPo
// IN18 RY
// IN19 RX
__attribute__RAM_D2__BSS__ static uint16_t ADC2_Buffer[2];
// IN10 LY
// IN11 LX

__attribute__DTCM__ joystick_state_t joystick;
__attribute__DTCM__ LiPo_state_e LiPo_State;
__attribute__DTCM__ button_state_t button;
__attribute__DTCM__ up_senter_side_button_t button_f;

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
 * wifi_sta_dns_handle
 */

UINT robo_con_init(VOID* memory_ptr)
{
    TX_BYTE_POOL* byte_pool = (TX_BYTE_POOL*)memory_ptr;

    CHAR* pointer;

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC1_Buffer, 3);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t*)ADC2_Buffer, 2);

    if (tx_byte_allocate(byte_pool, (VOID**)&pointer, ROBO_CON_APP_STACK_SIZE, TX_NO_WAIT) !=
        TX_SUCCESS) {
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

    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_Base_Start_IT(&htim7);

    return ROBO_CON_SUCCSES;
}

static void robo_con_thread(ULONG thred_input)
{
    static uint8_t tick_count;

    while (1) {
        __enable_irq();

        if (tick_count >= 100) {
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            tick_count = 0;
        }
        tick_count++;

        update_joystick(joystick);
        update_LiPo_State(LiPo_State);
        update_button(button);
        update_button_f(button_f);

        printf("LIPO -> %d : ", ADC1_Buffer[0]);
        printf("LY -> %d : ", ADC1_Buffer[1]);
        printf("LX -> %d : ", ADC1_Buffer[2]);
        printf("RY -> %d : ", ADC2_Buffer[0]);
        printf("RX -> %d\r\n", ADC2_Buffer[1]);

        tx_thread_sleep(1);
    }
}

void update_joystick(joystick_state_t& hjoystick)
{
    hjoystick.LX     = (uint8_t)ADC1_Buffer[2];
    hjoystick.LY     = (uint8_t)ADC1_Buffer[1];
    hjoystick.L_Push = HAL_GPIO_ReadPin(LP_STICK_GPIO_Port, LP_STICK_Pin);
    hjoystick.RX     = (uint8_t)ADC2_Buffer[1];
    hjoystick.RY     = (uint8_t)ADC2_Buffer[0];
    hjoystick.R_Push = HAL_GPIO_ReadPin(RP_STICK_GPIO_Port, RP_STICK_Pin);
}

void update_LiPo_State(LiPo_state_e& hLiPo)
{
    static float voltage;
    voltage = (float)ADC1_Buffer[0] / 255.0f * 6.6f;
    if (voltage >= 4.0f) {
        hLiPo           = FULL;
        LiPo_Toggle_LED = false;
        HAL_GPIO_WritePin(LEDLiPo_GPIO_Port, LEDLiPo_Pin, GPIO_PIN_RESET);
    } else if (voltage <= 3.8f) {
        hLiPo = EMPTY;
        HAL_GPIO_WritePin(LEDLiPo_GPIO_Port, LEDLiPo_Pin, GPIO_PIN_SET);
        LiPo_Toggle_LED = false;
    } else {
        hLiPo           = LITTLE;
        LiPo_Toggle_LED = true;
    }
}

void update_button(button_state_t& hbutton)
{
    hbutton.R1 = HAL_GPIO_ReadPin(R1_BUTTON_GPIO_Port, R1_BUTTON_Pin);
    hbutton.R2 = HAL_GPIO_ReadPin(R2_BUTTON_GPIO_Port, R2_BUTTON_Pin);
    hbutton.R3 = HAL_GPIO_ReadPin(R3_BUTTON_GPIO_Port, R3_BUTTON_Pin);
    hbutton.R4 = HAL_GPIO_ReadPin(R4_BUTTON_GPIO_Port, R4_BUTTON_Pin);
    hbutton.L5 = HAL_GPIO_ReadPin(L5_BUTTON_GPIO_Port, L5_BUTTON_Pin);
    hbutton.L6 = HAL_GPIO_ReadPin(L6_BUTTON_GPIO_Port, L6_BUTTON_Pin);
    hbutton.L7 = HAL_GPIO_ReadPin(L7_BUTTON_GPIO_Port, L7_BUTTON_Pin);
    hbutton.L8 = HAL_GPIO_ReadPin(L8_BUTTON_GPIO_Port, L8_BUTTON_Pin);
}

void update_button_f(up_senter_side_button_t& hbutton_f)
{
    hbutton_f.RU = HAL_GPIO_ReadPin(R_PUSH_GPIO_Port, R_PUSH_Pin);
    hbutton_f.RS = HAL_GPIO_ReadPin(R_PUSH_HOME_GPIO_Port, R_PUSH_HOME_Pin);
    hbutton_f.LU = HAL_GPIO_ReadPin(L_PUSH_GPIO_Port, L_PUSH_Pin);
    hbutton_f.LS = HAL_GPIO_ReadPin(L_PUSH_HOME_GPIO_Port, L_PUSH_HOME_Pin);
}

void mcu_wake_led()
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LEDWL_GPIO_Port, LEDWL_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LEDUSB_GPIO_Port, LEDUSB_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);

    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LEDWL_GPIO_Port, LEDWL_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LEDUSB_GPIO_Port, LEDUSB_Pin, GPIO_PIN_SET);

}

extern "C" {
void tim_lib(TIM_HandleTypeDef* htim)
{
    // 10Hz
    if (htim == &htim6) {
        if (LiPo_Toggle_LED) {
        }
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    }

    // 100Hz
    if (htim == &htim7) {
        __enable_irq();
    }
}
}
