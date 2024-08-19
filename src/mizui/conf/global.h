#pragma once

#include <vfs/path.h>
#include <conf/yaml.h>
namespace mizui::conf {
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
