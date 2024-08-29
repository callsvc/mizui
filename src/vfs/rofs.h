#pragma once
#include <vector>

#include <vfs/mappable.h>
#include <vfs/path.h>
namespace mizui::vfs {
    class RoFile final : public Ranged {
    public:
        RoFile(const Path& filename, const u64 offset, const u64 virSize, Mappable& ro) : Ranged(Read, virSize, offset), name(filename), support(ro) {}
        explicit operator bool() const override;
        Path name;
    private:
        u64 readSomeImpl(std::span<u8> output, u64 offset) override;
        Mappable& support;
    };

    class RoFs {
    public:
        virtual ~RoFs() = default;
    protected:
        virtual std::vector<RoFile> getFiles() = 0;
    };
}
