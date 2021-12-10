cmake_minimum_required(VERSION 3.21)
project(cs201 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(cs201
        file-tests.c
        fileops.jdiscipi.c
        fileops.jdiscipi.h)
