#pragma once
#include <boost/program_options.hpp>

#include <core/assets_manager.h>
#include <hle/virtual_nx.h>
#include <conf/global.h>
namespace mizui::core {
    namespace po = boost::program_options;
    class Application {
    public:
        void initialize(const std::vector<char*>& args);

        AssetsManager assets;
        vfs::Path rootDir;
        static void halt();
    private:
        void processOptions(const std::vector<char*>& args);
        conf::Global config;

        hle::VirtualNx sw;
    };
}
