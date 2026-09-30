#include "host/ble_gatt.h"
#include "host/ble_uuid.h"
#include "services/gatt/ble_svc_gatt.h"
#include <stdint.h>

static const ble_uuid16_t heart_rate_chr_uuid = BLE_UUID16_INIT(0x2A37);
static int heart_rate_chr_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *args);
static int led_chr_access(uint16_t conn_handle, uint16_t attr_handle,struct ble_gatt_access_ctxt *ctxt, void *arg);

static const ble_uuid16_t heart_rate_svc_uuid = BLE_UUID16_INIT(0x180D);
static uint16_t heart_rate_chr_val_handle;

static const ble_uuid16_t auto_io_svc_uuid = BLE_UUID16_INIT(0x1815);
static const ble_uuid128_t led_chr_uuid = BLE_UUID128_INIT( 0x23, 0xd1, 0xbc, 0xea, 0x5f, 0x78, 0x23, 0x15, 0xde, 0xef, 0x12, 0x12, 0x25, 0x15, 0x00, 0x00 );

static uint16_t led_chr_val_handle;

static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &heart_rate_svc_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]) {
            {
                .uuid = &heart_rate_chr_uuid.u,
                .access_cb = heart_rate_chr_access,
                .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_INDICATE,
                .val_handle = &heart_rate_chr_val_handle
            },
            {
                0,
            }
        }
    },
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &auto_io_svc_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]) {
            {
                .uuid = &led_chr_uuid.u,
                .access_cb = led_chr_access,
                .flags = BLE_GATT_CHR_F_WRITE,
                .val_handle = &led_chr_val_handle,
            },
            {
                0,
            }
        }
    },
    {
        0,
    }
};


int gatt_svc_init(void) {
    int rc = 0;

    ble_svc_gatt_init();
    
    rc = ble_gatts_count_cfg(gatt_svr_svcs);
    if (rc != 0) {
        return rc;
    }

    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    if (rc != 0) {
        return rc;
    }

    return 0;
}