cmake_minimum_required(VERSION 3.20)
project(line_shepherd_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(line_shepherd_fw_sim
    src/main.c
    src/sensors.c
    src/tdr.c
    src/classifier.c
    src/line_db.c
    src/ui.c
)

target_include_directories(line_shepherd_fw_sim PRIVATE include)
target_compile_options(line_shepherd_fw_sim PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(line_shepherd_fw_sim m)
