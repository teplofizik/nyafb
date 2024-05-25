cmake_minimum_required(VERSION 3.8) # Проверка версии CMake.
project(nyafb CXX)

set (CMAKE_CXX_STANDARD 17)

set (SOURCES
	framebuffer.cpp
	image.cpp
	
	images/image_png.cpp
)

set (HEADERS
	framebuffer.h
	image.h
	
	images/image_png.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} png)