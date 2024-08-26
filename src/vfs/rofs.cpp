#include <vfs/rofs.h>

namespace mizui::vfs {
    RoRangedFile::RoRangedFile(const u64 readOff, const u64 readSize, Mappable& readOnly) :
        offset(readOff),
        size(readSize),
        support(readOnly) {
    }
}