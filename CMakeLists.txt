cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(vgfx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directory where exe,dll and static library saved
set(VGFX_BINARY_DIR ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VGFX_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VGFX_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${VGFX_BINARY_DIR})

message(STATUS "Project directory - ${CMAKE_CURRENT_SOURCE_DIR}")

#Variables
set(PWINDOW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3dparty/pwindow)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3dparty/imgui)

#Includes
include_directories(${PWINDOW_DIR}/include)
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)

#Imgui
add_library(imgui ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx9.cpp ${IMGUI_DIR}/backends/imgui_impl_win32.cpp)

target_link_libraries(imgui dwmapi)

# Examples
add_subdirectory(examples pwindow)

