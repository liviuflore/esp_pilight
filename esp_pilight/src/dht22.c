#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/portmacro.h"
#include "sys/time.h"
#include "drivers/gpio.h"
#include "debug.h"


#define DHT_READ_INTERVAL 5000
#define MAXTIMINGS 10000
#define BREAKTIME 20

#define DHT_PIN_NO 2
#define DHT_PIN_INT_MODE GPIO_PIN_INTR_ANYEGDE

void ICACHE_FLASH_ATTR registerInterrupt(int pin, GPIO_INT_TYPE mode);

LOCAL void DHT_gpio_int_callback(void *arg);
void LOCAL DHT_read_bit();


int ICACHE_FLASH_ATTR DHT_init (void)
{
    gpio_intr_handler_register(DHT_gpio_int_callback, NULL);
    //gpio_intr_unmask();
    registerInterrupt(DHT_PIN_NO, DHT_PIN_INT_MODE);

    return 1;
}

void ICACHE_FLASH_ATTR registerInterrupt(int pin, GPIO_INT_TYPE mode)
{
    portENTER_CRITICAL();

    // disable open drain
    GPIO_REG_WRITE(GPIO_PIN_ADDR(GPIO_ID_PIN(pin)),
                   GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(pin))) & (~GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)));

    //clear interrupt status
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(pin));

    // set the mode   
    gpio_pin_intr_state_set(GPIO_ID_PIN(pin), mode);

    portEXIT_CRITICAL();
}

LOCAL void DHT_gpio_int_callback(void *arg)
{
    int i;
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
    printf("gpio_int_callback! %08X\n", gpio_status);
    if ((gpio_status & BIT(DHT_PIN_NO))) {
        printf("triggered on int: %d\n", DHT_PIN_NO);
        //disable interrupt
        gpio_pin_intr_state_set(GPIO_ID_PIN(DHT_PIN_NO), GPIO_PIN_INTR_DISABLE);
        // call func here
        DHT_read_bit();
        //clear interrupt status
        GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(DHT_PIN_NO));
        // restore
        gpio_pin_intr_state_set(GPIO_ID_PIN(DHT_PIN_NO), DHT_PIN_INT_MODE);
    }
}

int timings[2][1024] = { { 0 } };
int timings_current = 0;
int timings_index[2] = { 0 };
struct timeval  last_tv = { 0 };
void ICACHE_FLASH_ATTR DHT_read_bit()
{
    struct timeval  tv, tv_diff;
    if (timings_index[timings_current] >= 1024) {
        printf("||| overflow 1024 |||\n");
        return;
    }
    gettimeofday(&tv, NULL);
    timersub(tv, last_tv, tv_diff);
    last_tv.tv_sec = tv.tv_sec;
    last_tv.tv_usec = tv.tv_usec;
    timings[timings_current][timings_index[timings_current]++] = (tv_diff.tv_sec * 1000000) + tv_diff.tv_usec;
}

int ICACHE_FLASH_ATTR DHT_read_temp()
{
    int i;

    gpio_intr_mask();

    //print old timings
    if (timings_index[timings_current] != 0) {
        ESP_ALW("recorded %d timings", timings_index[timings_current]);
        for (i = 0; i < timings_index[timings_current]; i++) {
            printf("%d ", timings[timings_current][timings_index[timings_current]]);
        }
        ESP_ALW("done");
    }
    else
        ESP_ALW("nothing recorded");
    //set current recoring place
    timings_current = (timings_current + 1) % 2;
    timings_index[timings_current] = 0;

    GPIO_OUTPUT_SET(2, 1);
    os_delay_us(50000);
    os_delay_us(50000);
    os_delay_us(50000);
    os_delay_us(50000);
    os_delay_us(50000);
    GPIO_OUTPUT_SET(2, 0);
    os_delay_us(20000);
    GPIO_OUTPUT_SET(2, 1);
    os_delay_us(40);
    GPIO_DIS_OUTPUT(2);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);
    gpio_intr_unmask();

    // delay some time
    // read from timings
    // gpio_intr_mask();

    return 0;
}

