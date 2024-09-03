#pragma once
#include <span>
// ReSharper disable once CppUnusedIncludeDirective
#include <fstream>

#include <vfs/support.h>
namespace mizui::vfs {
    class Mappable final : public Support {
    public:
        Mappable() = default;
        Mappable(std::fstream& io);

        explicit operator bool() const override;
        u64 getSize() const override;
    private:
        u64 readSomeImpl(std::span<u8> output, u64 offset = {}) override;
        i32 descriptor{};
    };
}
