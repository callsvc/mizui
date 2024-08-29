#pragma once
#include <string_view>
#include <vector>
#include <utility>

#include <vfs/path.h>
namespace mizui::crypt {
    using Key128 = std::array<u8, 16>;
    using Key256 = std::array<u8, 32>;

    class PlatformKeys {
    public:
        PlatformKeys();
        void initialize(const vfs::Path& keysDir);

        using addTypedKeys = void (PlatformKeys::*)(const std::string_view&, const std::string_view&);
        bool addKeys(const vfs::Path& keyFile, addTypedKeys function);

        void addTitleKey(const std::string_view& alias, const std::string_view& value);
        void addTitleKey(Key128& alias, Key128& value);
        void addProdKey(const std::string_view& alias, const std::string_view& value);
    private:
        std::vector<std::pair<std::string_view, Key256>> keys256Names;

        std::vector<std::pair<Key128, Key128>> titles;
    };
}
