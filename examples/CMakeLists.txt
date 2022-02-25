cmake_minimum_required(VERSION 3.15)
project(ssp-sdk-api)


if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message("apple config")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (CMAKE_CROSSCOMPILING)
        message("cross compile config")
    else ()
        set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
        set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")
    endif ()
endif ()

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(test)
