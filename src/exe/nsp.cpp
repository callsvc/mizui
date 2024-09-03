#include <exe/nsp.h>

#include <common/magic.h>
#include <orizonti/es/title_ticket.h>
namespace mizui::exe {
    ExecutableFormat Nsp::checkExecutableType() {
        const auto magic{backing.readSome<u32>()};
        if (backing.size() < sizeof(magic))
            return ExecutableFormat::Unrecognized;
        if (magic != makeMagic("HFS0") ||
            magic != makeMagic("PFS0"))
            return ExecutableFormat::Unrecognized;

        return ExecutableFormat::Nsp;
    }
    Nsp::Nsp(crypt::PlatformKeys& set, std::fstream&& os) :
        Executable(std::move(os)),
        pfs(std::make_unique<orizonti::fs::PartitionFilesystem>(backing)) {

        nspFiles = pfs->getFiles();
        const auto main{pfs->open("main")};
        const auto npdm{pfs->open("main.npdm")};
        if (!(main && npdm)) {

        }

        if (!nspFiles.empty())
            readTickets(set);

        header = backing.readSome<NspHeader>();
    }
    void Nsp::readTickets(crypt::PlatformKeys& set) {
        for (const auto& entry : nspFiles) {
            const std::filesystem::path& ioName{entry.name};
            if (ioName.extension() != ".tik")
                continue;

            vfs::RoFile file{entry};
            orizonti::crypt::Ticket ticket{file};

            crypt::Key128 value;
            std::memcpy(&value[0], &ticket.titleKeyBlock[0], sizeof(value));
            set.addTitleKey(ticket.rights, value);
        }
    }

    void Nsp::loadExecutable() {
    }
    void Nsp::readSegmentImpl(std::span<u8> section, u32 fileOffset, u32 compressed, bool isCompressed, bool checkHash) {
    }
}
