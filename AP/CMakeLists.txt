cmake_minimum_required(VERSION 3.10)
project(a02)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(a02 a02.cc)
target_link_libraries(a02 ${SDL2_LIBRARIES})
