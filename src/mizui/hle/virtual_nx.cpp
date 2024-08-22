#include <ranges>
#include <unistd.h>
#include <sys/stat.h>

#include <hle/virtual_nx.h>
namespace mizui::hle {
    void VirtualNx::stockEveryExecutable(const std::vector<vfs::Path>& executables) {
        for (const auto& [idx, path] :
            std::views::enumerate(executables)) {
            const std::filesystem::path& local{path};
            bool duplicate{};
#if defined(__linux__)
            struct stat64 ioStatus{};
            stat64(local.c_str(), &ioStatus);
            if (ioStatus.st_mode & S_IFLNK) {
                std::vector<char> followLink(100);
                readlink(local.c_str(), &followLink[0], followLink.size());
                stat64(&followLink[0], &ioStatus);
            }

            for (const auto& unique : playable) {
                if (unique.handle == ioStatus.st_ino)
                    duplicate = true;
                if (duplicate)
                    break;
            }
#endif
            if (!duplicate)
                playable.emplace_back(idx);
        }
    }
}
