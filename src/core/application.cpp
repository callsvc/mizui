#include <iostream>

#include <common/magic.h>
#include <core/application.h>
namespace mizui::core {
    po::options_description commands{"Mizui options"};

    void Application::initialize(const std::vector<char*>& args) {
        commands.add_options()
            ("help",
                "display the table of commands available to the user")
            ("root-dir", po::value<std::string>(),
                "change the default root directory to the one chosen by the user");
        processOptions(args);
        if (!rootDir) {
#if defined(MIZUI_USE_TEMP_DIR_AS_ROOT)
            rootDir = std::filesystem::temp_directory_path() / "Mizui";
#else
            rootDir = std::filesystem::current_path();
#endif
        }

        const std::filesystem::path& root{rootDir};
        if (!exists(root)) {
            create_directory(root);
        }
        config = conf::Global(rootDir / "mizui.yaml");
        assets.initialize(rootDir);

        device = hle::VirtualNx(assets);
    }

    bool Application::loadApplication(const u64 application) {
        auto playIt{std::begin(device.playable)};
        std::advance(playIt, application);

        if (playIt == device.playable.end())
            return {};

        const std::filesystem::path& path{playIt->ios};
        std::fstream io{path};
        vfs::Mappable game{io};

        const auto format = [&] {
            const auto version{game.readSome<u32>()};
            if (version == makeMagic("NSO0")) {
                return exe::ExecutableFormat::Nso;
            }
            if (version == makeMagic("PFS0")) {
                return exe::ExecutableFormat::Nsp;
            }
            return exe::ExecutableFormat::Unrecognized;
        }();
        return device.loadExecutable(format, std::move(io));
    }

    std::vector<LoadableApplication> Application::getAllApplications() {
        std::vector<LoadableApplication> applications;

        for (const auto& playable : device.playable) {
            applications.emplace_back(playable.title, playable.ios, playable.playId);
        }
        return applications;
    }

    void Application::halt() {
        std::cin.clear();
        std::cin.ignore();
        std::cin.get();
    }

    void Application::processOptions(const std::vector<char*>& args) {
        po::variables_map vm;
        store(parse_command_line(args.size(), &args[0], commands), vm);
        notify(vm);

        if (vm.contains("root-dir"))
            rootDir = vm["root-dir"].as<std::string>();
        if (vm.contains("help")) {
            commands.print(std::cout);
            halt();
        }
    }
}
