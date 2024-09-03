#pragma once

#include <vfs/path.h>
#include <config/yaml.h>
namespace mizui::config {
    class Global {
    public:
        Global() = default;
        ~Global();
        Global(const vfs::Path& yaml);
        auto operator = (Global&& spawn) noexcept {
            yamlFile.swap(yamlFile);
            global = spawn.global;

            return this;
        }
    private:
        std::fstream yamlFile{};
        Yaml global;
    };
}
