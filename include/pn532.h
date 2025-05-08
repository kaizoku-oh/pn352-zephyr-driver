#ifndef _PN532_H_
#define _PN532_H_

#include <zephyr/device.h>
#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

__subsystem struct pn532_driver_api {
	int (*get_fw_version)(const struct device *dev, uint32_t *version);
};

static inline int pn532_get_fw_version(const struct device *dev, uint32_t *version)
{
	const struct pn532_driver_api *api = (const struct pn532_driver_api *)dev->api;
	return api->get_fw_version(dev, version);
}

int pn532_write(const struct device *dev, const uint8_t *buf, size_t len);
int pn532_read(const struct device *dev, uint8_t *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* _PN532_H_ */
