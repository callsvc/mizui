cmake_minimum_required(VERSION 3.21)

add_library(include INTERFACE)
target_include_directories(include PUBLIC INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mizui>
    $<INSTALL_INTERFACE:mizui>
)
add_subdirectory(mizui)
add_subdirectory(app)
