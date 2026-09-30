#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "soc/gpio_num.h"

void app_main(void)
{
    gpio_config_t io_conf_1 = {};

    io_conf_1.mode = GPIO_MODE_OUTPUT;
    io_conf_1.pin_bit_mask = (1ULL << GPIO_NUM_2);

    gpio_config(&io_conf_1);

    gpio_config_t io_conf_2 = {};
    io_conf_2.pin_bit_mask = (1ULL << GPIO_NUM_33);
    io_conf_2.mode = GPIO_MODE_OUTPUT;
    
    gpio_config(&io_conf_2);

    int cnt = 0;
    while (1) {
        printf("cnt: %d\n", cnt++);
        gpio_set_level(GPIO_NUM_2, cnt % 2);
        gpio_set_level(GPIO_NUM_33, (cnt + 1) % 2);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}