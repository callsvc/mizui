#pragma once
#include <vector>

#include <vfs/mappable.h>
#include <vfs/path.h>
namespace mizui::vfs {
    class ReadOnlyFile final : public Support {
    public:
        ReadOnlyFile(const Path& filename, const u64 off, const u64 sz, Mappable& ro) :
            name(filename), offset(off), size(sz), backing(ro) {
        }
        explicit operator bool() const override;
        Path name;
    private:
        u64 offset;
        u64 size;
        Mappable& backing;
        u64 readSomeImpl(std::span<u8> output, u64 offset) override;
    };

    class RoFs {
    public:
        virtual ~RoFs() = default;
    protected:
        virtual std::vector<ReadOnlyFile> getFiles() = 0;
        virtual std::optional<ReadOnlyFile> open(const std::string& filename) = 0;
    };
}
