# 项目名称
project(ZMain)

aux_source_directory(. SRC_LIST)

add_executable(demo main.cpp ${SRC_LIST})

# 添加头文件和依赖
add_subdirectory(zcore)
add_subdirectory(zconsole)

target_link_libraries(demo PUBLIC zcore)
target_link_libraries(demo PUBLIC zconsole)
