#pragma once

#include <exe/executable.h>
#include <decl.h>
namespace mizui::exe::nso {
    struct SegmentHeader {
        u32 offset, memoryOffset;
        u32 decompressed;
        union {
            u32 module;
            u32 bssSize;
        };
    };
    struct RoSegmentHeaderRelative {
        u32 offset;
        u32 size;
    };

    struct NsoHeader {
        u32 magic;
        u32 version;
        u32 pad0;
        u32 flags;
        std::array<SegmentHeader, 3> segments;
        std::array<u8, 0x20> moduleId;
        std::array<u32, 3> segmentsSize;
        std::array<u8, 0x1c> pad1;
        std::array<RoSegmentHeaderRelative, 3> roSegmentsRelatives;
        std::array<std::array<u8, 0x20>, 3> segmentsHashes;
    };
    static_assert(sizeof(NsoHeader) == SizeNso);
    static_assert(std::is_trivially_copyable_v<NsoHeader>);

    class Nso final : public Executable {
    public:
        Nso(std::fstream&& os) : Executable(std::move(os)) {
        }
        ExecutableFormat checkExecutableType() override;
        void loadExecutable() override;

        NsoHeader header{};
    private:
        void printRoSectionInfo();

        void decompressSegment(SegmentType segment, u32 offset);
        void readSegmentImpl(std::span<u8> section, u32 fileOffset, u32 compressed, bool isCompressed, bool checkHash) override;
    };
}