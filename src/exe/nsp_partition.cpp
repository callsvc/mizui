#include <exe/nsp_partition.h>
#include <exe/nca.h>

namespace mizui::exe {
    NspPartition::NspPartition(vfs::Mappable& nsp) :
        pfs(std::make_unique<orizonti::fs::PartitionFilesystem>(nsp)) {

        nspFiles = pfs->getFiles();
        readNcaEntries();

        const auto main{pfs->open("main")};
        const auto npdm{pfs->open("main.npdm")};
        const auto isExtracted = [&] () {
            return main && npdm;
        };
        if (!isExtracted()) {
        }
    }

    void NspPartition::readNcaEntries() const {
        for (auto& meta : pfs->cachedMeta) {
            [[maybe_unused]] Nca metaNca{meta};
        }
    }
}
