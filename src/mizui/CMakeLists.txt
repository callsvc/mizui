cmake_minimum_required(VERSION 3.21)

add_library(mizui STATIC)
target_sources(mizui PUBLIC
    decl.h
    core/application.cpp
    core/application.h
    core/assets_manager.cpp
    core/assets_manager.h
    core/games_paths.cpp
    core/games_paths.h
    hle/virtual_nx.cpp
    hle/virtual_nx.h
    hle/application.h
    exe/executable.cpp
    exe/executable.h
    exe/nso/nso.cpp
    exe/nso/nso.h
    conf/global.cpp
    conf/global.h
    conf/yaml.cpp
    conf/yaml.h
    vfs/mappable.cpp
    vfs/mappable.h
    vfs/path.h
)

add_subdirectory(orizonti)
target_link_libraries(mizui PUBLIC mizui_interface orizonti Boost::program_options Boost::algorithm)
