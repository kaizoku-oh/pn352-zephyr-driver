#include "pn532.h"
#include <zephyr/drivers/spi.h>

int pn532_write(const struct device *dev, const uint8_t *buf, size_t len)
{
	const struct spi_dt_spec *bus = dev->config;
	const struct spi_buf tx_buf = { .buf = (void *)buf, .len = len };
	const struct spi_buf_set tx = { .buffers = &tx_buf, .count = 1 };
	return spi_write_dt(bus, &tx);
}

int pn532_read(const struct device *dev, uint8_t *buf, size_t len)
{
	const struct spi_dt_spec *bus = dev->config;
	const struct spi_buf rx_buf = { .buf = buf, .len = len };
	const struct spi_buf_set rx = { .buffers = &rx_buf, .count = 1 };
	return spi_read_dt(bus, &rx);
}
