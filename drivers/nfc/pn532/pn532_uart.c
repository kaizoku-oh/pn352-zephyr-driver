#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include <pn532.h>

LOG_MODULE_DECLARE(pn532, CONFIG_NFC_LOG_LEVEL);

// Implementing UART initialization
int pn532_uart_init(const struct device *dev)
{
    const struct pn532_config *config = dev->config;

    if (!device_is_ready(config->comm_dev)) {
        LOG_ERR("UART device not ready");
        return -ENODEV;
    }

    return 0;
}

// Implementing UART write
int pn532_uart_write(const struct device *dev, const uint8_t *data, size_t len)
{
    const struct pn532_config *config = dev->config;

    for (size_t i = 0; i < len; i++) {
        uart_poll_out(config->comm_dev, data[i]);
    }

    return 0;
}

// Implementing UART read
int pn532_uart_read(const struct device *dev, uint8_t *data, size_t len)
{
    const struct pn532_config *config = dev->config;
    size_t received = 0;

    while (received < len) {
        if (uart_poll_in(config->comm_dev, &data[received]) == 0) {
            received++;
        } else {
            k_msleep(1);
        }
    }

    return received == len ? 0 : -EIO;
}