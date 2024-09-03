#pragma once
#include <array>

#include <crypt/platform_keys.h>
#include <vfs/mappable.h>
namespace mizui::exe {
    enum Distribution {
        EShop,
        GameCard
    };
    enum ContentType {
        Program,
        Meta,
        Control,
        Manual,
        Data,
        PublicData
    };
    enum KeyGeneration : u8 {
        FwSystem12 = 0x0c, // 12.1.0
        FwSystem16 = 0x10, // 16.0.0
    };
    struct OffsetFsEntry {
        u32 start;
        u32 end;
        u64 reserved;
    };
    struct NcaHeader {
        std::array<u8, 0x100> headerSignature;
        decltype(headerSignature) npdmSignature;

        u32 magic;
        Distribution distType;
        ContentType contType;
        __attribute__((deprecated)) u8 oldKeyGenUnused;
        u8 keyArea;
        u64 contentSize;
        u64 programId;
        u32 contentIndex;

        u32 sdkAddonVersion;
        KeyGeneration keyGeneration;
        u8 signatureKeyGeneration;
        std::array<u8, 0xe> reserved;

        crypt::Key128 rightsId;
        std::array<OffsetFsEntry, 4> offsets;
        // Array of SHA256 hashes (over each FsHeader)
        std::array<std::array<u8, 0x20>, 4> hashes;
    };
    static_assert(std::is_trivially_copyable_v<NcaHeader>, "The above structure should not have constructors");
    class Nca {
    public:
        Nca(vfs::Support& nca);

    private:
        NcaHeader header;
    };
}
