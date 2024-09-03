#include <vfs/nca.h>
#include <crypt/aes_decrypt.h>
#include <common/magic.h>
namespace mizui::exe {
    Nca::Nca(const crypt::PlatformKeys& set, vfs::Support& nca) {
        header = nca.readSome<NcaHeader>();
        auto isHeaderFine = [&] {
            return header.magic == makeMagic("NCA3");
        };
        if (!isHeaderFine()) {
            const auto key{set.headerKey};
            if (!key)
                throw std::runtime_error("NCA does not have a valid header key");
            crypt::AesDecrypt aes{*key, crypt::Aes128Xts};
            aes.decryptXts(header, 0, 0x200);
        }
    }
}
