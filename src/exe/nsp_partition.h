
#pragma once

#include <crypt/platform_keys.h>
#include <orizonti/fs/partition_filesystem.h>
namespace mizui::exe {
    class NspPartition {
    public:
        NspPartition(crypt::PlatformKeys& set, vfs::Mappable& nsp);
        std::vector<vfs::ReadOnlyFile> nspFiles;

        void readNcaEntries() const;
    private:

        std::unique_ptr<orizonti::fs::PartitionFilesystem> pfs;
        crypt::PlatformKeys& keys;
    };
}