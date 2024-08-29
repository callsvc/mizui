#pragma once

#include <vector>
#include <vfs/mappable.h>
namespace mizui::exe {
    enum class ExecutableFormat {
        Unrecognized,
        Nso,
        Nsp
    };
    enum HeadersSz {
        SizeNso = 0xe0 + 0x20
    };

    enum SegmentType {
        Text, RoData, Data
    };

    class Executable {
    public:
        Executable() = default;
        Executable(std::fstream&& passport);
        virtual ~Executable() = default;

        virtual ExecutableFormat checkExecutableType() = 0;
        virtual void loadExecutable() = 0;

        vfs::Mappable backing;
        std::vector<u8> regions;

        std::span<u8> textSegment;
        std::span<u8> roSegment;
        std::span<u8> dataSegment;
    protected:
        virtual void readSegmentImpl(std::span<u8> section, u32 fileOffset, u32 compressed, bool isCompressed, bool checkHash) = 0;

    private:
        std::fstream ioHandler;
    };
}
