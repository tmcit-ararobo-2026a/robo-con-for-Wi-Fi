
#include "app/printf.h"
//
#include "main.h"
//
#include "tx_api.h"
#include "tx_port.h"
//
#include "usbd_cdc.h"
//
#include "app/robo-con.h"

extern USBD_HandleTypeDef hUsbDeviceHS;

__attribute__DTCM__ bool CDC_Tramsmit_request = false;

__attribute__DTCM__ static uint8_t TX_Buffer0[4096] = {0};
__attribute__DTCM__ static uint8_t TX_Buffer1[4096] = {0};
__attribute__DTCM__ static uint8_t RX_Buffer[4096]  = {0};
__attribute__DTCM__ static uint16_t Buffer_len      = {0};
__attribute__DTCM__ static uint16_t Buffer_len_     = {0};
__attribute__DTCM__ static bool Transmission_Buffer = false;

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

    USBD_CDC_SetRxBuffer(&hUsbDeviceHS, RX_Buffer);

    HAL_GPIO_WritePin(LED_USB_GPIO_Port, LED_USB_Pin, GPIO_PIN_SET);
    HAL_Delay(3000);
    HAL_GPIO_WritePin(LED_USB_GPIO_Port, LED_USB_Pin, GPIO_PIN_RESET);

    return CDC_SUCCSES;
}

static void CDC_thread(ULONG thred_input)
{
    while (1) {
        __enable_irq();

        HAL_GPIO_TogglePin(LED_USB_GPIO_Port, LED_USB_Pin);

        if (CDC_Tramsmit_request) {
            Transmission_Buffer = !Transmission_Buffer;
            Buffer_len_         = Buffer_len;
            Buffer_len          = 0;

            if (!Transmission_Buffer) {
                USBD_CDC_SetTxBuffer(&hUsbDeviceHS, TX_Buffer1, Buffer_len_);
            } else {
                USBD_CDC_SetTxBuffer(&hUsbDeviceHS, TX_Buffer0, Buffer_len_);
            }

            USBD_CDC_TransmitPacket(&hUsbDeviceHS);

            CDC_Tramsmit_request = false;
        }

        tx_thread_sleep(100);
    };
}

int _write(int file, char* ptr, int len)
{
    if (Transmission_Buffer) {
        for (uint16_t i = 0; i < len; i++) {
            TX_Buffer1[Buffer_len + i] = ptr[i];
        }
    } else {
        for (uint16_t i = 0; i < len; i++) {
            TX_Buffer0[Buffer_len + i] = ptr[i];
        }
    }
    Buffer_len += len;
    CDC_Tramsmit_request = true;

    return len;
}
