# Defining build rules for PN532 driver
zephyr_library()
zephyr_library_sources(pn532.c)
zephyr_library_sources_ifdef(CONFIG_PN532_I2C pn532_i2c.c)
zephyr_library_sources_ifdef(CONFIG_PN532_SPI pn532_spi.c)
zephyr_library_sources_ifdef(CONFIG_PN532_UART pn532_uart.c)
zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)