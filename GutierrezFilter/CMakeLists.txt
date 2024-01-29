cmake_minimum_required(VERSION 3.21)
project(GutierrezFilter C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(GutierrezFilter
        src/GutierrezFilters.c
        src/BmpProcessor.c
        src/PixelProcessor.c
        )
target_link_libraries(GutierrezFilter m)
