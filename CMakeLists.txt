cmake_minimum_required(VERSION 3.5) # Проверка версии CMake.
project(fbtest)

set (CMAKE_CXX_STANDARD 17)

include_directories(source/)

add_subdirectory(source/nyafb)
add_subdirectory(source/apps)
