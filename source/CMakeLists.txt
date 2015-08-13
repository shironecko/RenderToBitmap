project(RenderToBitmap)

# compiler flags
if(MSVC)
  message("Generating for MSVC")
  set(AdditionalFlags "-W4 -Zi")
elseif(MINGW)
  message("Generating for MinGW")
  set(AdditionalFlags "-std=c++11 -Wall -ggdb3")
else()
  message("Generating for GCC/Clang")
  set(AdditionalFlags "-std=c++11 -m64 -Wall -ggdb3")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${AdditionalFlags}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${AdditionalFlags}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${AdditionalFlags}")

set(SourceFiles main.cpp bitmap.cpp mesh.cpp vec.cpp)
set(HeaderFiles types.h bitmap.h mesh.h vec.h)

add_executable(RenderToBitmap ${SourceFiles} ${HeaderFiles})
