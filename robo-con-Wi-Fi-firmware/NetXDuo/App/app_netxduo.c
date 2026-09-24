/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.c
  * @author  MCD Application Team
  * @brief   NetXDuo applicative file
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

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Includes ------------------------------------------------------------------*/
#include "app_netxduo.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cyhal.h"
#include "cybsp.h"
#include "cyabs_rtos.h"

#include "cy_wcm.h"
#include "cy_network_mw_core.h"
#include "whd_types.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#if  !defined(WIFI_SSID)
   #define WIFI_SSID                       "WIFI_SSID"
#endif /* (WIFI_SSID) */
#if  !defined(WIFI_PASSWORD)
   #define WIFI_PASSWORD                   "WIFI_PASSWORD"
#endif /* (WIFI_PASSWORD) */

#define MAX_WIFI_RETRY_COUNT     5

#define CY_RSLT_ERROR            ((cy_rslt_t)-1)

#define DEFAULT_PRIORITY         20
#define DEFAULT_MEMORY_SIZE      1024
#define ARP_MEMORY_SIZE          DEFAULT_MEMORY_SIZE

#define DEFAULT_PORT             6000
#define QUEUE_MAX_SIZE           512

/* LED blink timer period value */
#define LED_BLINK_PERIOD        (3000)


#define PRINT_IP_ADDRESS(addr)           do { \
                                              printf("STM32 %s: %lu.%lu.%lu.%lu \n", #addr, \
                                              (addr >> 24) & 0xff,                    \
                                              (addr >> 16) & 0xff,                    \
                                              (addr >> 8) & 0xff,                     \
                                              (addr & 0xff));                         \
                                         } while(0)

#define PRINT_DATA(addr, port, data)    do { \
                                              printf("[%lu.%lu.%lu.%lu:%u] -> '%s' \n", \
                                              (addr >> 24) & 0xff,                      \
                                              (addr >> 16) & 0xff,                      \
                                              (addr >> 8) & 0xff,                       \
                                              (addr & 0xff), port, data);               \
                                        } while(0)

/* The delay in milliseconds between successive scans.*/
#define SCAN_DELAY_MS                           (3000u)
typedef struct
{
    uint32_t result_count;
} wcm_scan_data_t;
wcm_scan_data_t         scan_data;
cy_wcm_mac_t            last_bssid;
#define PRINT_SCAN_TEMPLATE() \
    printf("\r\n--------------------------------------------------" \
           "--------------------------------------------------\r\n" \
           "  #                  SSID                  RSSI   Channel       " \
           "MAC Address              Security\r\n" \
           "--------------------------------------------------" \
           "--------------------------------------------------\r\n");

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

TX_THREAD AppMainThread;
UCHAR* pointer;

NX_UDP_SOCKET UDPSocket;


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

void application_start(ULONG id);
void wifi_scan(cy_wcm_scan_filter_t* scan_filter);
void scan_result_callback(cy_wcm_scan_result_t* result_ptr, void* user_data, cy_wcm_scan_status_t status);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
//static void print_ip4(uint32_t ip)
//{
//    unsigned char bytes[4];
//    bytes[0] = ip & 0xFF;
//    bytes[1] = (ip >> 8) & 0xFF;
//    bytes[2] = (ip >> 16) & 0xFF;
//    bytes[3] = (ip >> 24) & 0xFF;
//    printf("IPV4 addr = %d.%d.%d.%d\n", bytes[0], bytes[1], bytes[2], bytes[3]);
//}

void wifi_scan(cy_wcm_scan_filter_t* scan_filter)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    scan_data.result_count = 0;
    memset(last_bssid, 0, sizeof(cy_wcm_mac_t));

    PRINT_SCAN_TEMPLATE();

    result =  cy_wcm_start_scan(scan_result_callback, &scan_data, scan_filter);
    if (CY_RSLT_SUCCESS == result)
    {
    	tx_thread_suspend(&AppMainThread);
    }
    /* Add Delay before starting the scan again */
    cy_rtos_delay_milliseconds(SCAN_DELAY_MS);
}

bool scan_check_bssid_in_list(cy_wcm_scan_result_t* result_ptr)
{
    bool present = false;
    if (memcmp(result_ptr->BSSID, last_bssid, sizeof(cy_wcm_mac_t)) == 0)
    {
        /* Already existing BSSID, ignore the result */
        present = true;
        return present;
    }
    return present;
}

/* USER CODE END PFP */

/**
  * @brief  Application NetXDuo Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_NetXDuo_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END MX_NetXDuo_MEM_POOL */

  /* USER CODE BEGIN 0 */

  /* USER CODE END 0 */

  /* USER CODE BEGIN MX_NetXDuo_Init */
    /* Allocate the main thread pool. */
    ret = tx_byte_allocate(byte_pool, (VOID**)&pointer, 2 * DEFAULT_MEMORY_SIZE, TX_NO_WAIT);
    
    if (ret != TX_SUCCESS)
    {
        return NX_NOT_ENABLED;
    }

    /* Create the main thread */
    ret = tx_thread_create(&AppMainThread, "App Main thread", application_start, 0, pointer,
                           2 * DEFAULT_MEMORY_SIZE,
                           DEFAULT_PRIORITY, DEFAULT_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

    if (ret != TX_SUCCESS)
    {
        return NX_NOT_ENABLED;
    }

  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/* USER CODE BEGIN 2 */
void application_start(ULONG id)
{
    //cy_rslt_t result = CY_RSLT_SUCCESS;
    //cy_wcm_config_t config;

    //config.interface = CY_WCM_INTERFACE_TYPE_STA;
    //cy_wcm_scan_filter_t scan_filter;
    //result = cy_wcm_init(&config);

    //if (result != CY_RSLT_SUCCESS)
    //{
    //    printf("Error initializing WCM\n");
    //    return;
    //}
    //scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_SSID;
    //memcpy(scan_filter.param.SSID, WIFI_SSID, strlen(WIFI_SSID) + 1);

    //wifi_scan(&scan_filter);
}

const char* security_to_str(whd_security_t security)
{
    switch (security)
    {
        case WHD_SECURITY_OPEN:
            return "OPEN";

        case WHD_SECURITY_WEP_PSK:
            return "WEP_PSK";

        case WHD_SECURITY_WEP_SHARED:
            return "WEP_SHARED";

        case WHD_SECURITY_WPA_TKIP_PSK:
            return "WPA_TKIP_PSK";

        case WHD_SECURITY_WPA_AES_PSK:
            return "WPA_AES_PSK";

        case WHD_SECURITY_WPA_MIXED_PSK:
            return "WPA_MIXED_PSK";

        case WHD_SECURITY_WPA2_AES_PSK:
            return "WPA2_AES_PSK";

        case WHD_SECURITY_WPA2_TKIP_PSK:
            return "WPA2_TKIP_PSK";

        case WHD_SECURITY_WPA2_MIXED_PSK:
            return "WPA2_MIXED_PSK";

        case WHD_SECURITY_WPA2_FBT_PSK:
            return "WPA2_FBT_PSK";

        case WHD_SECURITY_WPA3_SAE:
            return "WPA3_SAE";

        case WHD_SECURITY_WPA3_WPA2_PSK:
            return "WPA3_WPA2_PSK";

        case WHD_SECURITY_WPA_TKIP_ENT:
            return "WPA_TKIP_ENTERPRISE";

        case WHD_SECURITY_WPA_AES_ENT:
            return "WPA_AES_ENTERPRISE";

        case WHD_SECURITY_WPA_MIXED_ENT:
            return "WPA_MIXED_ENTERPRISE";

        case WHD_SECURITY_WPA2_TKIP_ENT:
            return "WPA2_TKIP_ENTERPRISE";

        case WHD_SECURITY_WPA2_AES_ENT:
            return "WPA2_AES_ENTERPRISE";

        case WHD_SECURITY_WPA2_MIXED_ENT:
            return "WPA2_MIXED_ENTERPRISE";

        case WHD_SECURITY_WPA2_FBT_ENT:
            return "WPA2_FBT_ENTERPRISE";

        case WHD_SECURITY_IBSS_OPEN:
            return "IBSS_OPEN";

        case WHD_SECURITY_WPS_SECURE:
            return "WPS_SECURE";

        case WHD_SECURITY_FORCE_32_BIT:
        case WHD_SECURITY_UNKNOWN:
        default:
            return "UNKNOWN_SECURITY";
    }
}

void scan_result_callback(cy_wcm_scan_result_t* result_ptr, void* user_data,
                          cy_wcm_scan_status_t status)
{
    wcm_scan_data_t* scan_data = (wcm_scan_data_t*)user_data;

    if (scan_data != NULL)
    {
        if (status == CY_WCM_SCAN_COMPLETE)
        {
            //xTaskNotify(WiFi_TaskHandle, 0, eNoAction);
        	tx_thread_resume(&AppMainThread);
        }
        else
        {
            if ((result_ptr != NULL) && (status == CY_WCM_SCAN_INCOMPLETE))
            {
                if ((strlen((const char*)result_ptr->SSID) != 0) &&
                    (scan_check_bssid_in_list(result_ptr) == false))
                {
                    scan_data->result_count++;
                    /* Copy BSSID to last bssid which will be used in scan_check_bssid_in_list to
                       avoid
                     * repeating BSSID in scan results
                     */
                    memcpy(&last_bssid, &result_ptr->BSSID[0], sizeof(last_bssid));
                    printf(" %2ld   %-32s     %4d     %3d      %02X:%02X:%02X:%02X:%02X:%02X"
                           "         %-15s\r\n",
                           scan_data->result_count, result_ptr->SSID,
                           result_ptr->signal_strength, result_ptr->channel,
                           result_ptr->BSSID[0], result_ptr->BSSID[1],
                           result_ptr->BSSID[2], result_ptr->BSSID[3],
                           result_ptr->BSSID[4], result_ptr->BSSID[5],
                           security_to_str((whd_security_t)result_ptr->security));
                }
            }
        }
    }
}

/* USER CODE END 2 */
