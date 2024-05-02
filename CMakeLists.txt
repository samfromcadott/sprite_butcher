cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)

project(sprite_packer)

set(source_code
	src/main.cc
)

include_directories(thirdparty/rectpack2D/src)

add_executable(sprite_packer ${source_code})

target_sources(sprite_packer
	PUBLIC
	FILE_SET cxx_modules TYPE CXX_MODULES
	FILES src/packing.cc
)
