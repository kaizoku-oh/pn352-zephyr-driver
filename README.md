# Zephyr driver for the PN532 NFC module


[![GitHub Build workflow status](https://github.com/bytefull/pn532-zephyr-driver/workflows/Build/badge.svg)](https://github.com/bytefull/pn532-zephyr-driver/actions/workflows/build.yml)
[![GitHub release](https://img.shields.io/github/v/release/bytefull/pn532-zephyr-driver)](https://github.com/bytefull/pn532-zephyr-driver/releases)
[![Zephyr Compatibility](https://img.shields.io/badge/Zephyr-v4.1.0-blue)](https://github.com/zephyrproject-rtos/zephyr/releases/tag/v4.1.0)
[![GitHub issues](https://img.shields.io/github/issues/bytefull/pn532-zephyr-driver)](https://github.com/bytefull/pn532-zephyr-driver/issues)
![GitHub top language](https://img.shields.io/github/languages/top/bytefull/pn532-zephyr-driver)
[![License](https://img.shields.io/github/license/bytefull/pn532-zephyr-driver)](https://github.com/bytefull/pn532-zephyr-driver/blob/main/LICENSE)

The PN532 Zephyr driver is an out-of-tree module supporting the NXP PN532 NFC module. It (will eventually) provides a transport-agnostic (I2C, SPI, UART) interface with a custom API, selectable via Kconfig, offering functionalities such as reading and writing NFC tags, initiating peer-to-peer communication, and emulating NFC cards, with extensibility for further NFC operations.

## ✅ TODO

- [x] Build a minimal working driver with a single API

- [x] Add build CI + README badge

- [x] Add release CI + README badge

- [ ] Add a unit test project to the module

- [ ] Add test CI running on `native_sim` board + README badge

- [ ] Add code coverage CI using `codecov` + README badge

- [ ] Add the other relavant missing APIs to the driver
