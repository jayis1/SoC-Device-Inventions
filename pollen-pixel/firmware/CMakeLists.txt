cmake_minimum_required(VERSION 3.20)
project(pollen_pixel_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(pollen_pixel_fw_sim
    src/main.c
    src/pipeline.c
    src/classifier.c
    src/flow.c
)

target_include_directories(pollen_pixel_fw_sim PRIVATE include)

target_compile_options(pollen_pixel_fw_sim PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(pollen_pixel_fw_sim m)
