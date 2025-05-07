#ifndef PN532_H
#define PN532_H

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

// Defining PN532 frame constants
#define PN532_PREAMBLE 0x00
#define PN532_STARTCODE1 0x00
#define PN532_STARTCODE2 0xFF
#define PN532_HOSTTOPN532 0xD4
#define PN532_PN532TOHOST 0xD5

// Defining command codes
#define PN532_CMD_GETFIRMWAREVERSION 0x02

// Defining driver context structure
struct pn532_data {
    const struct device *comm_dev;
    struct gpio_dt_spec cs_gpio; // Used for SPI
};

// Defining configuration structure
struct pn532_config {
    const struct device *comm_dev;
};

// Declaring transport-specific functions
#ifdef CONFIG_PN532_I2C
int pn532_i2c_init(const struct device *dev);
int pn532_i2c_write(const struct device *dev, const uint8_t *data, size_t len);
int pn532_i2c_read(const struct device *dev, uint8_t *data, size_t len);
#define PN532_INIT pn532_i2c_init
#define PN532_WRITE pn532_i2c_write
#define PN532_READ pn532_i2c_read
#elif defined(CONFIG_PN532_SPI)
int pn532_spi_init(const struct device *dev);
int pn532_spi_write(const struct device *dev, const uint8_t *data, size_t len);
int pn532_spi_read(const struct device *dev, uint8_t *data, size_t len);
#define PN532_INIT pn532_spi_init
#define PN532_WRITE pn532_spi_write
#define PN532_READ pn532_spi_read
#elif defined(CONFIG_PN532_UART)
int pn532_uart_init(const struct device *dev);
int pn532_uart_write(const struct device *dev, const uint8_t *data, size_t len);
int pn532_uart_read(const struct device *dev, uint8_t *data, size_t len);
#define PN532_INIT pn532_uart_init
#define PN532_WRITE pn532_uart_write
#define PN532_READ pn532_uart_read
#endif

#endif // PN532_H