cmake_minimum_required(VERSION 3.21)

if(NOT ${CMAKE_PROJECT_NAME} STREQUAL "Mizui")
    message(FATAL_ERROR "This component belongs to the Mizui project")
endif()
project(Orizonti)

add_library(orizonti STATIC)

target_sources(orizonti PUBLIC
    kernel/process.h
    fs/partition_filesystem.cpp
    fs/partition_filesystem.h
    os.cpp
    os.h
)
target_link_libraries(orizonti PUBLIC mizui_interface)

