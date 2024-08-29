#pragma once
#include <span>
// ReSharper disable once CppUnusedIncludeDirective
#include <fstream>

#include <vfs/ranged.h>
namespace mizui::vfs {
    class Mappable final : public Ranged {
    public:
        Mappable() = default;
        Mappable(std::fstream& io);

        explicit operator bool() const override;
        u64 size() const;
    private:
        u64 readSomeImpl(std::span<u8> output, u64 offset = {}) override;
        i32 descriptor{};
    };
}
