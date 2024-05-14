cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)

project(sprite_packer)

# Building the main program
include_directories(thirdparty/rectpack2D/src)
include_directories(thirdparty/lodepng)
include_directories(thirdparty/cli11/include)

add_executable(sprite_packer src/main.cc)

target_sources(sprite_packer
	PUBLIC
	FILE_SET cxx_modules TYPE CXX_MODULES
	FILES src/packing.cc src/image.cc thirdparty/lodepng/lodepng.cpp
)

# Building the viewer
add_subdirectory(thirdparty/raylib)
add_executable(viewer src/viewer.cc)
target_link_libraries(viewer raylib)
