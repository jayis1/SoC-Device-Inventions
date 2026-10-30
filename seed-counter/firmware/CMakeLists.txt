# Pico SDK integration is not included in this reference core.
cmake_minimum_required(VERSION 3.13)
project(seed_counter_core C)
add_library(seed_counter_core src/seed_counter.c)
target_include_directories(seed_counter_core PUBLIC src)
target_compile_features(seed_counter_core PUBLIC c_std_11)
