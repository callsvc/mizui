#include <fstream>
#include <vector>
#include <ranges>

#include <boost/algorithm/string.hpp>

#include <common/modifiers.h>
#include <crypt/platform_keys.h>
namespace mizui::crypt {
    void PlatformKeys::initialize(const vfs::Path& keysDir) {
        const std::filesystem::path& fsKeyPath{keysDir};
        const std::filesystem::directory_iterator keys{fsKeyPath};

        bool title{}, key{};
        for (const auto& postKey : keys) {
            const std::filesystem::path& location{postKey.path()};
            if (location.filename() == "title.keys")
                title = addKeys(location, &PlatformKeys::addTitleKey);
            if (location.filename() == "prod.keys")
                key = addKeys(location, &PlatformKeys::addProdKey);
        }
        if (!title || !key)
            throw std::runtime_error("There are missing keys");
    }

    PlatformKeys::PlatformKeys() {
        keys256Names.emplace_back("header_key", Key256{});
        keys256Names.emplace_back("sd_card_save_key_source", Key256{});
        keys256Names.emplace_back("sd_card_nca_key_source", Key256{});
        keys256Names.emplace_back("header_key_source", Key256{});
        keys256Names.emplace_back("sd_card_save_key", Key256{});
        keys256Names.emplace_back("sd_card_nca_key", Key256{});
    }

    void PlatformKeys::addTitleKey(const std::string_view& alias, const std::string_view& value) {
        auto key{viewToByteArray<16>(alias)};
        auto secret{viewToByteArray<16>(value)};
        titles.emplace_back(key, secret);
    }
    void PlatformKeys::addTitleKey(Key128& alias, Key128& value) {
        titles.emplace_back(alias, value);
    }
    void PlatformKeys::addProdKey(const std::string_view& alias, const std::string_view& value) {
        std::string taggedKey;
        std::ranges::transform(alias, std::back_inserter(taggedKey), tolower);

        auto begin{std::begin(keys256Names)};
        const auto end{std::end(keys256Names)};
        for (; begin != end; ++begin) {
            if (begin->first != taggedKey)
                continue;
            begin->second = viewToByteArray<32>(value);
            break;
        }
    }

    bool PlatformKeys::addKeys(const vfs::Path& keyFile, addTypedKeys function) {
        const std::filesystem::path& path{keyFile};
        std::vector<u8> keyData(file_size(path));
        std::ifstream stream{path, std::ios::binary};

        if (keyData.empty()) {
            throw std::runtime_error("The key file is empty");
        }
        stream.read(reinterpret_cast<char*>(&keyData[0]), keyData.size());
        std::vector<std::string_view> pairs;
        std::string_view content{reinterpret_cast<char*>(&keyData[0]), keyData.size()};
        split(pairs, content, boost::is_any_of("=\n"));
        if (pairs.size() % 2)
            pairs.pop_back();

        for (const auto& pair : pairs | std::views::chunk(2)) {
            auto name{boost::trim_copy(pair[0])};
            auto value{boost::trim_copy(pair[1])};
            (this->*function)(name, value);
        }
        return true;
    }
}
