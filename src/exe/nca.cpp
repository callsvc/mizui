#include <exe/nca.h>

namespace mizui::exe {
    Nca::Nca(vfs::Support& nca) {
        header = nca.readSome<NcaHeader>();
    }
}
