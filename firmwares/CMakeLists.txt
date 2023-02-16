cmake_minimum_required(VERSION 3.13)

# default o-level is 3, which is difficult to work with 
set(CMAKE_CXX_FLAGS_RELEASE "-O1")
# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

# who's project it would be if not mine? 
project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(flag main.c)
add_executable(hello hello.c)
add_executable(glitch_detect glitch_detect.c)

target_link_libraries(flag pico_stdlib)
target_link_libraries(glitch_detect pico_stdlib)
target_link_libraries(hello pico_stdlib)

pico_enable_stdio_uart(flag 0)
pico_enable_stdio_usb(flag 1)
pico_enable_stdio_uart(glitch_detect 0)
pico_enable_stdio_usb(glitch_detect 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(flag)
pico_add_extra_outputs(hello)
pico_add_extra_outputs(glitch_detect)

