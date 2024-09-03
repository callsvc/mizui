
#pragma once

#include <orizonti/fs/partition_filesystem.h>
namespace mizui::exe {
    class NspPartition {
    public:
        NspPartition(vfs::Support& nsp);
        std::vector<vfs::ReadOnlyFile> nspFiles;

        void readNcaEntries() const;
    private:

        std::unique_ptr<orizonti::fs::PartitionFilesystem> pfs;
    };
}