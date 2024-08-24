cmake_minimum_required(VERSION 3.21)

add_library(mizui_interface INTERFACE)
target_include_directories(mizui_interface PUBLIC INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mizui>
    $<INSTALL_INTERFACE:mizui>
)

target_link_libraries(mizui_interface PUBLIC INTERFACE fmt::fmt lz4_static)

add_subdirectory(mizui)
add_subdirectory(app)
