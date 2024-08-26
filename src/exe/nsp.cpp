#include <exe/nsp.h>

#include <magic.h>
namespace mizui::exe {
    ExecutableFormat Nsp::checkExecutableType() {
        u32 magic;
        backing.readSome(magic);
        if (backing.size() < sizeof(magic))
            return ExecutableFormat::Unrecognized;
        if (magic != makeMagic("HFS0") ||
            magic != makeMagic("PFS0"))
            return ExecutableFormat::Unrecognized;

        return ExecutableFormat::Nsp;
    }
    Nsp::Nsp(std::fstream&& os) :
        Executable(std::move(os)),
        pfs(std::make_unique<orizonti::fs::PartitionFilesystem>(backing)) {

        [[maybe_unused]] auto nspFiles{pfs->getFiles()};

        backing.readSome(header);
    }

    void Nsp::loadExecutable() {
    }
    void Nsp::readSegmentImpl(std::span<u8> section, u32 fileOffset, u32 compressed, bool isCompressed, bool checkHash) {
    }
}
