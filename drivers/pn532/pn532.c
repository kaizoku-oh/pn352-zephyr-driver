/*
 * Copyright (c) 2025 Bayrem Gharsellaoui
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT nxp_pn532

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(pn532, CONFIG_PN532_LOG_LEVEL);

#include "pn532.h"
#include "pn532_transport.h"

#define PREAMBLE                            0x00
#define START_OF_PACKET_CODE                0x00FF
#define PACKET_LENGTH                       LEN
#define PACKET_LENGTH_CHECKSUM              LCS
#define SPECIFIC_PN532_FRAME_IDENTIFIER     TFI
#define PACKET_DATA                         PD0...PDn
#define PACKET_DATA_CHECKSUM                DCS
#define POSTAMBLE                           0x00

/*
 * PN532 normal information frame format:
 *
 * Preamble        : 1 byte (0x00)
 * Start Code      : 2 byte (0x00, 0xFF)
 * Length          : 1 byte (length of TFI + data)
 * Length Checksum : 1 byte (LCS = 0x100 - Length)
 * Frame Identifier: 1 byte (TFI = 0xD4 for host-to-PN532, 0xD5 for PN532-to-host)
 * Data            : N bytes (payload)
 * Data Checksum   : 1 byte, such that (TFI + data + DCS) & 0xFF == 0x00
 * Postamble       : 0x00
 */
struct pn532_info_frame {
    uint8_t preamble;
    uint8_t start_code_1;
    uint8_t start_code_2;
    uint8_t len;
    uint8_t lcs;
    uint8_t tfi;
    uint8_t *data;
    uint8_t dcs;
    uint8_t postamble;
};

/*
 * PN532 acknowledgement frame format:
 *
 * Preamble   : 1 byte (0x00)
 * Start Code : 2 byte (0x00, 0xFF)
 * ACK Code   : 2 byte (0x00, 0xFF)
 * Postamble  : 1 byte (0x00)
 */
struct pn532_ack_frame {
    uint8_t preamble;
    uint8_t start_code_1;
    uint8_t start_code_2;
    uint8_t ack_code_1;
    uint8_t ack_code_2;
    uint8_t postamble;
};

/*
 * PN532 nack frame format:
 *
 * Preamble   : 1 byte (0x00)
 * Start Code : 2 byte (0x00, 0xFF)
 * NACK Code  : 2 byte (0xFF, 0x00)
 * Postamble  : 1 byte (0x00)
 */
struct pn532_nack_frame {
    uint8_t preamble;
    uint8_t start_code_1;
    uint8_t start_code_2;
    uint8_t nack_code_1;
    uint8_t nack_code_2;
    uint8_t postamble;
};

/*
 * PN532 error frame format:
 *
 * Preamble                              : 1 byte (0x00)
 * Start Code                            : 2 bytes (0x00, 0xFF)
 * Length                                : 1 byte (0x01)
 * Length Checksum                       : 1 byte (0xFF)
 * Specific Application Level Error Code : 1 byte (0x7F)
 * Data Checksum                         : 1 byte (0x81)
 * Postamble                             : 1 byte (0x00)
 */
struct pn532_error_frame {
    uint8_t preamble;
    uint8_t start_code_1;
    uint8_t start_code_2;
    uint8_t len;
    uint8_t lcs;
    uint8_t error_code;
    uint8_t dcs;
    uint8_t postamble;
};

struct pn532_data {
    uint32_t dummy;
};

struct pn532_config {
    const struct device *i2c_dev;
};

static int get_firmware_version(const struct device *dev, uint32_t *version)
{
    if ((dev == NULL) || (version == NULL)) {
        return -EINVAL;
    }

    *version = 0x01020304;

    return 0;
}

static DEVICE_API(pn532, pn532_api) = {
    .pn532_get_firmware_version = &get_firmware_version,
};

static int pn532_init(const struct device *dev)
{
    int ret = pn532_transport_init(dev);
    if (ret < 0) {
        LOG_ERR("Transport init failed: %d", ret);
        return ret;
    }

    LOG_INF("PN532 initialized");

    return 0;
}

#define PN532_DEFINE(inst)                             \
    static struct pn532_data data_##inst;              \
                                                       \
    static const struct pn532_config config_##inst = { \
        .i2c_dev = DEVICE_DT_GET(DT_INST_BUS(inst)),   \
    };                                                 \
                                                       \
    DEVICE_DT_INST_DEFINE(inst,                        \
                          pn532_init,                  \
                          NULL,                        \
                          &data_##inst,                \
                          &config_##inst,              \
                          POST_KERNEL,                 \
                          CONFIG_PN532_INIT_PRIORITY,  \
                          &pn532_api);

DT_INST_FOREACH_STATUS_OKAY(PN532_DEFINE)
