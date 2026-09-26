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
// #define USE_DTCM_SECTION
#define USE_ITCM_SECTION

#ifdef USE_SECTION
#ifdef USE_DTCM_SECTION
#define __attribute__DTCM__ __attribute__((__section__(".data")))
#else
#define __attribute__DTCM__
#endif
#define __attribute__RAM_D1__ __attribute__((__section__(".ram_d1_data")))
#define __attribute__RAM_D2__ __attribute__((__section__(".ram_d2_data")))
#define __attribute__RAM_D3__ __attribute__((__section__(".ram_d3_data")))
#define __attribute__FLASH__  __attribute__((__section__(".text_flash")))
#ifdef USE_ITCM_SECTION
#define __attribute__ITCM__ __attribute__((__section__(".itcm_text")))
#else

#define __attribute__ITCM__
#endif
#define __attribute__RAM_BackUp__ __attribute__((__section__(".ram_BackUp_data")))
#else
#define __attribute__DTCM__
#define __attribute__RAM_D1__
#define __attribute__RAM_D2__
#define __attribute__RAM_D3__
#define __attribute__FLASH__
#define __attribute__ITCM__
#define __attribute__RAM_BackUp__
#endif

__attribute__ITCM__ UINT robo_con_init(VOID* memory_ptr);
__attribute__ITCM__ void tim_lib(TIM_HandleTypeDef* htim);
__attribute__ITCM__ void mcu_wake_led();

#define ROBO_CON_SUCCSES      0U
#define ROBO_CON_POOLERROR    1U
#define ROBO_CON_THREAD_ERROR 2U

#ifdef __cplusplus
}
#endif

#endif
