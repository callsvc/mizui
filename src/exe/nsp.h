#pragma once
#include <memory>

#include <exe/executable.h>
#include <exe/nsp_partition.h>

#include <crypt/platform_keys.h>
namespace mizui::exe {
    struct NspHeader {
        u32 magic;
        u32 entryCount;
        u32 stringTableSize;
        u32 pad0;
    };
    class Nsp final : public Executable {
    public:
        Nsp(crypt::PlatformKeys& set, std::fstream&& os);

        ExecutableFormat checkExecutableType() override;
        void loadExecutable() override;
    private:
        void readTickets(crypt::PlatformKeys& set) const;

        void readSegmentImpl(std::span<u8> section, u32 fileOffset, u32 compressed, bool isCompressed,
            bool checkHash) override;

        NspHeader header;
        std::unique_ptr<NspPartition> nspFs;
    };
}
