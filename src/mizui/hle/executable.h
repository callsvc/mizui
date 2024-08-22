#pragma once

#include <fstream>
#include <vfs/mappable.h>
namespace mizui::hle {
    class Executable {
    public:
        Executable() = default;
        Executable(std::fstream&& passport) :
            ownership(std::move(passport)) {}

        vfs::Mappable backing;
        std::fstream ownership;
    };
}
