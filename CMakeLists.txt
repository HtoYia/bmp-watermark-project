cmake_minimum_required(VERSION 3.24)

project(BMPWatermark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bmp_lib
    src/bmp.cpp
    src/watermark.cpp
)

target_include_directories(bmp_lib PUBLIC include)

add_executable(bmp_watermark src/main.cpp)
target_link_libraries(bmp_watermark PRIVATE bmp_lib)

enable_testing()
add_subdirectory(tests)