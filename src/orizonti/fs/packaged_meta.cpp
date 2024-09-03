#include <orizonti/fs/packaged_meta.h>
namespace orizonti::fs {
    PackageMeta::PackageMeta(vfs::RoFile& cnmt) {
        cnmtHeader = cnmt.readSome<ContentHeader>();

    }
}
