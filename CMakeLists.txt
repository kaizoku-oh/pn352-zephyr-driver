zephyr_library()

zephyr_library_sources(
  pn532.c
)

if(CONFIG_PN532_TRANSPORT_I2C)
  zephyr_library_sources(pn532_i2c.c)
endif()

if(CONFIG_PN532_TRANSPORT_SPI)
  zephyr_library_sources(pn532_spi.c)
endif()

if(CONFIG_PN532_TRANSPORT_UART)
  zephyr_library_sources(pn532_uart.c)
endif()

zephyr_include_directories(include)
