#pragma once
#include <mbedtls/cipher.h>

#include <crypt/platform_keys.h>
namespace mizui::crypt {
    enum AesMode {
        Aes128Xts, // Commonly used in storage devices
        Aes128Ecb,
        Aes128Ctr, // Counter mode allows random access during decryption and enables the use of parallelism
    };
    struct DecryptIo {
        u8* destination;
        u8* source;
        u64 size;
    };
    class AesDecrypt {
    public:
        AesDecrypt(const Key256& useKey, AesMode mode);
        ~AesDecrypt();

        void decrypt(const DecryptIo& decCtx, u64 offset, u64 size);
        void decryptXts(const DecryptIo& decCtx, u64 sector, u64 size);

        template <typename T>
        void decryptXts(T& buffer, const u64 sector, const u64 size) {
            std::span ranged{reinterpret_cast<u8*>(&buffer), sizeof(T)};
            decryptXts({&ranged[0], &ranged[0], ranged.size()}, sector, size);
        }
    private:
        void setXtsIv(u64 counter);

        std::vector<u8> backing;
        mbedtls_cipher_context_t context;
    };
}
