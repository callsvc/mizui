#pragma once
#include <boost/program_options.hpp>

#include <core/assets_manager.h>
#include <cfg/global.h>
namespace mizui::core {
    namespace po = boost::program_options;

    class Application {
    public:
        Application() = default;
        void initialize(const std::vector<char*> &args);

        AssetsManager assets;
        vfs::Path rootDir;

        static void takeGrass();
    private:
        void processOptions(const std::vector<char*>& args);

        std::unique_ptr<cfg::Global> config;
    };
}
