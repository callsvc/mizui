#include <vfs/nca.h>
#include <exe/nsp_partition.h>
namespace mizui::exe {
    NspPartition::NspPartition(crypt::PlatformKeys& set, vfs::Mappable& nsp) :
        pfs(std::make_unique<orizonti::fs::PartitionFilesystem>(nsp)),
        keys(set) {

        nspFiles = pfs->getFiles();

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
            [[maybe_unused]] Nca metaNca{keys, meta};
        }
    }
}
