# 设置包含目录
include_directories(${CMAKE_SOURCE_DIR})
find_package(nlohmann_json CONFIG REQUIRED)

# 添加可执行文件
add_executable(MyExecutable 
    main.cpp 
    core.cpp
)

target_link_libraries(MyExecutable PRIVATE nlohmann_json::nlohmann_json)

# 添加库文件（如果有的话）
# add_library(MyLibrary STATIC lib_source.cpp)
