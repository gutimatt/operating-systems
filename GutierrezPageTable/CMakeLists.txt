cmake_minimum_required(VERSION 3.21)
project(GutierrezPageTable C)

set(CMAKE_C_STANDARD 99)

add_executable(GutierrezPageTable Simulator.c GutierrezDataLoader.c GutierrezPageTable.c)
