# Copyright (c) 2025 Bayrem Gharsellaoui
# SPDX-License-Identifier: Apache-2.0

message(STATUS "ZEPHYR_PN532_MODULE_DIR = ${ZEPHYR_PN532_MODULE_DIR}")

zephyr_library()
# zephyr_include_directories(${ZEPHYR_PN532_MODULE_DIR}/include/zephyr/drivers/pn532)
zephyr_syscall_header(${ZEPHYR_PN532_MODULE_DIR}/include/zephyr/drivers/pn532/pn532.h)
zephyr_library_sources(pn532.c)
