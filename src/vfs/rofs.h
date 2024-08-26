#pragma once
#include <vector>

#include <vfs/mappable.h>
#include <vfs/path.h>
namespace mizui::vfs {
    class RoRangedFile {
    public:
        RoRangedFile(u64 readOff, u64 readSize, Mappable& readOnly);
    private:
        Path name;
        std::vector<u8> content;

        u64 offset;
        u64 size;
        Mappable& support;
    };

    class RoFs {
    public:
        virtual ~RoFs() = default;
    protected:
        virtual std::vector<RoRangedFile> getFiles() = 0;
    };
}
