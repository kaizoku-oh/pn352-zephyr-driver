#include <zephyr/drivers/spi.h>
#include <zephyr/logging/log.h>
#include <pn532.h>

LOG_MODULE_DECLARE(pn532, CONFIG_NFC_LOG_LEVEL);

// Implementing SPI initialization
int pn532_spi_init(const struct device *dev)
{
    struct pn532_data *data = dev->data;
    const struct pn532_config *config = dev->config;

    if (!device_is_ready(config->comm_dev)) {
        LOG_ERR("SPI device not ready");
        return -ENODEV;
    }

    if (data->cs_gpio.port) {
        if (!device_is_ready(data->cs_gpio.port)) {
            LOG_ERR("CS GPIO not ready");
            return -ENODEV;
        }
        gpio_pin_configure_dt(&data->cs_gpio, GPIO_OUTPUT_HIGH);
    }

    return 0;
}

// Implementing SPI write
int pn532_spi_write(const struct device *dev, const uint8_t *data, size_t len)
{
    struct pn532_data *data = dev->data;
    const struct pn532_config *config = dev->config;

    struct spi_buf buf = {.buf = (void *)data, .len = len};
    struct spi_buf_set tx = {.buffers = &buf, .count = 1};

    if (data->cs_gpio.port) {
        gpio_pin_set_dt(&data->cs_gpio, 0);
    }

    int ret = spi_write(config->comm_dev, &spi_config, &tx);

    if (data->cs_gpio.port) {
        gpio_pin_set_dt(&data->cs_gpio, 1);
    }

    return ret;
}

// Implementing SPI read
int pn532_spi_read(const struct device *dev, uint8_t *data, size_t len)
{
    struct pn532_data *data = dev->data;
    const struct pn532_config *config = dev->config;

    struct spi_buf buf = {.buf = data, .len = len};
    struct spi_buf_set rx = {.buffers = &buf, .count = 1};

    if (data->cs_gpio.port) {
        gpio_pin_set_dt(&data->cs_gpio, 0);
    }

    int ret = spi_read(config->comm_dev, &spi_config, &rx);

    if (data->cs_gpio.port) {
        gpio_pin_set_dt(&data->cs_gpio, 1);
    }

    return ret;
}