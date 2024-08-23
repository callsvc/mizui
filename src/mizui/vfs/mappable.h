#pragma once
#include <fstream>

#include <decl.h>
namespace mizui::vfs {
    class Mappable {
        public:
        Mappable() = default;
        Mappable(std::fstream& io);

        explicit operator bool() const;

        u64 read{};
        u64 write{};
#if defined(__linux__)
        i32 descriptor{};
#endif
    };
}
