#include <cstring>

#include <boost/endian/arithmetic.hpp>
#include <crypt/aes_decrypt.h>
namespace mizui::crypt {

    AesDecrypt::AesDecrypt(const Key256& useKey, const AesMode mode) {
        mbedtls_cipher_init(&context);

        auto sanitizeContentType = [&] {
            if (mode == Aes128Xts)
                return MBEDTLS_CIPHER_AES_128_XTS;
            if (mode == Aes128Ctr)
                return MBEDTLS_CIPHER_AES_128_CTR;
            if (mode == Aes128Ecb)
                return MBEDTLS_CIPHER_AES_128_ECB;

            throw std::runtime_error("AES-128 operation mode not found");
        };
        if (mbedtls_cipher_setup(&context, mbedtls_cipher_info_from_type(sanitizeContentType())))
            throw std::runtime_error("Failed to set the key type for the generic cipher");

        if (mbedtls_cipher_setkey(&context, &useKey[0], useKey.size() * 8, MBEDTLS_DECRYPT))
            throw std::runtime_error("Failed to configure the decryption key");
    }

    AesDecrypt::~AesDecrypt() {
        mbedtls_cipher_free(&context);
    }

    void AesDecrypt::setXtsIv(const u64 counter) {
        std::array<u8, 0x10> iv{};
        const auto twisted{boost::endian::endian_reverse(counter)};
        std::memcpy(&iv[8], &twisted, 8);

        if (mbedtls_cipher_set_iv(&context, &iv[0], iv.size()))
            throw std::runtime_error("There was a failure in setting up the IV");
    }

    void AesDecrypt::decrypt(const DecryptIo& decCtx, const u64 offset, const u64 size) {
        constexpr auto maxBackingSize{1024 * 1024 * 1};
        const auto& source{decCtx.source[offset]};
        auto& destination{decCtx.destination[offset]};

        const auto target = [&] {
            if (&source != &destination)
                return &destination;

            if (size < maxBackingSize)
                if (backing.size() < size)
                    backing.resize(size);
            return &backing[0];
        }();

        u64 length;
        mbedtls_cipher_reset(&context);
        if (mbedtls_cipher_get_cipher_mode(&context) == MBEDTLS_MODE_XTS) {
            mbedtls_cipher_update(&context, &source, size, target, &length);
        }

        if (target == backing.data())
            std::memcpy(&destination, &backing[0], length);
    }
    void AesDecrypt::decryptXts(const DecryptIo& decCtx, u64 sector, const u64 size) {
        if (sector % size)
            throw std::runtime_error("Size not a multiple of the sector size");

        for (u64 block{}; block < decCtx.size; block += size) {
            setXtsIv(sector++);
            decrypt(decCtx, block, size);
        }
    }
}
