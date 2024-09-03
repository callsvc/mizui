#include <exe/nsp.h>

#include <common/magic.h>
#include <orizonti/es/title_ticket.h>
namespace mizui::exe {
    ExecutableFormat Nsp::checkExecutableType() {
        const auto magic{backing.readSome<u32>()};
        if (backing.getSize() < sizeof(magic))
            return ExecutableFormat::Unrecognized;
        if (magic != makeMagic("HFS0") ||
            magic != makeMagic("PFS0"))
            return ExecutableFormat::Unrecognized;

        return ExecutableFormat::Nsp;
    }
    Nsp::Nsp(crypt::PlatformKeys& set, std::fstream&& os) :
        Executable(std::move(os)),
        nspFs(std::make_unique<NspPartition>(set, backing)) {

        const auto files{nspFs->nspFiles};
        if (!files.empty())
            readTickets(set);
        nspFs->readNcaEntries();
    }
    void Nsp::readTickets(crypt::PlatformKeys& set) const {
        for (const auto& entry : nspFs->nspFiles) {
            const std::filesystem::path& ioName{entry.name};
            if (ioName.extension() != ".tik")
                continue;

            vfs::ReadOnlyFile file{entry};
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
