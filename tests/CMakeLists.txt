cmake_minimum_required(VERSION 3.16)
set(TEST_SRCS
    main.cpp
    httpclienttest.cpp
    httpclienttest.h
    )
add_executable(tests
  ${TEST_SRCS}
)
set(TEST_SRCS
    )
find_package(Qt6 REQUIRED COMPONENTS Core Test)
target_link_libraries(tests PRIVATE
    gtest
    gtest_main
    VTXFramework
    Qt6::Test)
