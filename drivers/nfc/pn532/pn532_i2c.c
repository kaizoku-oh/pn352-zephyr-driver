#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <pn532.h>

LOG_MODULE_DECLARE(pn532, CONFIG_NFC_LOG_LEVEL);

// Implementing I2C initialization
int pn532_i2c_init(const struct device *dev)
{
    const struct pn532_config *config = dev->config;

    if (!device_is_ready(config->comm_dev)) {
        LOG_ERR("I2C device not ready");
        return -ENODEV;
    }

    return 0;
}

// Implementing I2C write
int pn532_i2c_write(const struct device *dev, const uint8_t *data, size_t len)
{
    const struct pn532_config *config = dev->config;
    return i2c_write(config->comm_dev, data, len, DT_REG_ADDR(DT_DRV_INST(0)));
}

// Implementing I2C read
int pn532_i2c_read(const struct device *dev, uint8_t *data, size_t len)
{
    const struct pn532_config *config = dev->config;
    return i2c_read(config->comm_dev, data, len, DT_REG_ADDR(DT_DRV_INST(0)));
}