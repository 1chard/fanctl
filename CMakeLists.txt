cmake_minimum_required(VERSION 3.5)

project(fanctl)

add_executable(fanctl main.c ioconfig.c fanmanager.c fanctl.c)

set(CMAKE_C_FLAGS -lm)
set(CMAKE_C_STANDARD 11)
