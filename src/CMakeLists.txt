# 项目名称
project(ZMain)

aux_source_directory(. SRC_LIST)
add_subdirectory(zfs)

add_executable(demo main.cpp ${SRC_LIST})

# 添加头文件和依赖
target_link_libraries(demo PUBLIC zfs)