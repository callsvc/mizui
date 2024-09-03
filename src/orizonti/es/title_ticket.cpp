#include <print>

#include <boost/align/align_up.hpp>
#include <orizonti/es/title_ticket.h>
namespace orizonti::crypt {
    Ticket::Ticket(vfs::Support& ticketIo) {
        const auto signType{ticketIo.readSome<SignatureType>()};

        u64 offset{};
        switch (signType) {
            case Rsa4096Sha1:
            case Rsa4096Sha2:
                offset = 0x23c;
                break;
            case Rsa2048Sha1:
            case Rsa2048Sha2:
                offset = 0x13c;
                break;
            case EcdsaSha1:
            case EcdsaSha2:
                offset = 0x3c + 0x40;
            default: {}
        }
        offset = boost::alignment::align_up(0x4 + offset, 0x40);

        *this = ticketIo.readSome<Ticket>(offset);
        if (titleKeyType)
            if (titleKeyType == 1 && (signType != 0x010001 && signType != 0x010004))
                throw std::runtime_error("Bad title key type");

        std::print("Retail cert name: {}\n", std::string_view{&issuer[0]});
    }
}
