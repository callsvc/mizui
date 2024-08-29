#pragma once

#include <array>

#include <vfs/rofs.h>
#include <crypt/platform_keys.h>
namespace orizonti::crypt {
    using namespace mizui;

    template <u64 Size>
    using Signature = std::array<u8, Size>;

    enum SignatureType : u32 {
        Rsa4096Sha1 = 0x010000,
        Rsa2048Sha1 = 0x010001,
        EcdsaSha1 = 0x010002,
        Rsa4096Sha2 = 0x010003,
        Rsa2048Sha2 = 0x010004,
        EcdsaSha2 = 0x010005,
    };

    struct Ticket {
        Ticket() = default;
        Ticket(vfs::RoFile& ticketIo);

        std::array<char, 0x40> issuer;
        std::array<u8, 0x100> titleKeyBlock;
        u8 version; // Always 2 for Switch (ES) Tickets
        u8 titleKeyType;
        u16 ticketVersion;
        u8 licenseType;
        u8 masterKeyRevision;
        u16 properties;
        u64 reserved;
        u64 id;
        u64 device;
        mizui::crypt::Key128 rights;

        u32 account;
        // The signature block is part of the ticket's data block, which means that the unknown data may or may not
        // contain data due to the calculation of the signature sizes, as they precede the ticket data
    };
}
