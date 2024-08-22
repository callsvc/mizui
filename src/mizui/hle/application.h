#pragma once

#include <string>
#include <vector>

#include <decl.h>
#include <vfs/path.h>
namespace mizui::hle {
    struct Application {
        u64 playId;
        u64 handle;
        vfs::Path ios;
        std::string title;
        std::vector<u8> icon;
    };
}