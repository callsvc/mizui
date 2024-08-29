#pragma once

#include <array>
#include <stdexcept>
#include <ranges>

#include <common/decl.h>
namespace mizui {
    template<u64 Size>
        auto viewToByteArray(const std::string_view& text) {
        auto decodeByte = [&](auto encode) -> u8 {
            if (encode >= 'A' && encode <= 'F')
                return encode - 'A' + 10;
            if (encode >= 'a' && encode <= 'f')
                return encode - 'a' + 10;
            if (encode >= '0' && encode <= '9')
                return encode - '0';

            throw std::runtime_error("Invalid char code");
        };

        if (text.size() % 2 || text.size() != Size * 2) {
            throw std::runtime_error("Malformed input string");
        }
        std::array<u8, Size> result{};
        for (const auto [index, encode] : std::views::enumerate(text)) {
            auto value{decodeByte(encode)};
            if (!(index % 2)) {
                result[index ? index / 2 >= 1 ? index / 2 : 0 : 0] = value << 4;
            } else {
                result[index ? (index - 1) / 2 >= 1 ? (index - 1) / 2 : 0 : 0] |= value;
            }
        }
        return result;
    }
}
