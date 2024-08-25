#pragma once

#include <orizonti/kernel/process.h>
namespace orizonti {
    class OperatingSystem {
    public:
        OperatingSystem();
    private:
        kernel::Process kProc;
    };
}
