cmake_minimum_required(VERSION 3.20)
project(stoma_clip_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(stoma_clip_fw_sim
    src/main.c
    src/sensors.c
    src/porometer.c
    src/ui.c
)

target_include_directories(stoma_clip_fw_sim PRIVATE include)

target_compile_options(stoma_clip_fw_sim PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(stoma_clip_fw_sim m)
