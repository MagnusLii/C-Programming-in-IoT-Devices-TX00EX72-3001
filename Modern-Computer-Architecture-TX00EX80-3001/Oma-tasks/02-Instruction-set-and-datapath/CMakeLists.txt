cmake_minimum_required(VERSION 3.22.0)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(project_xyz VERSION 1.0 LANGUAGES C CXX ASM)

pico_sdk_init()

#set(source_location ${CMAKE_CURRENT_LIST_DIR}/src)

#include_directories(${CMAKE_CURRENT_LIST_DIR}/lib)


add_executable(${PROJECT_NAME} main.c)
#add_library(led          ${source_location}/led.c)

pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c)
#target_link_libraries(led             pico_stdlib hardware_pwm)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)