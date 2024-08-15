#pragma once
#include <vector>

#include <vfs/path.h>
namespace mizui::core {
    class GamesPaths {
    public:
        GamesPaths() = default;
        explicit GamesPaths(const vfs::Path& common);

    private:
        std::vector<vfs::Path> searchPaths;
    };
}