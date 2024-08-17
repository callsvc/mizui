#pragma once
#include <vector>

#include <vfs/path.h>
namespace mizui::core {
    class GamesPaths {
    public:
        GamesPaths() = default;
        explicit GamesPaths(const vfs::Path& common);

        void populate();
        std::vector<vfs::Path> readable;
    private:
        std::vector<vfs::Path> searchPaths;
    };
}