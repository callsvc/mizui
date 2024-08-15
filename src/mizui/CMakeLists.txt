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

    cfg/global.cpp
    cfg/global.h
    cfg/yaml.cpp
    cfg/yaml.h

    vfs/path.h
)

add_subdirectory(orizonti)
target_link_libraries(mizui PUBLIC orizonti fmt Boost::program_options Boost::algorithm)
