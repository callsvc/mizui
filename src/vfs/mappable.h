#pragma once
#include <decl.h>
namespace mizui::vfs {
    class Mappable {
        public:
        Mappable() = default;
        Mappable(std::fstream& io);

        explicit operator bool() const;
        template <typename T> requires (
            !std::is_same_v<T, std::span<u8>> && std::is_trivially_copyable_v<T>)
        auto readSome(T& output) {
            // ReSharper disable once CppLocalVariableMayBeConst
            std::span data(reinterpret_cast<u8*>(&output), sizeof(T));
            return readSome(data);
        }
        u64 readSome(std::span<u8> output, u64 offset = 0) const;

        u64 rdPos{};
        u64 wrPos{};

        i32 descriptor{};
    };
}
