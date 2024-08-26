#include <filesystem>
#include <fstream>
#include <vector>
#include <ranges>

#include <cstring>
#include <unistd.h>
#include <sys/stat.h>

#include <vfs/mappable.h>
namespace mizui::vfs {
    Mappable::Mappable(std::fstream& io) {
        rdPos = io.tellg();
        wrPos = io.tellp();

        io.seekg(0, std::ios::beg);
        std::vector<char> testBytes(100);
        std::vector<char> srcBytes(100);
        io.read(&testBytes[0], testBytes.size());

        std::filesystem::directory_iterator opened("/proc/self/fd");
        if (opened == std::filesystem::directory_iterator{})
            return;
        std::stringstream ss;

        for (const auto& fd : opened | std::views::drop(3)) {
            ss.clear();
            std::string sysDescriptor{fd.path()};
            sysDescriptor.erase(0, sysDescriptor.find_last_of("/") + 1);
            ss << sysDescriptor;

            i32 target{};
            ss >> target;

            if (target < 3)
                continue;
            ::pread(target, &srcBytes[0], srcBytes.size(), 0);
            if (std::memcmp(&testBytes[0], &srcBytes[0], srcBytes.size()))
                continue;

            descriptor = target;
            break;
        }

        io.seekg(rdPos);
        io.seekp(wrPos);
    }

    Mappable::operator bool() const {
        return descriptor > 3;
    }

    u64 Mappable::readSome(const std::span<u8> output, u64 offset) const {
        constexpr auto bufferingSize{4096};
        u64 readSize{bufferingSize};

        decltype(offset) buffPos{};
        do {
            const auto fence{output.size() - buffPos};
            if (fence < readSize)
                readSize = fence;
            decltype(buffPos) result;
            if ((result = ::pread64(descriptor, &output[buffPos], readSize, offset + buffPos)) < 1) {
                throw std::runtime_error("Could not read the file");
            }

            buffPos += result;
        } while (buffPos < output.size());
        return buffPos;
    }

    u64 Mappable::size() const {
        struct stat64 ios;
        const auto result{fstat64(descriptor, &ios)};
        if (result != 0)
            return u64{};

        return ios.st_size;
    }
}
