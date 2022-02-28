cmake_minimum_required(VERSION 3.15)
project(ssp-sdk)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I ${PROJECT_SOURCE_DIR}/..")

add_subdirectory(api)
add_subdirectory(vst)
