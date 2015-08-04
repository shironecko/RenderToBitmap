project(RenderToBitmap)

# compiler flags
if(MSVC)
  message("Generating for MSVC")
  set(AdditionalFlags "-W4")
elseif(MINGW)
  message("Generating for MinGW")
  set(AdditionalFlags "-std=c++11 -Wall")
else()
  message("Generating for GCC/Clang")
  set(AdditionalFlags "-std=c++11 -m64 -Wall")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${AdditionalFlags}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${AdditionalFlags}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${AdditionalFlags}")

set(SourceFiles main.cpp bitmap.cpp)

add_executable(RenderToBitmap ${SourceFiles})
