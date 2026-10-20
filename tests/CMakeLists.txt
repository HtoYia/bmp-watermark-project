find_package(GTest REQUIRED)

add_executable(bmp_tests
    bmp_test.cpp
    color_test.cpp
    watermark_test.cpp
)

target_link_libraries(bmp_tests PRIVATE
    bmp_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(bmp_tests)