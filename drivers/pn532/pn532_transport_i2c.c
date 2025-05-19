/*
 * Copyright (c) 2025 Bayrem Gharsellaoui
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(pn532_i2c, CONFIG_PN532_LOG_LEVEL);

#include "pn532.h"
#include "pn532_transport.h"

int pn532_transport_init(const struct device *dev)
{
    ARG_UNUSED(dev);
    LOG_INF("PN532 I2C transport initialized");

    return 0;
}

int pn532_transport_write(const struct device *dev, const uint8_t *buf, size_t len)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(buf);
    ARG_UNUSED(len);

    return 0;
}

int pn532_transport_read(const struct device *dev, uint8_t *buf, size_t len)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(buf);
    ARG_UNUSED(len);

    return 0;
}
