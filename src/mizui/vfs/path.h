#pragma once
#include <filesystem>
#include <string>

namespace mizui::vfs {
    class Path {
    public:
        Path() = default;
        Path(const std::string& path) :
            osPath(path) {}
        Path(const std::filesystem::path& path) : osPath(path) {}
        explicit operator std::string() const {
            return osPath;
        }
        explicit operator std::filesystem::path() const {
            return osPath;
        }
        Path operator/(const std::string& path) const {
            return Path{osPath + "/" + path};
        }
        explicit operator bool() const {
            return !osPath.empty();
        }
    private:
        std::string osPath;
    };
}
