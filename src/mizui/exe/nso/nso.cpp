#include <exe/nso/nso.h>

#include <exe/rofs.h>
namespace mizui::exe::nso {
    bool Nso::sanitizeInputIo() {
        return true;
    }
    void Nso::loadExecutable() {
        [[maybe_unused]] RoFs partitionFs{*this};
    }
}
