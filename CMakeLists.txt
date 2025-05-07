# Defining top-level build rules for PN532 driver module
cmake_minimum_required(VERSION 3.20.0)
project(pn532_driver)

# Adding driver source directory
add_subdirectory(drivers/nfc/pn532)