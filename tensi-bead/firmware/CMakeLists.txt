cmake_minimum_required(VERSION 3.20)
project(tensi_bead_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(tensi_bead_fw_sim
    src/main.c
    src/sensors.c
    src/analysis.c
    src/ui.c
)

target_include_directories(tensi_bead_fw_sim PRIVATE include)
target_compile_options(tensi_bead_fw_sim PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(tensi_bead_fw_sim m)
