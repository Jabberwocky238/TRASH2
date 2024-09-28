# 项目名称
project(zconsole_test)

add_executable(${PROJECT_NAME} test.cpp)

include_directories(${THIRD_PARTY_BASE_DIR})
include_directories(${ZSRC_BASE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC zconsole)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)