#include "pn532.h"

static int pn532_get_fw_version_impl(const struct device *dev, uint32_t *version)
{
    uint8_t cmd[] = { 0x02 };
    uint8_t resp[6] = {0};

    pn532_write(dev, cmd, sizeof(cmd));
    pn532_read(dev, resp, sizeof(resp));

    *version = (resp[3] << 16) | (resp[4] << 8) | resp[5];
    return 0;
}

static const struct pn532_driver_api api = {
    .get_fw_version = pn532_get_fw_version_impl,
};

#define PN532_INIT(n) \
    DEVICE_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &api);

DT_INST_FOREACH_STATUS_OKAY(PN532_INIT)
