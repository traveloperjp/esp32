#include "common.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "soc/gpio_num.h"
#include <stdint.h>

static uint8_t led_state;
uint8_t get_led_state(void) { return led_state; }

void led_on(void) {
    gpio_set_level(GPIO_NUM_0, true);
}

void led_off(void) {
    gpio_set_level(GPIO_NUM_0, false);
}

void led_init(void) {
    ESP_LOGI(TAG, "example configured to blink gpio led!");
    gpio_reset_pin(GPIO_NUM_0);
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_OUTPUT);
}