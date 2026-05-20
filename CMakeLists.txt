cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pico SDK import
include(pico_sdk_import.cmake)

project(testcoderange C CXX ASM)

# Initialize SDK
pico_sdk_init()

# Add executable
add_executable(testcoderange
    testcoderange.cpp
    VL53L0X.cpp
)

# Program metadata
pico_set_program_name(testcoderange "testcoderange")
pico_set_program_version(testcoderange "0.1")

# STDIO configuration (IMPORTANT)
pico_enable_stdio_usb(testcoderange 1)
pico_enable_stdio_uart(testcoderange 0)

# Libraries
target_link_libraries(testcoderange
    pico_stdlib
    hardware_i2c
    hardware_timer
)

# Include directories
target_include_directories(testcoderange PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Extra outputs (.uf2, .bin, etc.)
pico_add_extra_outputs(testcoderange)