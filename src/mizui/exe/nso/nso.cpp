#include <cstring>
#include <iostream>
#include <sstream>

#include <boost/regex.hpp>
#include <fmt/format.h>
#include <lz4.h>

#include <exe/nso/nso.h>
namespace mizui::exe::nso {
    auto makeMagic(const std::string_view& number) {
        u64 value{};
        constexpr u32 zeroes{4};
        if (number.size() <= 4) {
            std::memcpy(&value, &number[0], number.size());
        }
        for (decltype(value) leading{}; leading < zeroes - number.size(); leading++)
            value <<= 8;

        return value;
    }

    ExecutableFormat Nso::checkExecutableType() {
        u32 magic{};
        if (backing.readSome(magic) != sizeof(magic))
            return Unrecognized;
        if (magic != makeMagic("NSO0")) {
            return Unrecognized;
        }

        return ExecutableFormat::Nso;
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
        std::vector<u8> compressedChunk;
        std::span readBuffer{section};
        if (isCompressed) {
            compressedChunk.resize(compressed);
        }

        if (!compressedChunk.empty())
            readBuffer = std::span(compressedChunk);
        if (backing.readSome(readBuffer, fileOffset) != readBuffer.size()) {
            throw std::runtime_error("Failed to read some data");
        }
        if (&compressedChunk[0] != &section[0]) {
            const std::span decompress{reinterpret_cast<char*>(&readBuffer[0]), readBuffer.size()};
            const std::span output{reinterpret_cast<char*>(&section[0]), section.size()};

            if (LZ4_decompress_safe(&decompress[0], &output[0], decompress.size(), output.size()) < 1) {
                throw std::runtime_error("LZ4_decompress_safe failed");
            }
        }
        if (checkHash) {
        }
    }

    void Nso::loadExecutable() {
        backing.readSome(header);

        std::memcpy(&pumpkin[0], &header, sizeof(header));
        if (header.version != 0) {
            throw std::runtime_error("Unknown version");
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
