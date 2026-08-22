cmake_minimum_required(VERSION 3.16)
project(pipe_halo_demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(pipe_halo_demo
    src/main.c
    src/sensors.c
    src/dsp.c
    src/classifier.c
)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(pipe_halo_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
