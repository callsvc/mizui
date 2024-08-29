#pragma once
#include <array>

#include <vfs/mappable.h>
#include <vfs/rofs.h>
namespace orizonti::fs {
    using namespace mizui;
#pragma pack(push, 1)
    struct FsPacket {
        u64 offset;
        u64 size;
        u32 strOffset; // Reserved, when present in the header
    };
    static_assert(std::is_trivially_copyable_v<FsPacket>);

    struct PartitionHeader {
        u32 magic;
        u32 countOfEntries;
        u32 strTableSize;
        u32 pad0;
    };

    struct Pfs0Entry {
        FsPacket table;
        u32 pad0;
    };
    struct Hfs0Entry {
        FsPacket table;
        u32 hashRegionSize;
        u64 pad0;
        std::array<u8, 0x20> regionHash;
    };
#pragma pack(pop)

    struct PfFile {
        std::string filename;
        u64 offset;
        u64 size;
    };

    class PartitionFilesystem final : vfs::RoFs {
    public:
        PartitionFilesystem(vfs::Mappable& placeable);
        std::vector<vfs::RoFile> getFiles() override;
    private:
        vfs::Mappable& backing;
        PartitionHeader header;
        bool isHfs;
        std::vector<PfFile> availableFiles;
    };
};