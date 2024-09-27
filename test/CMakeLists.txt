# 项目名称
project(Ztest)

add_executable(test_fdnode test_fdnode.cpp)

include_directories(${THIRD_PARTY_BASE_DIR})
include_directories(${ZSRC_BASE_DIR})


target_link_libraries(test_fdnode PUBLIC zfs)

add_test(
    NAME test_fdnode
    COMMAND test_fdnode
)