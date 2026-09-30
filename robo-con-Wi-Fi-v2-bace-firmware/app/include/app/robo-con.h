/**
 * @file robo-con.h
 * @author your name (you@domain.com)
 * @brief セクションと初期化関数
 * @version 0.1
 * @date 2026-09-02
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef ROBO_CON_H
#define ROBO_CON_H

#include "main.h"
#include "tx_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* defined */
#define USE_SECTION

#define __at_section(section) __attribute__((__section__(section)))

#ifdef USE_SECTION
#define __attribute__DTCM__         __at_section(".data")
#define __attribute__DTCM__BSS__    __at_section(".tbss")
#define __attribute__RAM_D1__       __at_section(".data_ram1")
#define __attribute__RAM_D1__BSS__  __at_section(".tbss_ram1")
#define __attribute__RAM_D2__       __at_section(".data_ram2")
#define __attribute__RAM_D2__BSS__  __at_section(".tbss_ram2")
#define __attribute__RAM_D3__       __at_section(".data_ram3")
#define __attribute__RAM_D3__BSS__  __at_section(".tbss_ram3")
#define __attribute__FLASH__        __at_section(".flash")
#define __attribute__ITCM__         __at_section(".itcm")
#define __attribute__RAM_BackUp__   __at_section(".data_backup")
#else
#define __attribute__DTCM__
#define __attribute__RAM_D1__
#define __attribute__RAM_D2__
#define __attribute__RAM_D3__
#define __attribute__FLASH__
#define __attribute__ITCM__
#define __attribute__RAM_BackUp__
#endif

__attribute__ITCM__ void tim_lib(TIM_HandleTypeDef* htim);
__attribute__FLASH__ UINT robo_con_init(VOID* memory_ptr);
__attribute__FLASH__ void mcu_wake_led();

#define ROBO_CON_SUCCSES      0U
#define ROBO_CON_POOLERROR    1U
#define ROBO_CON_THREAD_ERROR 2U

#ifdef __cplusplus
}
#endif

#endif
