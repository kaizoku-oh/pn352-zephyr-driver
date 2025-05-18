/*
 * Copyright (c) 2025 Bayrem Gharsellaoui
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT nxp_pn532

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/emul.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/i2c_emul.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(pn532, CONFIG_PN532_LOG_LEVEL);

#include "pn532.h"
#include "pn532_transport.h"

struct pn532_data {
    uint32_t dummy;
};

#ifdef CONFIG_TEST
struct pn532_emul_cfg {
    uint16_t addr;
};
#else
struct pn532_config {
    const struct device *i2c_dev;
};
#endif /* CONFIG_TEST */

static int get_firmware_version(const struct device *dev, uint32_t *version)
{
    if ((dev == NULL) || (version == NULL)) {
        return EINVAL;
    }

    *version = 0x01020304;

    return 0;
}

static DEVICE_API(pn532, pn532_api) = {
    .pn532_get_firmware_version = &get_firmware_version,
};

static int emul_pn532_reg_write(const struct emul *target, int reg, int val)
{

	return -EIO;
}

static int emul_pn532_reg_read(const struct emul *target, int reg, int *val)
{

	switch (reg) {
	case REGISTER_VERSION:
	*val = 0x1000;
		break;
	case REGISTER_CRATE:
	*val = crate_value;
	break;
	case REGISTER_SOC:
	*val = 0x3525;
		break;
	case REGISTER_VCELL:
	*val = 0x4387;
		break;
	default:
		LOG_ERR("Unknown register 0x%x read", reg);
		return -EIO;
	}
	LOG_INF("read 0x%x = 0x%x", reg, *val);

	return 0;
}

static int pn532_transfer_i2c(const struct emul *target, struct i2c_msg *msgs,
				       int num_msgs, int addr)
{
	/* Largely copied from emul_bmi160.c */
	unsigned int val;
	int reg;
	int rc;

	__ASSERT_NO_MSG(msgs && num_msgs);

	i2c_dump_msgs_rw(target->dev, msgs, num_msgs, addr, false);
	switch (num_msgs) {
	case 2:
		if (msgs->flags & I2C_MSG_READ) {
			LOG_ERR("Unexpected read");
			return -EIO;
		}
		if (msgs->len != 1) {
			LOG_ERR("Unexpected msg0 length %d", msgs->len);
			return -EIO;
		}
		reg = msgs->buf[0];

		/* Now process the 'read' part of the message */
		msgs++;
		if (msgs->flags & I2C_MSG_READ) {
			switch (msgs->len - 1) {
			case 1:
				rc = emul_pn532_reg_read(target, reg, &val);
				if (rc) {
					/* Return before writing bad value to message buffer */
					return rc;
				}

				/* SBS uses SMBus, which sends data in little-endian format. */
				sys_put_le16(val, msgs->buf);
				break;
			default:
				LOG_ERR("Unexpected msg1 length %d", msgs->len);
				return -EIO;
			}
		} else {
			/* We write a word (2 bytes by the SBS spec) */
			if (msgs->len != 2) {
				LOG_ERR("Unexpected msg1 length %d", msgs->len);
			}
			uint16_t value = sys_get_le16(msgs->buf);

			rc = emul_pn532_reg_write(target, reg, value);
		}
		break;
	default:
		LOG_ERR("Invalid number of messages: %d", num_msgs);
		return -EIO;
	}

	return rc;
}

static const struct i2c_emul_api pn532_emul_api_i2c = {
	.transfer = pn532_transfer_i2c,
};

/**
 * Set up a new emulator (I2C)
 *
 * @param emul Emulation information
 * @param parent Device to emulate
 * @return 0 indicating success (always)
 */
static int emul_pn532_init(const struct emul *target, const struct device *parent)
{
	ARG_UNUSED(target);
	ARG_UNUSED(parent);

	return 0;
}

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

#ifdef CONFIG_TEST

#define PN532_EMUL(n)                                         \
    static struct pn532_data data##n;                         \
                                                                 \
    static const struct pn532_emul_cfg pn532_emul_cfg_##n = { \
        .addr = DT_INST_REG_ADDR(n),                          \
    };                                                           \
                                                                 \
    EMUL_DT_INST_DEFINE(n, emul_pn532_init, NULL,           \
                        &pn532_emul_cfg_##n, &pn532_emul_api_i2c, NULL)

DT_INST_FOREACH_STATUS_OKAY(PN532_EMUL)

#else

#define PN532_DEFINE(inst)                                         \
    static struct pn532_data data##inst;                           \
                                                                   \
    static const struct pn532_config config##inst = {              \
        .i2c_dev = DEVICE_DT_GET(DT_INST_BUS(inst)),               \
    };                                                             \
                                                                   \
    DEVICE_DT_INST_DEFINE(inst, pn532_init, NULL,                  \
                          &data##inst, &config##inst,              \
                          POST_KERNEL, CONFIG_PN532_INIT_PRIORITY, \
                          &pn532_api);

DT_INST_FOREACH_STATUS_OKAY(PN532_DEFINE)

#endif /* CONFIG_TEST */
