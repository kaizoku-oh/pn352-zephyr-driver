# Copyright (c) 2025 Bayrem Gharsellaoui
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(pn532)
zephyr_include_directories(../../../include/zephyr/drivers/pn532)
target_sources(testbinary PRIVATE main.c)
