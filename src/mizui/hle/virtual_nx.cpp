#include <ranges>
#include <hle/virtual_nx.h>

namespace mizui::hle {
    void VirtualNx::stockEveryExecutable(const std::vector<vfs::Path>& executables) {
        for (const auto& [idx, path] :
            std::views::enumerate(executables)) {
            playable.emplace_back(idx);
        }
    }
}
