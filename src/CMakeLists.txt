cmake_minimum_required(VERSION 3.21)

add_library(mizui STATIC)
add_library(mizui_interface INTERFACE)

target_include_directories(mizui_interface PUBLIC INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:>
)
target_link_libraries(mizui_interface PUBLIC INTERFACE fmt::fmt lz4_static)
add_subdirectory(app)

target_sources(mizui PUBLIC
    common/decl.h
    common/magic.h
    common/modifiers.h
    core/application.cpp
    core/application.h
    core/assets_manager.cpp
    core/assets_manager.h
    core/games_paths.cpp
    core/games_paths.h
    crypt/platform_keys.cpp
    crypt/platform_keys.h
    hle/virtual_nx.cpp
    hle/virtual_nx.h
    hle/application.h
    exe/executable.cpp
    exe/executable.h
    exe/nso.cpp
    exe/nso.h
    exe/nsp.cpp
    exe/nsp.h
    conf/global.cpp
    conf/global.h
    conf/yaml.cpp
    conf/yaml.h
    vfs/ranged.h
    vfs/mappable.cpp
    vfs/mappable.h
    vfs/path.h
    vfs/rofs.cpp
    vfs/rofs.h
)

add_subdirectory(orizonti)
target_link_libraries(mizui PUBLIC mizui_interface orizonti Boost::program_options Boost::algorithm)
