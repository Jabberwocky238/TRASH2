# CMake最低版本要求
cmake_minimum_required(VERSION 3.5)

# 项目名称
project(MyProject)

# 设置C++标准为C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置vcpkg工具链文件路径
include("D:/Scoop/apps/vcpkg/current/scripts/buildsystems/vcpkg.cmake")

find_package(nlohmann_json CONFIG REQUIRED)


# 添加执行文件
add_executable(MyExecutable src/main.cpp)

target_link_libraries(MyExecutable PRIVATE nlohmann_json::nlohmann_json)
