#include "pn532.h"
#include <zephyr/drivers/i2c.h>

int pn532_write(const struct device *dev, const uint8_t *buf, size_t len)
{
	const struct i2c_dt_spec *bus = dev->config;
	return i2c_write_dt(bus, buf, len);
}

int pn532_read(const struct device *dev, uint8_t *buf, size_t len)
{
	const struct i2c_dt_spec *bus = dev->config;
	return i2c_read_dt(bus, buf, len);
}
