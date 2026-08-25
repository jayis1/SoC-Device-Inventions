cmake_minimum_required(VERSION 3.20)
project(trap_sage_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(trap_sage_fw_sim
    src/main.c
    src/sensors.c
    src/dsp.c
    src/thermals.c
    src/classifier.c
    src/ui.c
)

target_include_directories(trap_sage_fw_sim PRIVATE include)
target_compile_options(trap_sage_fw_sim PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(trap_sage_fw_sim m)
