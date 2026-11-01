cmake_minimum_required(VERSION 3.13)
project(crease_meter C)
set(CMAKE_C_STANDARD 11)
add_executable(crease_meter_test main.c)
target_compile_definitions(crease_meter_test PRIVATE CREASE_METER_HOST_TEST)
enable_testing()
add_test(NAME core COMMAND crease_meter_test)
# Pico SDK firmware adapter intentionally not included in this reference core.
