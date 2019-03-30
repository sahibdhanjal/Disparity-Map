cmake_minimum_required(VERSION 3.7)
project(DisparityMap)

find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp include/disparity.cpp include/disparity.h)

add_executable(disparity ${SOURCE_FILES})
target_link_libraries(disparity ${OpenCV_LIBS} )
