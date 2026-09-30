
#include "app/printf.h"
//
#include "main.h"
#include "usart.h"
//
#include "tx_api.h"
//
#include "app/robo-con.h"


#define USB_TX_BUFFER_SIZE 8192U
__attribute__DTCM__ static uint8_t TX_Buffer[USB_TX_BUFFER_SIZE] = {0};
__attribute__DTCM__ static uint8_t RX_Buffer[2048]  = {0};
__attribute__DTCM__ static uint16_t Buffer_len      = {0};
__attribute__DTCM__ bool CDC_Tramsmit_request = false;

__attribute__DTCM__ static TX_THREAD CDC_Thread;

__attribute__ITCM__ static void CDC_thread(ULONG thred_input);

#define CDC_APP_STACK_SIZE                  512
#define CDC_APP_THREAD_PRIO                 10
#define CDC_APP_THREAD_PREEMPTION_THRESHOLD CDC_APP_THREAD_PRIO
#define CDC_APP_THREAD_TIME_SLICE           TX_NO_TIME_SLICE
#define CDC_APP_THREAD_AUTO_START           TX_AUTO_START
#define CDC_APP_THREAD_DONT_START           TX_DONT_START

UINT cdc_init(VOID* memory_ptr)
{
    TX_BYTE_POOL* byte_pool = (TX_BYTE_POOL*)memory_ptr;

    CHAR* pointer;

    if (tx_byte_allocate(byte_pool, (VOID**)&pointer, CDC_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS) {
        printf("cdc pool allocate error\r\n");
        return CDC_POOLERROR;
    }

    if (tx_thread_create(
            &CDC_Thread,
            "CDC Thread",
            CDC_thread,
            0,
            pointer,
            CDC_APP_STACK_SIZE,
            CDC_APP_THREAD_PRIO,
            CDC_APP_THREAD_PREEMPTION_THRESHOLD,
            CDC_APP_THREAD_TIME_SLICE,
            CDC_APP_THREAD_AUTO_START
        )) {
        printf("cdc thread initializing error\r\n");
        return CDC_THREAD_ERROR;
    }

    HAL_GPIO_WritePin(LEDUSB_GPIO_Port, LEDUSB_Pin, GPIO_PIN_SET);
    HAL_Delay(3000);
    HAL_GPIO_WritePin(LEDUSB_GPIO_Port, LEDUSB_Pin, GPIO_PIN_RESET);

    return CDC_SUCCSES;
}

static void CDC_thread(ULONG thred_input)
{
    while (1) {
        __enable_irq();

        HAL_GPIO_TogglePin(LEDUSB_GPIO_Port, LEDUSB_Pin);

        if (CDC_Tramsmit_request) {

            CDC_Tramsmit_request = false;
        }

        tx_thread_sleep(100);
    };
}

int _write(int file, char* ptr, int len)
{
    uint16_t llen;
    if((Buffer_len + len) > USB_TX_BUFFER_SIZE){}

    for(uint16_t i = 0;i < len;i++){
        TX_Buffer[len + Buffer_len] = ptr[len];
    }

    Buffer_len += len;
    CDC_Tramsmit_request = true;

    return len;
}
