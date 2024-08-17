#pragma once

#include <core/games_paths.h>
#include <decl.h>
namespace mizui::core {
    struct NxApplication {
        u64 playId;
        std::string title;
        std::vector<u8> icon;
    };

    class AssetsManager {
    public:
        AssetsManager() = default;
        void initialize(const vfs::Path& baseDir);

        vfs::Path root;
        vfs::Path logs;
        vfs::Path keys;
        vfs::Path games; // Default path for the games
    private:
        void validatePath(const vfs::Path& path) const;

        GamesPaths collection;
        std::vector<NxApplication> playable;
    };
}
