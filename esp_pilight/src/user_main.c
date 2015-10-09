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

#include "user_config.h"
#include "user_platform.h"
#include "dht22.h"

#include "debug.h"


/******************************************************************************
* FunctionName : main_task
* Description  : Does all the other stuff
* Parameters   : none
* Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR main_task (void *pvParameters)
{
    // Waiting for connect semaphore
    //while (xSemaphoreTake (semConnect, portMAX_DELAY) != pdTRUE);
    ESP_ALW ("CONNECTED");

    //http_server_netconn_init();
    //DHT_init();

    while (1);
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR user_init(void)
{
    ESP_ALW ("SDK version:%s", system_get_sdk_version());

    user_platform_init();
    DHT_init();

    while (1) {
        vTaskDelay(MStoTICKS(2000));
        ESP_ALW("READ DHT:");
        DHT_read_temp();
    }
}

