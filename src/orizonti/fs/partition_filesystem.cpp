#include <stdexcept>
#include <vector>

#include <orizonti/fs/partition_filesystem.h>
#include <magic.h>
namespace orizonti::fs {
    PartitionFilesystem::PartitionFilesystem(vfs::Mappable& placeable) : backing(placeable) {
        if (!backing.readSome(header))
            throw std::runtime_error("Incorrect size of backing");

        isHfs = header.magic == makeMagic("HFS0");
        const auto entrySize{isHfs ? sizeof(Hfs0Entry) : sizeof(Pfs0Entry)};
        if (isHfs)
            throw std::runtime_error("HFS is not supported for now");

        const auto superBlockSize{sizeof(header) + header.countOfEntries * entrySize + header.strTableSize};
        const auto stringsOffset{superBlockSize - header.strTableSize};
        const auto contentOffset{stringsOffset + header.strTableSize};

        std::vector<u8> content(superBlockSize);
        if (backing.readSome(content) != superBlockSize)
            throw std::runtime_error("Incorrect size of backing");

        if (!header.countOfEntries) {
            return;
        }
        availableFiles.reserve(header.countOfEntries);

        for (u32 partEntry{}; partEntry < header.countOfEntries; partEntry++) {
            FsPacket entryHead;
            const auto entryOffset{sizeof(header) + (partEntry * entrySize)};
            if (entryOffset + sizeof(entryHead) > content.size()) {
                throw std::runtime_error("The specified buffer has been exhausted");
            }
            std::memcpy(&entryHead, &content[entryOffset], sizeof(FsPacket));
            const auto filenameOffset{stringsOffset + entryHead.strOffset};
            const std::string name{reinterpret_cast<const char*>(&content[filenameOffset])};

            availableFiles.emplace_back(std::move(name), contentOffset + entryHead.offset, entryHead.size);
        }
    }

    std::vector<vfs::RoRangedFile> PartitionFilesystem::getFiles() {
        std::vector<vfs::RoRangedFile> files;
        files.reserve(availableFiles.size());
        for (const auto& fsEntry : availableFiles) {
            files.emplace_back(fsEntry.offset, fsEntry.size, backing);
        }
        return files;
    }
}
