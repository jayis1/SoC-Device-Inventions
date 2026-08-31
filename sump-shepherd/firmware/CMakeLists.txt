cmake_minimum_required(VERSION 3.20)
project(sump_shepherd_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(sump_shepherd_fw_sim
    src/main.c
    src/sensors.c
    src/classifier.c
    src/ui.c
)

target_include_directories(sump_shepherd_fw_sim PRIVATE include)
target_compile_options(sump_shepherd_fw_sim PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(sump_shepherd_fw_sim m)
