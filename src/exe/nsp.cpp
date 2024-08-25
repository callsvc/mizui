#include <exe/nsp.h>

namespace mizui::exe {
    ExecutableFormat Nsp::checkExecutableType() {
        return ExecutableFormat::Nsp;
    }
    Nsp::Nsp(std::fstream&& os) : Executable(std::move(os)) {
        backing.readSome(header);
    }

    void Nsp::loadExecutable() {
    }
    void Nsp::readSegmentImpl(std::span<u8> section, u32 fileOffset, u32 compressed, bool isCompressed, bool checkHash) {
    }
}
