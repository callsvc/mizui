#include <core/games_paths.h>

namespace mizui::core {
    GamesPaths::GamesPaths(const vfs::Path& common) {
        searchPaths.reserve(8);
        searchPaths.push_back(common);
    }
}
