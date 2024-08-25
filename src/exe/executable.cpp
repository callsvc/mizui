#include <exe/executable.h>

namespace mizui::exe {
    Executable::Executable(std::fstream&& passport) {
        ioHandler = std::move(passport);
        backing = vfs::Mappable(ioHandler);
    }
}
