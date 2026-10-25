cmake_minimum_required(VERSION 3.13)
# Portable algorithm-core project. For Pico SDK integration, provide pico_sdk_import.cmake
# from the pinned Raspberry Pi Pico SDK v2.x checkout, then add_executable and link pico_stdlib.
project(spool_scout_core C)
set(CMAKE_C_STANDARD 11)
add_library(spool_scout_core STATIC main.c)
target_compile_options(spool_scout_core PRIVATE -Wall -Wextra -Werror)
