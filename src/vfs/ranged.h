#pragma once
#include <span>

#include <common/decl.h>
namespace mizui::vfs {
    enum RangedAccess {
        Read,
        Write
    };
    class Ranged {
    public:
        virtual ~Ranged() = default;
        Ranged(const RangedAccess ioMode = Read, const u64 sz = {}, const u64 rPos = {}, const u64 wPos = {}) : mode(ioMode), size(sz), readPos(rPos), writePos(wPos) {}

        template <typename T> requires (!std::is_same_v<T, std::span<u8>>)
        auto readSome(std::span<T> output, const u64 offset) {
            return readSomeImpl(output, offset);
        }
        template <typename T> requires (!std::is_same_v<T, std::span<u8>>)
        auto readSome(std::span<T> output) {
            return readSome(output, readPos);
        }

        template <typename T> requires (!std::is_same_v<T, std::span<u8>> && std::is_trivially_copyable_v<T>)
        auto readSome(T& output, const u64 offset) {
            // ReSharper disable once CppLocalVariableMayBeConst
            std::span data{reinterpret_cast<u8*>(&output), sizeof(T)};
            return readSomeImpl(data, offset);
        }
        template <typename T> requires (!std::is_same_v<T, std::span<u8>> && std::is_trivially_copyable_v<T>)
        auto readSome(const u64 offset) {
            T object;
            // ReSharper disable once CppLocalVariableMayBeConst
            std::span data{reinterpret_cast<u8*>(&object), sizeof(T)};
            readSomeImpl(data, offset);
            return object;
        }
        template <typename T> requires (!std::is_same_v<T, std::span<u8>> && std::is_trivially_copyable_v<T>)
        auto readSome() {
            return readSome<T>(readPos);
        }

        virtual explicit operator bool() const = 0;
        virtual u64 getSize() const {
            return size;
        }
    protected:
        virtual u64 readSomeImpl(std::span<u8> output, u64 offset = {}) = 0;

        RangedAccess mode;
        u64 size;

        u64 readPos;
        u64 writePos;
    };
}