#include <iostream>

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

        const auto& executables{assets.collection.readable};
        sw.stockEveryExecutable(executables);
    }

    bool Application::loadApplication(const u64 application) {
        auto playIt{std::begin(sw.playable)};
        std::advance(playIt, application);

        if (playIt == sw.playable.end())
            return {};

        const std::filesystem::path& path{playIt->ios};
        std::fstream io{path};
        std::vector<char> version(6);
        io.read(&version[0], version.size());

        if (std::string_view(&version[0], 6).find("NSO") !=
            std::string_view::npos) {
            sw.loadExecutable(exe::Nso, std::move(io));
            return true;
        }
        if (std::string_view(&version[0], 6).find("PFS0") !=
            std::string_view::npos) {
            sw.loadExecutable(exe::Nsp, std::move(io));

            return true;
        }
        return {};
    }

    std::vector<LoadableApplication> Application::getAllApplications() {
        std::vector<LoadableApplication> applications;

        for (const auto& playable : sw.playable) {
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
