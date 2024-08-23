#include <exe/rofs.h>

mizui::exe::RoFs::RoFs(const Executable& within) {
    if (!within.backing)
        return;
}
