#include <vfs/rofs.h>

namespace mizui::vfs {
    u64 RoFile::readSomeImpl(const std::span<u8> output, const u64 offset) {
        if (readPos != offset) {
            if (offset > size)
                return {};
            readPos += offset;
        }
        const auto result{support.readSome(output, readPos)};
        if (result != size) {
        }
        return result;
    }

    RoFile::operator bool() const {
        return mode == Read && readPos > 0 && support;
    }
}
