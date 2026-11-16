# Host-side compile target only; this is not a Pico SDK firmware image.
cmake_minimum_required(VERSION 3.16)
project(hinge_witness_core C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_library(hinge_witness OBJECT src/hinge_witness.c)
target_compile_options(hinge_witness PRIVATE -Wall -Wextra -Werror)
