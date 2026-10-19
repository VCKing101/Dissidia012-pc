// Nintendo stream headers: see d012_nintendo_stream.hpp. Layouts from the
// public format pages (BRSTM on the Mario Kart Wii wiki, BCSTM on 3dbrew,
// BFSTM on the Mario Kart 8 wiki), followed through the headers' references.
// MIT licence; see port/README.md.

#include "d012_nintendo_stream.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <vector>

namespace d012::nintendo_stream {
namespace {

constexpr std::size_t kHeaderSize = 0x40u;
constexpr std::size_t kMaxHeader = 1u << 20u;

// Reads the header's numbers in its byte order; a read past the end sets
// `cut` and gives 0.
class Reader {
public:
    Reader(std::span<const std::uint8_t> data, bool big) : data_(data), big_(big) {}
    [[nodiscard]] std::uint32_t u8(std::size_t at) { return get(at, 1u); }
    [[nodiscard]] std::uint32_t u16(std::size_t at) { return get(at, 2u); }
    [[nodiscard]] std::uint32_t u32(std::size_t at) { return get(at, 4u); }
    [[nodiscard]] bool cut() const noexcept { return cut_; }

private:
    std::uint32_t get(std::size_t at, std::size_t size) {
        if (at > data_.size() || data_.size() - at < size) {
            cut_ = true;
            return 0u;
        }
        std::uint32_t value = 0u;
        for (std::size_t i = 0; i < size; ++i) {
            const std::uint32_t byte = data_[at + (big_ ? i : size - 1u - i)];
            value = (value << 8u) | byte;
        }
        return value;
    }
    std::span<const std::uint8_t> data_;
    bool big_;
    bool cut_{};
};

bool magic_at(std::span<const std::uint8_t> data, std::size_t at, const char *magic) {
    return at <= data.size() && data.size() - at >= 4u && std::memcmp(data.data() + at, magic, 4u) == 0;
}

// The byte order mark: the bytes FE FF (big-endian) or FF FE (little-endian).
std::optional<bool> big_endian(std::span<const std::uint8_t> data) {
    if (data[4] == 0xFEu && data[5] == 0xFFu) return true;
    if (data[4] == 0xFFu && data[5] == 0xFEu) return false;
    return std::nullopt;
}

bool check(const StreamInfo &info, bool cut, std::string &error) {
    const std::string what = info.format + " file";
    if (cut) error = "not a valid " + what + ": its header is cut short";
    else if (info.channels == 0u) error = "not a valid " + what + ": no channels";
    else if (info.rate < 1000u || info.rate > 384000u)
        error = "not a valid " + what + ": a sample rate of " + std::to_string(info.rate) + " Hz";
    else if (info.samples == 0u) error = "not a valid " + what + ": no samples";
    else if (info.codec != 2)
        error = "this " + what + " holds " + codec_name(info.codec) + " audio, which FFmpeg (and so the port) " +
                (info.codec == 0 || info.codec == 1 ? "does not decode correctly" : "cannot decode") +
                "; only DSP-ADPCM streams are supported: use a DSP-ADPCM BRSTM of the track, or a WAV or FLAC with "
                "the loop set in the mod";
    else return true;
    return false;
}

} // namespace

bool is_stream_extension(std::string_view extension) {
    return extension == ".brstm" || extension == ".bcstm" || extension == ".bfstm";
}

const char *codec_name(int codec) {
    switch (codec) {
    case 0: return "PCM8";
    case 1: return "PCM16";
    case 2: return "DSP-ADPCM";
    case 3: return "IMA-ADPCM";
    default: return "an unknown codec's";
    }
}

std::optional<StreamInfo> parse_header(std::span<const std::uint8_t> data, std::string &error) {
    if (data.size() < kHeaderSize) {
        error = "not a BRSTM, BCSTM or BFSTM file: shorter than a header";
        return std::nullopt;
    }
    StreamInfo info;
    const bool brstm = magic_at(data, 0u, "RSTM");
    if (brstm) info.format = "BRSTM";
    else if (magic_at(data, 0u, "CSTM")) info.format = "BCSTM";
    else if (magic_at(data, 0u, "FSTM")) info.format = "BFSTM";
    else {
        error = "not a BRSTM, BCSTM or BFSTM file";
        return std::nullopt;
    }
    const auto big = big_endian(data);
    if (!big) {
        error = "not a valid " + info.format + " file: no byte order mark";
        return std::nullopt;
    }
    info.big_endian = *big;
    Reader r(data, *big);
    std::size_t stream = 0u;
    if (brstm) {
        // HEAD: three references (type, data type, padding, offset from HEAD + 8);
        // the first is the stream info.
        const std::size_t head = r.u32(0x10u);
        if (!magic_at(data, head, "HEAD")) {
            error = "not a valid BRSTM file: no HEAD chunk where its header says";
            return std::nullopt;
        }
        stream = head + 8u + r.u32(head + 0x0Cu);
        info.codec = static_cast<int>(r.u8(stream));
        info.loop_flag = r.u8(stream + 1u) != 0u;
        info.channels = r.u8(stream + 2u);
        info.rate = r.u16(stream + 4u);  // 16 bits, as FFmpeg reads it
        info.loop_start = r.u32(stream + 8u);
        info.samples = r.u32(stream + 0x0Cu);
    } else {
        // Section references (type, padding, offset, size) from 0x14; 0x4000 is INFO.
        std::optional<std::size_t> info_at;
        const std::uint32_t sections = std::min<std::uint32_t>(r.u16(0x10u), 16u);
        for (std::uint32_t i = 0; i < sections; ++i)
            if (r.u16(0x14u + 12u * i) == 0x4000u) info_at = r.u32(0x14u + 12u * i + 4u);
        if (!info_at || !magic_at(data, *info_at, "INFO")) {
            error = "not a valid " + info.format + " file: no INFO block where its header says";
            return std::nullopt;
        }
        // INFO + 8: a reference (0x4100, padding, offset from INFO + 8) to the stream info.
        if (r.u16(*info_at + 8u) != 0x4100u) {
            error = "not a valid " + info.format + " file: its INFO block has no stream info";
            return std::nullopt;
        }
        stream = *info_at + 8u + r.u32(*info_at + 0x0Cu);
        info.codec = static_cast<int>(r.u8(stream));
        info.loop_flag = r.u8(stream + 1u) != 0u;
        info.channels = r.u8(stream + 2u);
        info.rate = r.u32(stream + 4u);
        info.loop_start = r.u32(stream + 8u);
        info.samples = r.u32(stream + 0x0Cu);
    }
    if (!check(info, r.cut(), error)) return std::nullopt;
    return info;
}

std::optional<StreamInfo> read_header(const std::filesystem::path &file, std::string &error) {
    std::ifstream in(file, std::ios::binary);
    std::vector<std::uint8_t> data(kHeaderSize);
    if (!in.read(reinterpret_cast<char *>(data.data()), static_cast<std::streamsize>(data.size()))) {
        error = "not a BRSTM, BCSTM or BFSTM file: shorter than a header";
        return std::nullopt;
    }
    // How far the header's own block reaches: HEAD for a BRSTM, INFO otherwise.
    std::size_t end = kHeaderSize;
    if (const auto big = big_endian(data)) {
        Reader r(data, *big);
        if (magic_at(data, 0u, "RSTM")) {
            end = static_cast<std::size_t>(r.u32(0x10u)) + r.u32(0x14u);
        } else {
            for (std::uint32_t i = 0; i < std::min<std::uint32_t>(r.u16(0x10u), 4u); ++i)
                if (r.u16(0x14u + 12u * i) == 0x4000u)
                    end = static_cast<std::size_t>(r.u32(0x14u + 12u * i + 4u)) + r.u32(0x14u + 12u * i + 8u);
        }
    }
    end = std::clamp(end, kHeaderSize, kMaxHeader);
    data.resize(end);
    in.read(reinterpret_cast<char *>(data.data() + kHeaderSize), static_cast<std::streamsize>(end - kHeaderSize));
    data.resize(kHeaderSize + static_cast<std::size_t>(in.gcount()));
    return parse_header(data, error);
}

} // namespace d012::nintendo_stream
