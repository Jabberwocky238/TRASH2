# 项目名称
project(zconsole)

add_library(${PROJECT_NAME} STATIC 
prompt.cc
zconsole.cc
)

# 添加头文件和依赖
target_include_directories(${PROJECT_NAME} PUBLIC ${ZSRC_BASE_DIR}/zconsole)
target_include_directories(${PROJECT_NAME} PUBLIC ${ZSRC_BASE_DIR}/zcore)

target_link_libraries(${PROJECT_NAME} PUBLIC zcore)
