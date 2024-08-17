#pragma once

#include <vfs/path.h>
#include <cfg/yaml.h>
namespace mizui::cfg {
    class Global {
    public:
        Global() = default;
        ~Global();
        Global(const vfs::Path& yaml);
    private:
        std::fstream yamlFile;
        Yaml global;
    };
}
