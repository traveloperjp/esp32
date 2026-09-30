#include "common.h"
#include "services/gap/ble_svc_gap.h"

int gap_init(void) {
    int rc = 0;

    ble_svc_gap_init();

    rc = ble_svc_gap_device_name_set(DEVICE_NAME);
    if (rc != 0) {
        ESP_LOGE(TAG, "failed to set device name to %s, error code: %d",
                 DEVICE_NAME, rc);
        return rc;
    }

    return rc;
}