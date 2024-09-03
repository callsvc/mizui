#pragma once

#include <vfs/rofs.h>
namespace orizonti::fs {
    using namespace mizui;

    enum ContentType : u8 {
        Invalid,
        SystemProgram,
        SystemData,
        SystemUpdate,
        Application = 0x80,
        Patch,
        Dlc,
    };
#pragma pack(push, 1)
    struct ContentHeader {
        u64 id;
        u32 version;
        ContentType metaType;
        u8 reserved0;
        u16 extendedSize;
        u16 count;
        u16 metaCount;
        u8 metaAttributes;
        std::array<u8, 3> reserved1;
        u32 requiredVersion;
        u32 reserved2;
    };
#pragma pack(pop)
    class PackageMeta {
        public:
        PackageMeta(vfs::RoFile& cnmt);

    private:
        ContentHeader cnmtHeader;
    };
}
