#pragma once
#include <filesystem>
#include <string>

namespace mizui::vfs {
    class Path {
    public:
        Path() = default;
        Path(const std::string& path) :
            osPath(path) {
            if (osPath.has_root_path()) {
                if (!exists(osPath)) {
                }
            }
        }
        Path(const std::filesystem::path& path) : osPath(path) {}
        explicit operator std::string() const {
            return osPath;
        }
        explicit operator const std::filesystem::path&() const {
            return osPath;
        }
        Path operator / (const std::string& path) const {
            return Path{osPath.string() + "/" + path};
        }
        auto operator ==(const std::string& path) const {
            return osPath == path;
        }
        explicit operator bool() const {
            return !osPath.empty();
        }
    private:
        std::filesystem::path osPath;
    };
}
