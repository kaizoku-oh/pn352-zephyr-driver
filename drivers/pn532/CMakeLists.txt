zephyr_include_directories(../../include/zephyr/drivers/pn532)
zephyr_library()
zephyr_library_sources(pn532.c)
zephyr_syscall_header(${ZEPHYR_PN532_MODULE_DIR}/drivers/pn532/pn532.h)
