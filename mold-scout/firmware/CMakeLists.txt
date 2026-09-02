cmake_minimum_required(VERSION 3.16)
project(mold_scout_reference C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(mold_scout
    src/main.c
    src/sensors.c
    src/dsp.c
    src/classifier.c
)

target_link_libraries(mold_scout m)
