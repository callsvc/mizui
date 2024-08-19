#pragma once

#include <hle/application.h>
#include <vfs/path.h>
namespace mizui::hle {
    class VirtualNx {
    public:
        VirtualNx() = default;
        void stockEveryExecutable(const std::vector<vfs::Path>& executables);
    private:
        std::vector<Application> playable;
    };
}