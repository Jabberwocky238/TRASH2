# 项目名称
project(zcore_test)

add_executable(${PROJECT_NAME} test_fdnode.cpp)

include_directories(${THIRD_PARTY_BASE_DIR})
include_directories(${ZSRC_BASE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC zcore)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)