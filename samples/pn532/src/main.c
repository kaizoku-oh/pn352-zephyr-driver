/*
 * Copyright (c) 2025 Bayrem Gharsellaoui
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdbool.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

#include "pn532.h"

/**
  * @brief  Program entry point
  * @param  None
  * @retval None
  */
int main(void)
{
  const struct device *dev = DEVICE_DT_GET_ONE(nxp_pn532);
  uint32_t fw_version = 0;

  if (!device_is_ready(dev))
  {
    LOG_INF("PN532 device not ready");
    return;
  }

  if (pn532_get_firmware_version(dev, &fw_version) == 0)
  {
    LOG_INF("PN532 Firmware Version: %02X.%02X",
            (fw_version >> 8) & 0xFF,
            fw_version & 0xFF);
  }
  else
  {
    LOG_ERR("Failed to get PN532 firmware version");
  }

  while (true)
  {
    k_msleep(500);
  }

  return EXIT_FAILURE;
}
