/*
 * Copyright (c) 2025 Bayrem Gharsellaoui
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT nxp_pn532

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/emul.h>
#include <zephyr/drivers/i2c_emul.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(emul_pn532, CONFIG_PN532_LOG_LEVEL);

struct pn532_emul_cfg {
    uint16_t addr;
};

static int pn532_transfer_i2c(const struct emul *target, struct i2c_msg *msgs, int num_msgs, int addr)
{
    ARG_UNUSED(target);
    ARG_UNUSED(msgs);
    ARG_UNUSED(num_msgs);
    ARG_UNUSED(addr);

    return 0;
}

static const struct i2c_emul_api pn532_emul_api_i2c = {
    .transfer = pn532_transfer_i2c,
};

static int emul_pn532_init(const struct emul *target, const struct device *parent)
{
    ARG_UNUSED(target);
    ARG_UNUSED(parent);

    return 0;
}

#define PN532_EMUL(n)                                         \
    static const struct pn532_emul_cfg pn532_emul_cfg_##n = { \
        .addr = DT_INST_REG_ADDR(n),                          \
    };                                                        \
                                                              \
    EMUL_DT_INST_DEFINE(n,                                    \
                        emul_pn532_init,                      \
                        NULL,                                 \
                        &pn532_emul_cfg_##n,                  \
                        &pn532_emul_api_i2c,                  \
                        NULL)

DT_INST_FOREACH_STATUS_OKAY(PN532_EMUL)
