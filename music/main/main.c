#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

#define LEDC_SPEED_MODE   LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES     LEDC_TIMER_13_BIT
#define LEDC_DUTY_50PCT   4096   // 8192段階（13bit）の50%
#define BUZZER_GPIO       GPIO_NUM_25

// タイマーとチャンネルを一度だけ初期化する
void ledc_init(ledc_timer_t timer_num,ledc_channel_t channel_num, int gpio_num) {
    ledc_timer_config_t timer_config = {};
    timer_config.speed_mode = LEDC_SPEED_MODE;
    timer_config.timer_num = timer_num;
    timer_config.freq_hz = 440; // 初期値。以後は play_note 内で変更する
    timer_config.duty_resolution = LEDC_DUTY_RES;
    timer_config.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    ledc_channel_config_t chan_config = {};
    chan_config.gpio_num = gpio_num;
    chan_config.speed_mode = LEDC_SPEED_MODE;
    chan_config.channel = channel_num;
    chan_config.timer_sel = timer_num;
    chan_config.duty = LEDC_DUTY_50PCT;
    ESP_ERROR_CHECK(ledc_channel_config(&chan_config));
}

// 指定した周波数の音を duration_sec 秒間鳴らす
void play_note(float duration_sec, uint32_t frequency_hz) {
    if (frequency_hz == 0) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);
    } else {
        ESP_ERROR_CHECK(ledc_set_freq(LEDC_SPEED_MODE, LEDC_TIMER_0, frequency_hz));
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, LEDC_DUTY_50PCT);

        ESP_ERROR_CHECK(ledc_set_freq(LEDC_SPEED_MODE, LEDC_TIMER_1, frequency_hz*2));
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, LEDC_DUTY_50PCT);
    }
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);


    const TickType_t xDelay = pdMS_TO_TICKS(duration_sec * 500);
    vTaskDelay(xDelay);

    ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_1, 0);
    ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_1);
    vTaskDelay(pdMS_TO_TICKS(30));
}

void app_main(void) {
    ledc_init(LEDC_TIMER_0, LEDC_CHANNEL_0, GPIO_NUM_25);
    ledc_init(LEDC_TIMER_1, LEDC_CHANNEL_1, GPIO_NUM_18);

    play_note(2, 0);
    play_note(0.25, 392); // ソ (G4: 392Hz)
    play_note(0.5, 0);
    play_note(0.25, 392); // ソ (G4: 392Hz)
    play_note(1, 523); // ド (C5: 523Hz)
    play_note(1, 587); // レ (D5: 587Hz)
    play_note(1, 659); // ミ (E5: 659Hz)
    play_note(1, 698); // ファ (F5: 698Hz)
    play_note(1, 783);
    play_note(2, 1046);
    play_note(0.75, 987);
    play_note(0.25, 880);
    play_note(1, 880);
    play_note(1.5, 783);
    play_note(0.5, 739);
    play_note(0.5, 739);
    play_note(0.5, 880);
    play_note(1, 783);
    play_note(2.5, 659);

    play_note(0.25, 329);
    play_note(0.5, 0);
    play_note(0.25, 329);
    play_note(2, 329);
    play_note(1, 369);
    play_note(1, 414);
    play_note(2.5, 439);
    play_note(0.5, 439);
    play_note(0.5, 492);
    play_note(0.5, 522);
    play_note(2.5, 586);
    play_note(0.5, 439);
    play_note(0.5, 439);
    play_note(0.5, 522);
    play_note(1, 522);
    play_note(1, 492);
    play_note(1, 439);
    play_note(1, 391);
    play_note(2.5, 782);
    play_note(0.5, 685);
    play_note(0.5, 697);
    play_note(0.5, 782);
    play_note(2.5, 878);
    
    play_note(0.5, 439);
    play_note(0.5, 492);
    play_note(0.5, 522);
    play_note(2, 697);
    play_note(2, 658);
    play_note(3, 522);
}