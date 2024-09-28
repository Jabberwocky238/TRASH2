# 项目名称
project(zcore)

add_library(${PROJECT_NAME} STATIC 
fdnode.cc
zutils.cc
)

# 添加头文件和依赖
target_include_directories(${PROJECT_NAME} PUBLIC ${ZSRC_BASE_DIR}/zcore)
# target_link_libraries(${PROJECT_NAME} PUBLIC ${ZFS_LIBRARIES})
