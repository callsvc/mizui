#pragma once

#include <cstring>
#include <string_view>

#include <common/decl.h>
namespace mizui {
    inline auto makeMagic(const std::string_view& number) {
        u64 value{};
        constexpr u32 zeroes{4};
        if (number.size() <= 4) {
            std::memcpy(&value, &number[0], number.size());
        }
        for (decltype(value) leading{}; leading < zeroes - number.size(); leading++)
            value <<= 8;

        return value;
    }
}