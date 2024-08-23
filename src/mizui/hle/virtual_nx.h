#pragma once

#include <vfs/path.h>

#include <hle/application.h>
#include <exe/executable.h>
namespace mizui::hle {
    class VirtualNx {
    public:
        VirtualNx() = default;
        void stockEveryExecutable(const std::vector<vfs::Path>& executables);
        bool loadNso(std::fstream&& handle);
        std::vector<Application> playable;

        std::unique_ptr<exe::Executable> program;
    };
}