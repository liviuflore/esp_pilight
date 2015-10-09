#ifndef __DEBUG_H__
#define __DEBUG_H__


#define ESP_DEBUG
#define ESP_TRACE

#define MODULE_NAME "WS"

#ifdef WIN32
#define os_printf   printf
#endif

#ifdef ESP_DEBUG
#define ESP_ERR(msg,...)    printf(MODULE_NAME " ERR: [%s][%d] " msg "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define ESP_INF(msg,...)    printf(MODULE_NAME " INF: [%s][%d] " msg "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define ESP_DBG(msg,...)    printf(MODULE_NAME " DBG: [%s][%d] " msg "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define ESP_ERR(msg,...)    printf(MODULE_NAME " ERR: [%s][%d] " msg "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define ESP_INF(...)
#define ESP_DBG(...)
#endif

#ifdef ESP_TRACE
#define ESP_FINTRO()        printf(MODULE_NAME " Entering %s\n", __FUNCTION__)
#define ESP_FEXIT()         printf(MODULE_NAME " Exiting  %s\n", __FUNCTION__)
#else
#define ESP_FINTRO()
#define ESP_FEXIT()
#endif

#define ESP_ALW(msg,...)    printf(MODULE_NAME " " msg "\n", ##__VA_ARGS__)
#define ESP_ASSERT(cond)    if (!(cond)) {printf(MODULE_NAME " ASSERT FAILED! (" #cond ")\n", __FUNCTION__, __LINE__);}


#endif

