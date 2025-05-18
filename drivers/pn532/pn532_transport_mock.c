/*
 * Copyright (c) 2025 Bayrem Gharsellaoui
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(pn532_test, CONFIG_PN532_LOG_LEVEL);

#include "pn532_transport.h"

int pn532_transport_init(const struct device *dev)
{
    LOG_INF("Mock PN532 transport initialized");

    return 0;
}

int pn532_transport_write(const struct device *dev, const uint8_t *buf, size_t len)
{
    LOG_DBG("Mock write %zu bytes", len);

    return 0;
}

int pn532_transport_read(const struct device *dev, uint8_t *buf, size_t len)
{
    LOG_DBG("Mock read %zu bytes", len);
    memset(buf, 0xA5, len);

    return 0;
}
