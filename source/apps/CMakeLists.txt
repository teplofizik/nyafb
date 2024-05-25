cmake_minimum_required(VERSION 3.5) # Проверка версии CMake.
project(fbtest_1)

set (CMAKE_CXX_STANDARD 17)

add_executable(fbtest_1 fbtest_1.cpp)
target_link_libraries(fbtest_1 nyafb)
