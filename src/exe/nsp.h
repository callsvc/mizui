#pragma once
#include <memory>

#include <exe/executable.h>
#include <orizonti/fs/partition_filesystem.h>
namespace mizui::exe {
    struct NspHeader {
        u32 magic;
        u32 entryCount;
        u32 stringTableSize;
        u32 pad0;
    };
    class Nsp final : public Executable {
    public:
        Nsp(std::fstream&& os);

        ExecutableFormat checkExecutableType() override;
        void loadExecutable() override;

    private:
        std::unique_ptr<orizonti::fs::PartitionFilesystem> pfs;

        void readSegmentImpl(std::span<u8> section, u32 fileOffset, u32 compressed, bool isCompressed,
            bool checkHash) override;
        NspHeader header;
    };
}
