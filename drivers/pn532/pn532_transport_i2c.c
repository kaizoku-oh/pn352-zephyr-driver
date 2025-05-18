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

#define PN532_I2C_ADDRESS DT_REG_ADDR(DT_DRV_INST(0))

struct pn532_config {
    const struct device *i2c_dev;
};

int pn532_transport_init(const struct device *dev)
{
    const struct pn532_config *cfg = dev->config;

    if (!device_is_ready(cfg->i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -ENODEV;
    }

    return 0;
}

int pn532_transport_write(const struct device *dev, const uint8_t *buf, size_t len)
{
    const struct pn532_config *cfg = dev->config;
    return i2c_write(cfg->i2c_dev, buf, len, PN532_I2C_ADDRESS);
}

int pn532_transport_read(const struct device *dev, uint8_t *buf, size_t len)
{
    const struct pn532_config *cfg = dev->config;
    return i2c_read(cfg->i2c_dev, buf, len, PN532_I2C_ADDRESS);
}
