#pragma once

#include <exe/executable.h>
namespace mizui::exe::nso {
    class Nso final : public Executable {
    public:
        Nso(std::fstream&& os) : Executable(std::move(os)) {
        }
        bool sanitizeInputIo() override;
        void loadExecutable() override;
    };
}