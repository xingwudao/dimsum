# cmake 最低版本要求
cmake_minimum_required(VERSION 2.8)

# 工程名字
project (DIMSUM)


if(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
endif()

set (DIMSUM_VERSION_MAJOR 1)
set (DIMSUM_VERSION_MINOR 0)
configure_file (
    "${PROJECT_SOURCE_DIR}/dimsum_config.h.in"
    "${PROJECT_BINARY_DIR}/dimsum_config.h"
    )

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# 子目录
add_subdirectory (src)
