# Add this directory and the WCH EVT library to the CH32V003 vendor project.
# The vendor library/toolchain are intentionally not vendored here.
cmake_minimum_required(VERSION 3.20)
project(bounce_bench C)
add_executable(bounce_bench main/main.c)
target_include_directories(bounce_bench PRIVATE main ${WCH_EVT_INCLUDE_DIR})
target_link_libraries(bounce_bench ${WCH_EVT_LIBRARIES})
