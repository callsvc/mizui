#include <orizonti/fs/packaged_meta.h>
namespace orizonti::fs {
    PackageMeta::PackageMeta(vfs::Support& cnmt) {
        cnmtHeader = cnmt.readSome<ContentHeader>();

    }
}
