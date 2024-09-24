# CMake最低版本要求
cmake_minimum_required(VERSION 3.5)

# 项目名称
project(MyProject)

# 设置C++标准为C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 设置vcpkg工具链文件路径
include("D:/Scoop/apps/vcpkg/current/scripts/buildsystems/vcpkg.cmake")
# 设置包含目录
set(THIRD_PARTY_BASE_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
include_directories(${THIRD_PARTY_BASE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_LIST)
# 生成可执行文件
add_executable(demo ${SRC_LIST})



