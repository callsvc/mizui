#include <core/application.h>
#include <decl.h>
int main(const mizui::i32 argc, char** argv) {
    mizui::core::Application app;

    std::vector<char*> args;
    for (mizui::i32 arg{}; arg < argc; arg++) {
        args.push_back(argv[arg]);
    }
    app.initialize(args);

    app.takeGrass();
    return {};
}
