cmake_minimum_required(VERSION 3.21)
project(ImageProcessor C)

set(CMAKE_C_STANDARD 99)

add_executable(ImageProcessor src/GutierrezImageProcessor.c src/PixelProcessor.c src/BmpProcessor.c src/PpmProcessor.c)