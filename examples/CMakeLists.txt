cmake_minimum_required(VERSION 3.1)

project(examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Examples directory - ${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(mainwindow main.cpp)
target_link_libraries(mainwindow mingw32 d3d9 imgui)

