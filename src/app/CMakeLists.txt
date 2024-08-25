cmake_minimum_required(VERSION 3.21)

add_executable(mizterm)
target_sources(mizterm PRIVATE
    mizui.cpp
)
target_link_libraries(mizterm PRIVATE mizui)
