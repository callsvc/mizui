#include <conf/global.h>

namespace mizui::conf {
    Global::~Global() {
        if (yamlFile.is_open())
            yamlFile.close();
    }

    Global::Global(const vfs::Path& yaml) {
        const std::filesystem::path config{yaml};
        yamlFile = {config, std::ios::in | std::ios::out};

        if (!yamlFile.is_open()) {
        }
        global = Yaml(yamlFile);
    }
}