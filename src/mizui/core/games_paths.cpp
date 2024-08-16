#include <functional>
#include <fstream>

#include <core/games_paths.h>
namespace mizui::core {
    GamesPaths::GamesPaths(const vfs::Path& common) {
        searchPaths.reserve(8);
        searchPaths.push_back(common);
    }

    void GamesPaths::populate() {
        std::function<void(const std::filesystem::path&)> searchForGames = [&](const std::filesystem::path& path) {
            const std::filesystem::directory_iterator walker(path);
            if (walker == std::filesystem::directory_iterator())
                return;
            for (const auto& entry : walker) {
                if (entry.is_directory()) {
                    searchForGames(entry.path());
                }
                if (!entry.exists() || !entry.is_regular_file()) {
                    throw std::runtime_error("No such file or directory");
                }
                std::fstream access{entry.path(), std::ios::in | std::ios::binary};
                if (!access.is_open()) {
                    throw std::runtime_error("Could not open file");
                }
                access.close();
                readable.emplace_back(entry.path());
            }
        };
        for (const auto& gamePath : searchPaths) {
            const std::filesystem::path& path(gamePath);
            searchForGames(path);
        }
    }
}
