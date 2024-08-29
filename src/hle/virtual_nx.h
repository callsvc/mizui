#pragma once

#include <vfs/path.h>

#include <hle/application.h>
#include <exe/executable.h>
#include <crypt/platform_keys.h>

namespace mizui::core {
    class AssetsManager;
}
namespace mizui::hle {
    class VirtualNx {
    public:
        VirtualNx() = default;
        VirtualNx(const core::AssetsManager& assets);

        void stockEveryExecutable(const std::vector<vfs::Path>& executables);
        bool loadExecutable(exe::ExecutableFormat format, std::fstream&& handle);
        std::vector<Application> playable;

        crypt::PlatformKeys keysDb{};
    private:
        std::unique_ptr<exe::Executable> program;
    };
}