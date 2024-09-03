cmake_minimum_required(VERSION 3.21)

add_subdirectory(boost)
add_subdirectory(fmt)

set(LZ4_BUILD_CLI OFF CACHE BOOL "Build lz4 program" FORCE)
add_subdirectory(lz4/build/cmake)

set(ENABLE_PROGRAMS OFF CACHE BOOL "Build Mbed TLS programs." FORCE)
set(ENABLE_TESTING OFF CACHE BOOL "Build Mbed TLS tests" FORCE)
set(UNSAFE_BUILD ON CACHE BOOL "Allow unsafe builds. These builds ARE NOT SECURE." FORCE)
add_subdirectory(mbedtls)

