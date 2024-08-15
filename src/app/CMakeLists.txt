cmake_minimum_required(VERSION 3.21)

add_executable(program)
target_sources(program PRIVATE
     program.cpp
)
target_link_libraries(program PRIVATE mizui)
