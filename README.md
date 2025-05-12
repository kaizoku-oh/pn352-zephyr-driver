# Zephyr driver for the PN532 NFC module

The PN532 Zephyr driver is an out-of-tree module supporting the NXP PN532 NFC module. It (will eventually) provides a transport-agnostic (I2C, SPI, UART) interface with a custom API, selectable via Kconfig, offering functionalities such as reading and writing NFC tags, initiating peer-to-peer communication, and emulating NFC cards, with extensibility for further NFC operations.

## ✅ TODO

- [ ] Build a minimal working driver with a single API

- [ ] Add a unit test project to the module

- [ ] Add build CI + README badge

- [ ] Add test CI running on `native_sim` board + README badge

- [ ] Add code coverage CI using `codecov` + README badge

- [ ] Add release CI + README badge

- [ ] Add the other relavant missing APIs to the driver
