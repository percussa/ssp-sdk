cmake_minimum_required(VERSION 3.15)
project(test)

set(SRC test.cpp )

add_library(test SHARED ${SRC})

target_link_libraries(test epoxy)

