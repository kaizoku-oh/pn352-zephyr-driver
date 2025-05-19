/*
 * Copyright (c) 2025 Bayrem Gharsellaoui
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT nxp_pn532

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(pn532, CONFIG_PN532_LOG_LEVEL);

#include "pn532.h"
#include "pn532_transport.h"

struct pn532_data {
    uint32_t dummy;
};

struct pn532_config {
    const struct device *i2c_dev;
};

static int get_firmware_version(const struct device *dev, uint32_t *version)
{
    if ((dev == NULL) || (version == NULL)) {
        return -EINVAL;
    }

    *version = 0x01020304;

    return 0;
}

static DEVICE_API(pn532, pn532_api) = {
    .pn532_get_firmware_version = &get_firmware_version,
};

static int pn532_init(const struct device *dev)
{
    int ret = pn532_transport_init(dev);
    if (ret < 0) {
        LOG_ERR("Transport init failed: %d", ret);
        return ret;
    }

    LOG_INF("PN532 initialized");

    return 0;
}

#define PN532_DEFINE(inst)                            \
    static struct pn532_data data##inst;              \
                                                      \
    static const struct pn532_config config##inst = { \
        .i2c_dev = DEVICE_DT_GET(DT_INST_BUS(inst)),  \
    };                                                \
                                                      \
    DEVICE_DT_INST_DEFINE(inst,                       \
                          pn532_init,                 \
                          NULL,                       \
                          &data##inst,                \
                          &config##inst,              \
                          POST_KERNEL,                \
                          CONFIG_PN532_INIT_PRIORITY, \
                          &pn532_api);

DT_INST_FOREACH_STATUS_OKAY(PN532_DEFINE)
