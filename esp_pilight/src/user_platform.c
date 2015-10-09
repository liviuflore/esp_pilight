/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/12/1, v1.0 create this file.
*******************************************************************************/
#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "user_config.h"
#include "user_platform.h"

#include "debug.h"


struct platform_saved_param platform_param;

LOCAL uint8 platform_version[20] = { 0 };



/******************************************************************************
* FunctionName : platform_print_reset_reason
* Description  : displays last reset reson
* Parameters   : none
* Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR platform_print_reset_reason(void)
{
    struct rst_info *rst_info = system_get_rst_info();

    ESP_ALW("reset reason: %x\n", rst_info->reason);

    if (rst_info->reason == REASON_WDT_RST ||
        rst_info->reason == REASON_EXCEPTION_RST ||
        rst_info->reason == REASON_SOFT_WDT_RST) {
        if (rst_info->reason == REASON_EXCEPTION_RST) {
            ESP_ALW("Fatal exception (%d):\n", rst_info->exccause);
        }
        ESP_ALW("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x, excvaddr=0x%08x, depc=0x%08x\n",
                rst_info->epc1, rst_info->epc2, rst_info->epc3, rst_info->excvaddr, rst_info->depc);
    }
}

/******************************************************************************
* FunctionName : platform_connect_ap
* Description  : Waits for connection and for an IP address before starting the other tasks.
*                Keeps checking for connection afterwards.
* Parameters   : none
* Returns      : none
*******************************************************************************/
int ICACHE_FLASH_ATTR platform_connect_ap(char *ssid, char *password)
{
    /* TODO: user smart connect */

    struct station_config stationConf;
    static struct ip_info ipinfo;

    unsigned char cnt = 0;
    unsigned char isinit = 0;

    ESP_ALW("Connectiong to %s", ssid);

    memset(&stationConf, 0, sizeof(stationConf));
    strcpy(stationConf.ssid, ssid);
    strcpy(stationConf.password, password);

    //Set station mode
    wifi_set_opmode(STATION_MODE);
    wifi_station_set_config(&stationConf);

    // wait for IP
    for (;;) {
        vTaskDelay(MStoTICKS(2000));
        uint8 connect_status = wifi_station_get_connect_status();
        if (connect_status == STATION_GOT_IP) {      // Got an IP?
            wifi_get_ip_info(0, &ipinfo);                               // Get it

            char *ipptr = (char *) &ipinfo.ip;
            ESP_ALW("Got IP addr [%d.%d.%d.%d]", ipptr[0], ipptr[1], ipptr[2], ipptr[3]);
            return 1;
        }
        else {                                      // Lost our connection?
            if (cnt > 6) {
                ESP_DBG("FAILED TO CONNECT TO %s [connect status %d]", ssid, connect_status);
                return 0;
            }
            ESP_DBG("Waiting for IP address [connect status %d] - %d", connect_status, cnt++);
        }
    }
    return 0;
}

/******************************************************************************
 * FunctionName : user_platform_init
 * Description  : 
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR user_platform_init(void)
{
    ESP_FINTRO();

    sprintf(platform_version, "%s%d.%d.%d", VERSION_TYPE, PLATFORM_VERSION_MAJOR, PLATFORM_VERSION_MINOR, PLATFORM_VERSION_REVISION);
    ESP_ALW("PLATFORM VERSION = %s", platform_version);

    system_param_load(ESP_PARAM_START_SEC, 0, &platform_param, sizeof(platform_param));
    platform_print_reset_reason();

    platform_connect_ap(MYSSID, MYPASSPHRASE);



    ESP_FEXIT();
}

