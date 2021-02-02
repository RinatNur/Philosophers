cmake_minimum_required(VERSION 3.17)
project(philosophers C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(philosophers
        main.c
        utils.c
        print_utils.c
        feast.c
        philosophers.h)
