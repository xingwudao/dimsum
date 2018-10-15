# 头文件路径
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/boost-1.50.0/include/")


# 库文件目录定义
set (BOOST_LIBS ${CMAKE_SOURCE_DIR}/thirdparty/boost-1.50.0/lib/)

# 制定库文件搜索目录
link_directories (${BOOST_LIBS} )

# 可执行文件名字
add_executable (compute-row-similarity compute-row-similarity.cc)

# 链接库文件
target_link_libraries (compute-row-similarity libboost_program_options.a)
