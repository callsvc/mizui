#include <cstring>
#include <iostream>
#include <sstream>

#include <boost/regex.hpp>
#include <fmt/format.h>
#include <lz4.h>

#include <exe/nso/nso.h>
namespace mizui::exe::nso {
    bool Nso::sanitizeInputIo() {
        return true;
    }

    void Nso::printRoSectionInfo() {
        std::stringstream output;
        std::string modulePath;
        const std::string content{reinterpret_cast<char*>(&roSegment[0]), roSegment.size()};
        if (std::memcmp(&content[0], "\\0\\0\\0\\0", 4)) {
            u32 length;
            std::memcpy(&length, &content[4], 4);
            if (length)
                modulePath = content.substr(8, length);
        }

        const boost::regex module{""};
        const boost::regex fsSdk{""};
        const boost::regex sdkLibraries{""};

        if (modulePath.empty()) {
            boost::smatch match;
            if (regex_search(content, match, module)) {
                modulePath = match.str();
            }
        }
        boost::smatch match;
        output << fmt::format("Module {}\n", modulePath);
        if (regex_search(content, match, fsSdk)) {
            output << fmt::format("FS SDK {}\n", match.str());
        }

        boost::sregex_iterator matchesSdkIt{content.begin(), content.end(), sdkLibraries};
        boost::sregex_iterator end;
        if (matchesSdkIt != end) {
            output << fmt::format("SDK Libraries\n");
            for (; matchesSdkIt != end; ++matchesSdkIt)
                output << fmt::format("\t{}\n", matchesSdkIt->str());

            output << "\n";
        }

        std::cout << output.str();
    }

    void Nso::decompressSegment(const SegmentType segment, const u32 offset) {
        auto& decompress{textSegment};

        if (segment == RoData)
            decompress = roSegment;
        else if (segment == Data)
            decompress = dataSegment;

        const auto segmentSize{header.segments[segment].decompressed};
        const auto compressed{(header.flags >> static_cast<u32>(segment)) & 1};
        const auto sanitize{(header.flags >> static_cast<u32>(segment)) & 8};

        decompress = {&regions[offset], segmentSize};
        readSegmentImpl(decompress, header.segments[segment].offset, header.segmentsSize[segment], compressed, sanitize);
    }

    void Nso::readSegmentImpl(const std::span<u8> section, const u32 fileOffset, const u32 compressed, const bool isCompressed, const bool checkHash) {
        u32 decompress{};
        if (isCompressed)
            decompress = compressed;

        if (decompress) {
            std::vector<u8> compressedChunk(compressed);
            if (backing.read(compressedChunk, fileOffset) != fileOffset) {
            }
            LZ4_decompress_safe(
                reinterpret_cast<char*>(&compressedChunk[0]),
                reinterpret_cast<char*>(&section[0]),
                decompress, section.size());
        } else {
            if (backing.read(section, fileOffset) != fileOffset) {
            }
        }

        if (checkHash) {
        }
    }

    void Nso::loadExecutable() {
        backing.read(header);

        std::memcpy(&pumpkin[0], &header, sizeof(header));
        if (header.version != 0) {
        }

        const auto requiredSegmentSz{
            header.segments[2].memoryOffset + header.segments[2].decompressed};
        regions.resize(requiredSegmentSz);

        const auto textOffset{header.segments[0].memoryOffset};
        decompressSegment(Text, textOffset);
        const auto roOffset{header.segments[1].memoryOffset};
        decompressSegment(RoData, roOffset);
        const auto dataOffset{header.segments[1].memoryOffset};
        decompressSegment(Data, dataOffset);

        printRoSectionInfo();
    }
}
