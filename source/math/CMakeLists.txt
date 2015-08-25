cmake_minimum_required(VERSION 2.8)

project(MathLibrary CXX)

set(SourceFiles vec.cpp matrix.cpp)
set(HeaderFiles types.h vec.h matrix.h)

add_library(MathLibrary ${SourceFiles} ${HeaderFiles})

set_property(TARGET MathLibrary PROPERTY CXX_STANDART 11)
set_property(TARGET MathLibrary PROPERTY CXX_STANDART_REQUIRED ON)
