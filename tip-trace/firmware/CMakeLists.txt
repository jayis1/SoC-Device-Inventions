# Portable core syntax target; adapter/build integration for Pico SDK remains future work.
cmake_minimum_required(VERSION 3.16)
project(tip_trace_core C)
add_library(tip_trace_core STATIC src/main.c)
target_compile_features(tip_trace_core PRIVATE c_std_11)
target_compile_options(tip_trace_core PRIVATE -Wall -Wextra -Werror)
