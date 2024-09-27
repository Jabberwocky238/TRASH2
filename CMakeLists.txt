# CMake最低版本要求
cmake_minimum_required(VERSION 3.5)

# 项目名称
project(MyProject VERSION 0.0.1 LANGUAGES CXX)
# include(CTest)
enable_testing()

# 设置C++标准为C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 设置包含目录
set(THIRD_PARTY_BASE_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
set(ZSRC_BASE_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${THIRD_PARTY_BASE_DIR})
include_directories(${ZSRC_BASE_DIR})

# 添加子目录
add_subdirectory(src)
add_subdirectory(test)

