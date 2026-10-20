find_package(GTest REQUIRED)

add_executable(bmp_tests
    color_test/color_test.cpp
    imagebmp_test/imagebmp_test.cpp
)

target_link_libraries(bmp_tests PRIVATE
    bmp_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(bmp_tests)
