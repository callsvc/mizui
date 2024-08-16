#pragma once
#include <vector>

#include <vfs/path.h>
namespace mizui::core {
    class GamesPaths {
    public:
        GamesPaths() = default;
        explicit GamesPaths(const vfs::Path& common);

        void populate();
    private:
        std::vector<vfs::Path> searchPaths;
        std::vector<vfs::Path> readable;
    };
}