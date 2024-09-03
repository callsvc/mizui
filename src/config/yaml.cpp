#include <regex>
#include <boost/algorithm/string.hpp>

#include <config/yaml.h>
namespace mizui::config {
    Yaml::Yaml(std::fstream& yaml) {
        std::vector<char> buffer(256);
        yaml.seekg(std::ios::beg);

        if (!yaml.gcount()) {
            pairs.clear();
        }
        const std::regex keyBased("^\\S+ : \\S+$");
        std::cmatch match;
        std::vector<std::string> split;

        do {
            yaml.getline(&buffer[0], buffer.size());
            std::string_view line{&buffer[0], static_cast<std::size_t>(yaml.gcount())};
            std::regex_search(line.begin(), line.end(), match, keyBased);
            if (!match.empty()) {
                auto result{match.str()};
                boost::split(split, result, boost::is_any_of(":"));
                if (split.size() != 2)
                    continue;

                auto& key{split[0]};
                auto& value{split[1]};
                boost::trim(key);
                boost::trim(value);
                pairs.emplace_back(key, value);
            }
        } while (yaml.gcount());
    }
}
