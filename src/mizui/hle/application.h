#pragma once

#include <string>
#include <vector>

#include <decl.h>
namespace mizui::hle {
    struct Application {
        u64 playId;
        u64 handle;
        std::string title;
        std::vector<u8> icon;
    };

}