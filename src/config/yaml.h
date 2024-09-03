#pragma once
#include <vector>
#include <fstream>

namespace mizui::config {

    struct Pair {
        std::string key;
        std::string value;
    };

    class Yaml {
    public:
        Yaml() = default;
        Yaml(std::fstream& yaml);

        std::vector<Pair> pairs;
        template<typename T>
        auto get(const std::string& bind) {
            for (const auto& [key, value] : pairs) {
                if constexpr (std::is_same_v<T, std::string>) {
                    if (key == bind)
                        return value;
                }
            }
            return T{};
        }
    };
}
