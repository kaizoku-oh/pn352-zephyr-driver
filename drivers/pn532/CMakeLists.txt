# Copyright (c) 2025 Bayrem Gharsellaoui
# SPDX-License-Identifier: Apache-2.0

zephyr_library()

zephyr_include_directories(../../include/zephyr/drivers/pn532)

zephyr_syscall_header(../../include/zephyr/drivers/pn532/pn532.h)

zephyr_library_sources(pn532.c)

if(CONFIG_PN532_TRANSPORT_MOCK)
  zephyr_library_sources(pn532_transport_mock.c)
else()
  zephyr_library_sources(pn532_transport_i2c.c)
endif()
