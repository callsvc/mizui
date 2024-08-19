#include <iostream>

#include <core/application.h>
namespace mizui::core {
    po::options_description commands{"Mizui options"};

    void Application::initialize(const std::vector<char*>& args) {
        commands.add_options()
            ("help", "display the table of commands available to the user")
            ("root-dir", po::value<std::string>(), "change the default root directory to the one chosen by the user");
        processOptions(args);

        if (!rootDir) {
            rootDir = std::filesystem::current_path();
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
            commands.print(std::cout, 0);
            halt();
        }
    }
}
