#include "pn532.h"
#include <zephyr/drivers/uart.h>

int pn532_write(const struct device *dev, const uint8_t *buf, size_t len)
{
	for (size_t i = 0; i < len; ++i) {
		uart_poll_out(dev, buf[i]);
	}
	return 0;
}

int pn532_read(const struct device *dev, uint8_t *buf, size_t len)
{
	for (size_t i = 0; i < len; ++i) {
		uart_poll_in(dev, &buf[i]);
	}
	return 0;
}
