#include "file.hpp"

#include "common.hpp"
#include "ftrace.hpp"
#include "log.hpp"

#include <filesystem>

String Tools::loadFile(StringView filename) {
    ftrace();
    File f {filename, File::Read | File::Binary};

    return f.read();
}

void Tools::saveFile(StringView data, StringView filename) {
    ftrace();
    File f {filename, File::Write | File::Binary};
    f.write(data);
}


Tools::File::File(StringView filename, uint8_t state) {
    ftrace();
    const char *name = svToCharPtr(alloca(filename.size() + 1), filename);
    const char *mode = stateToMode(state);
    if (!mode) {
        crash("failed to set mode from provided state");
    }
    m_fp = fopen(name, mode);
    if (!m_fp) {
        crash("File error: {}: {}", std::strerror(errno), filename);
    }
}

Tools::File::~File() {
    ftrace();
    std::fclose(m_fp);
}

String Tools::File::read() {
    ftrace();
    std::fseek(m_fp, 0l, SEEK_END);
    const size_t fsize = to<size_t>(std::ftell(m_fp));
    std::rewind(m_fp);

    String out(fsize, '\0');
    auto ret = std::fread(out.data(), sizeof(char), fsize, m_fp);
    if(ret < fsize){
        error("File error: {}", std::strerror(errno));
    }
    debug("file size = {}", fsize);
    debug("data = {}", out);
    debug("ret = {}", ret);
    return out;
}

void Tools::File::write(StringView data) {
    ftrace();
    if (data.size() < maxStackString) {
        writeImpl(svToCharPtr(alloca(data.size() + 1), data), data.size());
    } else {
        write(String {data});
    }
}

void Tools::File::write(const String &data) {
    ftrace();
    writeImpl(data.data(), data.size());
}
void Tools::File::writeImpl(const char *data, size_t size) {
    ftrace();
    if(std::fwrite(data, sizeof(char), size, m_fp) < size){
        error("File error: {}", std::strerror(errno));
    }
}

const char *Tools::File::stateToMode(uint8_t state) {
    ftrace();
    static char m[4] = {0};
    auto *it = std::begin(m);
    const auto stateRep = std::bit_cast<StateRepr>(state);
    if (stateRep.padding || (stateRep.write && stateRep.append) || !(stateRep.read || stateRep.write || stateRep.append)) {
        error("Illegal state for opening a file: {:#b}", state);
        return nullptr;
    }
    if (stateRep.write) {
        *it++ = 'w';
    } else if (stateRep.append) {
        *it++ = 'a';
    }

    if (stateRep.read) {
        *it++ = it == std::begin(m) ? 'r' : '+';
    }

    if (stateRep.binary) {
        *it++ = 'b';
    }
    for (; it != std::end(m); *it = 0, it++) { }
    return m;
}
char *Tools::File::svToCharPtr(void *dest, StringView source) {
    ftrace();
    std::memcpy(dest, &*source.begin(), source.size() * sizeof(char));

    static_cast<char *>(dest)[source.size()] = 0;
    return static_cast<char *>(dest);
}
