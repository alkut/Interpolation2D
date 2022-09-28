cmake_minimum_required(VERSION 3.23)
project(6sem_3d_so C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(6sem_3d_so
        Common.c
        Common.h
        HermitInterpolation.c
        HermitInterpolation.h
        interpolation2.c
        interpolation2.h
        main.c
        TestFunctions.c
        TestFunctions.h CubicSpline.c CubicSpline.h)

target_link_libraries(6sem_3d_so m)
