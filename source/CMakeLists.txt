cmake_minimum_required(VERSION 2.8)

project(RenderToBitmap CXX)

# compiler flags
if(MSVC)
  message("Generating for MSVC")
  set(AdditionalFlags "-W4")
  set(AdditionalDebugFlags "-Zi")
elseif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message("Generating for GCC/Clang")
  set(AdditionalFlags "-std=c++11 -m64 -Wall")
  set(AdditionalDebugFlags "-ggdb3 -pg")
else()
  message(FATAL_ERROR "Only MSVC and GNUCXX-type compilers are supported at the moment.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${AdditionalFlags}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${AdditionalFlags} ${AdditionalDebugFlags}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${AdditionalFlags}")

set(SourceFiles main.cpp bitmap.cpp mesh.cpp vec.cpp)
set(HeaderFiles types.h bitmap.h mesh.h vec.h matrix.h)

add_executable(RenderToBitmap ${SourceFiles} ${HeaderFiles})
