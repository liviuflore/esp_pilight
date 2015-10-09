#ifndef __USER_PLATFORM_H__
#define __USER_PLATFORM_H__



#define PLATFORM_VERSION_MAJOR      1U
#define PLATFORM_VERSION_MINOR      0U
#define PLATFORM_VERSION_REVISION   5U

#define VERSION_NUM                 (PLATFORM_VERSION_MAJOR * 1000 + PLATFORM_VERSION_MINOR * 100 + PLATFORM_VERSION_REVISION)
#define VERSION_TYPE                "v"

#define ESP_PARAM_START_SEC         0x3D

#define MStoTICKS(ms)               (ms / portTICK_RATE_MS)

struct platform_saved_param {
    uint8 devkey[40];
    uint8 token[40];
    uint8 activeflag;
    uint8 pad[3];
};

struct dhcp_client_info {
    ip_addr_t ip_addr;
    ip_addr_t netmask;
    ip_addr_t gw;
    uint8 flag;
    uint8 pad[3];
};


void ICACHE_FLASH_ATTR user_platform_init(void);


#endif /*__USER_PLATFORM_H__*/


