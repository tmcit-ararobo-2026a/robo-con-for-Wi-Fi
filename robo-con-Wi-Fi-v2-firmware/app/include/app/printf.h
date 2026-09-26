
#ifndef PRINTF_H
#define PRINTF_H

#include <stdbool.h>
#include <stdint-gcc.h>
#include <stdio.h>

#include "app/robo-con.h"
//
#include "app_azure_rtos_config.h"
#include "tx_api.h"

__attribute__ITCM__ UINT cdc_init(VOID* memory_ptr);

__attribute__ITCM__ int _write(int file, char* ptr, int len);

#define CDC_SUCCSES       0
#define CDC_POOLERROR     1
#define CDC_THREAD_ERROR  2
#define CDC_UX_INIT_ERROR 3

#endif
