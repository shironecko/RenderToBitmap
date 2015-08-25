cmake_minimum_required(VERSION 2.8)

project(RenderToBitmap CXX)

set(SourceFiles main.cpp bitmap.cpp mesh.cpp graphics.cpp)
set(HeaderFiles bitmap.h mesh.h graphics.h utility.h)

add_subdirectory(math)

add_executable(RenderToBitmap ${SourceFiles} ${HeaderFiles})
target_link_libraries(RenderToBitmap MathLibrary)

set_property(TARGET RenderToBitmap PROPERTY CXX_STANDART 11)
set_property(TARGET RenderToBitmap PROPERTY CXX_STANDART_REQUIRED ON)
