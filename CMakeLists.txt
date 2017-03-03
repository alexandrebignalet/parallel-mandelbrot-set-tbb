cmake_minimum_required(VERSION 3.6)
project(mandelbrot)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES benchmark-mandelbrot.cpp Point.cpp Point.h Mandelbrot.h Mandelbrot.cpp)
add_executable(mandelbrot ${SOURCE_FILES})