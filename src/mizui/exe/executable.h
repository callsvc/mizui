#pragma once

#include <fstream>
#include <vfs/mappable.h>
namespace mizui::exe {
    enum ExecutableFormats {
        Unrecognized,
        NSO
    };

    class Executable {
    public:
        Executable() = default;
        Executable(std::fstream&& passport);
        virtual ~Executable() = default;

        virtual bool sanitizeInputIo() = 0;
        virtual void loadExecutable() = 0;

        vfs::Mappable backing;
    private:
        std::fstream ioHandler;
    };
}
