#include "common.h"
#include "esp_log.h"
#include "gap.h"
#include "led.h"
#include "nimble/nimble_port.h"
#include "nvs.h"
#include "esp_err.h"
#include "portmacro.h"
#include "gatt_svc.h"

void app_main(void) {
    BaseType_t rc = 0;
    esp_err_t ret;

    led_init();
    nvs_init();

    ret = nimble_port_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "failed to initialize nimble stack, error code: %d ",
                 ret);
        return;
    }

    rc = gap_init();
    if (rc != 0) {
        ESP_LOGE(TAG, "failed to initialize GAP service, error code: %d", rc);
        return;
    }

    rc = gatt_svc_init();
    if (rc != 0) {
        ESP_LOGE(TAG, "failed to initialize GATT server, error code: %d", rc);
        nimble_port_deinit();
        return;
    }

    nimble_host_config_init();
}