#include <stdexcept>
#include <vector>

#include <orizonti/fs/partition_filesystem.h>
#include <common/magic.h>
#define PFS_SUPPORT_HFS 1
namespace orizonti::fs {
    PartitionFilesystem::PartitionFilesystem(vfs::Support& placeable) : backing(placeable) {
        header = backing.readSome<PartitionHeader>();

        if (header.magic == makeMagic("HFS0"))
            isHfs = true;
#if !PFS_SUPPORT_HFS
        if (isHfs) {
            throw std::runtime_error("HFS is not supported for now");
        }
#endif
        readEntries();

        for (auto& contentFile : getFiles()) {
            const std::string contentStrPath{contentFile.name};
            if (!contentStrPath.ends_with(".cnmt.nca") &&
                !contentStrPath.ends_with("meta0.ncd"))
                continue;
            cachedMeta.emplace_back(contentFile);
        }
    }

    std::vector<vfs::ReadOnlyFile> PartitionFilesystem::getFiles() {
        std::vector<vfs::ReadOnlyFile> files;
        files.reserve(availableFiles.size());
        for (const auto& fsEntry : availableFiles) {
            files.emplace_back(fsEntry.filename, fsEntry.offset, fsEntry.size, backing);
        }
        return files;
    }
    std::optional<vfs::ReadOnlyFile> PartitionFilesystem::open(const std::string& filename) {
        for (auto& fixedEntry : availableFiles) {
            if (fixedEntry.filename == filename) {
                return vfs::ReadOnlyFile{fixedEntry.filename, fixedEntry.offset, fixedEntry.size, backing};
            }
        }
        return {};
    }

    void PartitionFilesystem::readEntries() {
        const auto entrySize{isHfs ? sizeof(Hfs0Entry) : sizeof(Pfs0Entry)};

        const auto superBlockSize{sizeof(header) + header.countOfEntries * entrySize + header.strTableSize};
        const auto stringsOffset{superBlockSize - header.strTableSize};
        const auto contentOffset{stringsOffset + header.strTableSize};

        std::vector<u8> content(superBlockSize);
        if (backing.readSome(std::span(content)) != superBlockSize)
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
}
