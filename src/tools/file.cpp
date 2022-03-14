#include "file.hpp"

#include "common.hpp"
#include "ftrace.hpp"
#include "log.hpp"

namespace Tools {


FileErrorOr<String> File::readFile(StringView filename) noexcept {
    ftrace();
    if (filename.empty()) {
        return tl::unexpected(FileError {"Path is an empty string", 0});
    }
    return File::open(filename, File::Read | File::Binary).and_then([](File &&f) { return f.read(); });
}

FileErrorOr<void> File::writeFile(StringView filename, StringView data) noexcept {
    ftrace();
    if (filename.empty()) {
        return tl::unexpected(FileError {"Path is an empty string", 0});
    }
    return File::open(filename, File::Write | File::Binary).and_then([&data](File &&f) { return f.write(data); });
}

File::File(FILE *fp) noexcept
        : m_fp(fp) { }

File::File(File &&other) noexcept
        : m_fp(other.m_fp) {
    other.m_fp = nullptr;
}


FileErrorOr<File> File::open(StringView filename, uint8_t state) noexcept {
    ftrace();
    if (filename.empty()) {
        return tl::unexpected(FileError {"Path is an empty string", 0});
    }
    return stateToMode(state).and_then([&filename](const char *mode) -> FileErrorOr<File> {
        return svToCharPtr(svalloca(filename), filename).and_then([mode](const char *name) -> FileErrorOr<File> {
            auto *fp = fopen(name, mode);
            if (!fp) {
                return tl::unexpected {
                    FileError {"Could not open file", errno}
                };
            }
            scfreea(name);
            return File {fp};
        });
    });
}

FileErrorOr<void> File::close() noexcept {
    ftrace();
    if (m_fp && std::fclose(m_fp)) {
        return tl::unexpected {
            FileError {"Could not close file", errno}
        };
    }
    return {};
}

File::~File() noexcept {
    ftrace();
    close().map_error([](const FileError &err) { spdlog::error("{}: {}", err.errStr, std::strerror(err.errc)); });
}

FileErrorOr<String> File::read() noexcept {
    ftrace();
    std::fseek(m_fp, 0l, SEEK_END);
    const size_t fsize = to<size_t>(std::ftell(m_fp));
    std::rewind(m_fp);

    String out(fsize, '\0');
    auto ret = std::fread(out.data(), sizeof(char), fsize, m_fp);
    if (ret < fsize) {
        return tl::unexpected(FileError {"Could not read from file", errno});
    }
    return out;
}

FileErrorOr<void> File::write(StringView data) noexcept {
    ftrace();
    if (data.empty()) {
        return write(nullptr, 0);
    }
    if (data.size() < maxStackString) {
        return svToCharPtr(svalloca(data), data).and_then([&data, this](const char *d) {
            const auto res = write(d, data.size());
            scfreea(d);
            return res;
        });
    } else {
        return write(String {data});
    }
}

FileErrorOr<void> File::write(const String &data) noexcept {
    ftrace();
    if (data.empty()) {
        return write(nullptr, 0);
    }
    return write(data.c_str(), data.size());
}

FileErrorOr<void> File::write(const char *data, size_t size) noexcept {
    ftrace();
    if (data == nullptr || size == 0) {
        return {};
    }
    if (std::fwrite(data, sizeof(char), size, m_fp) < size) {
        return tl::unexpected(FileError {"Could not write to file", errno});
    }
    return {};
}

FileErrorOr<const char *> File::stateToMode(uint8_t state) noexcept {
    ftrace();
    static char m[4] = {0};
    auto *it = std::begin(m);
    const auto stateRep = std::bit_cast<StateRepr>(state);
    if (stateRep.padding || (stateRep.write && stateRep.append) || !(stateRep.read || stateRep.write || stateRep.append)) {
        return tl::unexpected(FileError {"Illegal state for opening a file", 0});
    }
    if (stateRep.write) {
        *it++ = 'w';
    } else if (stateRep.append) {
        *it++ = 'a';
    }

    if (stateRep.read) {
        *it = it == std::begin(m) ? 'r' : '+';
        it++;
    }

    if (stateRep.binary) {
        *it++ = 'b';
    }
    for (; it != std::end(m); *it = 0, it++) { }
    return m;
}


FileErrorOr<const char *> File::svToCharPtr(void *dest, StringView source) noexcept {
    ftrace();
    if (!dest) {
        return tl::unexpected(FileError {"Destination is null. Possible failed allocation", errno});
    }
    if (source.empty()) {
        return tl::unexpected(FileError {"Source is empty", 0});
    }
    std::memcpy(dest, &*source.begin(), source.size() * sizeof(StringView::value_type));

    static_cast<char *>(dest)[source.size()] = 0;
    return static_cast<char *>(dest);
}
}  // namespace Tools
