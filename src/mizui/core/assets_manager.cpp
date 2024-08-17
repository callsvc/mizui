#include <ranges>
#include <core/assets_manager.h>

namespace mizui::core {
    void AssetsManager::initialize(const vfs::Path& baseDir) {
        root = baseDir;

        logs = root / "logs";
        keys = root / "keys";
        games = root / "games";

        validatePath(logs);
        validatePath(keys);
        validatePath(games);

        collection = GamesPaths(games);
        collection.populate();

        const auto& every{collection.readable};
        for (const auto& [idx, path] :
            std::views::enumerate(every)) {
            playable.emplace_back(idx);
        }
    }

    void AssetsManager::validatePath(const vfs::Path& path) const {
        const std::filesystem::path& stdPath{path};
        if (stdPath.parent_path() != std::filesystem::path{root}) {
            throw std::runtime_error("Parent directory does not exists");
        }
        if (!exists(stdPath)) {
            create_directories(stdPath);
        }
    }
}
