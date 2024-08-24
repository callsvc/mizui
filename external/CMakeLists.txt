cmake_minimum_required(VERSION 3.21)

add_subdirectory(boost)
add_subdirectory(fmt)

set(LZ4_BUILD_CLI OFF CACHE BOOL "Build lz4 program" FORCE)
add_subdirectory(lz4/build/cmake)
