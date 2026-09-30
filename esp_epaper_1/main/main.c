#include <stdint.h>
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define GPIO_DIN GPIO_NUM_14
#define GPIO_CLK GPIO_NUM_13
#define GPIO_CS GPIO_NUM_15
#define GPIO_DC GPIO_NUM_27
#define GPIO_RST GPIO_NUM_32
#define GPIO_BUSY GPIO_NUM_25
#define GPIO_PWR GPIO_NUM_33
void wait_for_busy(void)
{
    for (int i = 0; i < 100; i++) {
        if (gpio_get_level(GPIO_BUSY) == 1) {
            printf("BUSY released\n");
            return;
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }

    printf("BUSY timeout, continue anyway\n");
}
// void wait_for_busy(void) {
//     vTaskDelay(pdMS_TO_TICKS(100));

//     while (gpio_get_level(GPIO_BUSY) == 0) {
//         printf("BUSY = %d\n", gpio_get_level(GPIO_BUSY));
//         vTaskDelay(pdMS_TO_TICKS(20));
//     }

//     return;
// }

void gpio_pin_init(void)
{
    uint64_t mask =
        (1ULL << 14) |
        (1ULL << 13) |
        (1ULL << 15) |
        (1ULL << 27) |
        (1ULL << 32) |
        (1ULL << 33);

    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = mask;
    io_conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io_conf);

    // BUSYだけ別に宣言する
    gpio_config_t busy_conf = {};
    busy_conf.pin_bit_mask = 1ULL << GPIO_BUSY;
    busy_conf.mode = GPIO_MODE_INPUT;
    busy_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&busy_conf);

    gpio_set_level(GPIO_PWR, 1);
    gpio_set_level(GPIO_CS, 1);
    gpio_set_level(GPIO_DC, 0);
    gpio_set_level(GPIO_RST, 1);
    gpio_set_level(GPIO_CLK, 0);
}

void send_command(uint8_t command)
{
    gpio_set_level(GPIO_CS, 0);
    gpio_set_level(GPIO_DC, 0);
    
    uint8_t i = 0;
    while (i < 8) {
        gpio_set_level(GPIO_DIN, (command >> (7 - i)) & 1);
        gpio_set_level(GPIO_CLK, 1);
        gpio_set_level(GPIO_CLK, 0);

        i++;
    }

    gpio_set_level(GPIO_CS, 1);
    gpio_set_level(GPIO_DC, 1);
}

void send_parameter(uint8_t parameter)
{
    gpio_set_level(GPIO_DC, 1);
    gpio_set_level(GPIO_CS, 0);

    uint8_t i = 0;
    while (i < 8) {
        gpio_set_level(GPIO_DIN, (parameter >> (7 - i)) & 1);
        gpio_set_level(GPIO_CLK, 1);
        gpio_set_level(GPIO_CLK, 0);

        i++;
    }

    gpio_set_level(GPIO_CS, 1);
}

void epaper_reset(void)
{
    gpio_set_level(GPIO_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(200));

    gpio_set_level(GPIO_RST, 0);
    esp_rom_delay_us(2000);   // 2ms

    gpio_set_level(GPIO_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(200));

    // BUSY_N: LOW = busy, HIGH = ready
    wait_for_busy();

    return ;
}

void epaper_init(void)
{
    epaper_reset();

    send_command(0x4D);
    send_parameter(0x78);

    send_command(0x00);
    send_parameter(0x0F);
    send_parameter(0x29);

    send_command(0x01);
    send_parameter(0x07);
    send_parameter(0x00);

    send_command(0x03);
    send_parameter(0x10);
    send_parameter(0x54);
    send_parameter(0x44);

    send_command(0x06);
    send_parameter(0x0F);
    send_parameter(0x0A);
    send_parameter(0x2F);
    send_parameter(0x25);
    send_parameter(0x22);
    send_parameter(0x2E);
    send_parameter(0x21);

    send_command(0x41);
    send_parameter(0x00);

    send_command(0x50);
    send_parameter(0x37);

    send_command(0x60);
    send_parameter(0x02);
    send_parameter(0x02);

    // 128 x 296
    send_command(0x61);
    send_parameter(0x00);
    send_parameter(0x80);
    send_parameter(0x01);
    send_parameter(0x28);

    send_command(0x65);
    send_parameter(0x00);
    send_parameter(0x00);
    send_parameter(0x00);
    send_parameter(0x00);

    // 以下には公開データシートに載っていないものもある
    send_command(0xE7);
    send_parameter(0x1C);

    send_command(0xE3);
    send_parameter(0x22);

    send_command(0xB4);
    send_parameter(0xD0);

    send_command(0xB5);
    send_parameter(0x03);

    send_command(0xE9);
    send_parameter(0x01);

    send_command(0x30);
    send_parameter(0x08);

    // Power ON
    send_command(0x04);
    vTaskDelay(pdMS_TO_TICKS(500));
    wait_for_busy();
}

void app_main(void)
{
    gpio_pin_init();
    // while (1) {
    //     printf("BUSY = %d\n", gpio_get_level(GPIO_BUSY));
    //     vTaskDelay(pdMS_TO_TICKS(500));
    // }

    // 決まり事。公式サンプルからのコピペ
    epaper_init();


    // DTM1(R10h)
    send_command(0x10);
    int i = 0;
    while (i < 9472) {
        send_parameter(0x1B);
        i++;
    }

    send_command(0x12);
    send_parameter(0x00);
}