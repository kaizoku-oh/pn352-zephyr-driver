#include <zephyr/ztest.h>
#include <zephyr/drivers/nfc.h>
#include <pn532.h>

// Mocking I2C transport functions (default for test)
static int mock_init(const struct device *dev) { return 0; }
static int mock_write(const struct device *dev, const uint8_t *data, size_t len)
{
    zassert_equal(len, 8, "Expected command length 8");
    zassert_equal(data[5], PN532_HOSTTOPN532, "Expected TFI");
    zassert_equal(data[6], PN532_CMD_GETFIRMWAREVERSION, "Expected command");
    return 0;
}
static int mock_read(const struct device *dev, uint8_t *data, size_t len)
{
    // Simulating firmware version response
    uint8_t response[] = {
        PN532_PREAMBLE, PN532_STARTCODE1, PN532_STARTCODE2,
        0x07, 0xF9, PN532_PN532TOHOST, PN532_CMD_GETFIRMWAREVERSION + 1,
        0x32, 0x01, 0x06, 0x07, 0x00
    };
    memcpy(data, response, MIN(len, sizeof(response)));
    return sizeof(response);
}

// Defining test driver instance
static struct pn532_data test_data;
static struct pn532_config test_config = {
    .comm_dev = NULL,
};
static const struct nfc_driver_api test_api = {
    .get_firmware_version = pn532_get_firmware_version,
};
DEVICE_DEFINE(test_pn532, "TEST_PN532", mock_init, NULL, &test_data, &test_config,
              POST_KERNEL, CONFIG_NFC_INIT_PRIORITY, &test_api);

// Testing firmware version reading
ZTEST(pn532, test_get_firmware_version)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(test_pn532));
    uint8_t version[4];
    size_t len = sizeof(version);

    // Setting mock transport functions
    int (*orig_write)(const struct device *, const uint8_t *, size_t) = PN532_WRITE;
    int (*orig_read)(const struct device *, uint8_t *, size_t) = PN532_READ;
    PN532_WRITE = mock_write;
    PN532_READ = mock_read;

    int ret = pn532_get_firmware_version(dev, version, &len);
    zassert_equal(ret, 0, "Failed to get firmware version");
    zassert_equal(len, 4, "Expected version length 4");
    zassert_equal(version[0], 0x32, "Expected IC 0x32");
    zassert_equal(version[1], 0x01, "Expected version 1");
    zassert_equal(version[2], 0x06, "Expected subversion 6");
    zassert_equal(version[3], 0x07, "Expected support 0x07");

    // Restoring original functions
    PN532_WRITE = orig_write;
    PN532_READ = orig_read;
}

ZTEST_SUITE(pn532, NULL, NULL, NULL, NULL, NULL);