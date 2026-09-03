cmake_minimum_required(VERSION 3.16)
project(torque_pebble_demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(torque_pebble_demo
    src/main.c
    src/sensors.c
    src/analysis.c
    src/classifier.c
)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(torque_pebble_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
