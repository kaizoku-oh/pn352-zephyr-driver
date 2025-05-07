#include <zephyr/device.h>
#include <zephyr/drivers/nfc.h>
#include <zephyr/logging/log.h>
#include <pn532.h>

LOG_MODULE_REGISTER(pn532, CONFIG_NFC_LOG_LEVEL);

// Defining helper function to send command and receive response
static int pn532_send_command(const struct device *dev, uint8_t cmd, uint8_t *response, size_t *resp_len)
{
    struct pn532_data *data = dev->data;

    // Constructing command frame
    uint8_t frame[8] = {
        PN532_PREAMBLE,
        PN532_STARTCODE1,
        PN532_STARTCODE2,
        0x02, // Length (command + TFI)
        0xFE, // Length checksum
        PN532_HOSTTOPN532,
        cmd,
        0x00 // Checksum placeholder
    };
    uint8_t checksum = PN532_HOSTTOPN532 + cmd;
    frame[7] = ~checksum + 1; // Two's complement checksum

    // Sending command
    int ret = PN532_WRITE(dev, frame, sizeof(frame));
    if (ret) {
        LOG_ERR("Failed to send command: %d", ret);
        return ret;
    }

    // Reading response (assuming max 32 bytes for firmware version)
    uint8_t resp_buf[32];
    ret = PN532_READ(dev, resp_buf, sizeof(resp_buf));
    if (ret < 0) {
        LOG_ERR("Failed to read response: %d", ret);
        return ret;
    }

    // Validating response frame
    if (resp_buf[0] != PN532_PREAMBLE || resp_buf[1] != PN532_STARTCODE1 ||
        resp_buf[2] != PN532_STARTCODE2 || resp_buf[5] != PN532_PN532TOHOST ||
        resp_buf[6] != (cmd + 1)) {
        LOG_ERR("Invalid response frame");
        return -EIO;
    }

    // Copying response data
    size_t len = resp_buf[3] - 1; // Excluding TFI
    if (*resp_len < len) {
        LOG_ERR("Response buffer too small");
        return -ENOMEM;
    }
    memcpy(response, &resp_buf[7], len);
    *resp_len = len;

    return 0;
}

// Implementing function to read firmware version
int pn532_get_firmware_version(const struct device *dev, uint8_t *version, size_t *len)
{
    return pn532_send_command(dev, PN532_CMD_GETFIRMWAREVERSION, version, len);
}

// Defining driver API
static const struct nfc_driver_api pn532_api = {
    .get_firmware_version = pn532_get_firmware_version,
};

// Initializing driver
static int pn532_init(const struct device *dev)
{
    struct pn532_data *data = dev->data;

    // Initializing transport
    int ret = PN532_INIT(dev);
    if (ret) {
        LOG_ERR("Transport init failed: %d", ret);
        return ret;
    }

    // Verifying device communication
    uint8_t version[4];
    size_t len = sizeof(version);
    ret = pn532_get_firmware_version(dev, version, &len);
    if (ret) {
        LOG_ERR("Failed to read firmware version: %d", ret);
        return ret;
    }

    LOG_INF("PN532 initialized, firmware version: %d.%d.%d",
            version[1], version[2], version[3]);

    return 0;
}

// Defining driver instance
#define PN532_DEFINE(inst) \
    static struct pn532_data pn532_data_##inst; \
    static struct pn532_config pn532_config_##inst = { \
        .comm_dev = DEVICE_DT_GET(DT_INST_PHANDLE(inst, i2c) DT_INST_PHANDLE(inst, spi) DT_INST_PHANDLE(inst, uart)), \
    }; \
    DEVICE_DT_INST_DEFINE(inst, pn532_init, NULL, &pn532_data_##inst, \
                         &pn532_config_##inst, POST_KERNEL, \
                         CONFIG_NFC_INIT_PRIORITY, &pn532_api);

DT_INST_FOREACH_STATUS_OKAY(PN532_DEFINE)
#endif