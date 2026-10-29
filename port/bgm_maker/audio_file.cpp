// An audio file as the Battle BGM Maker sees it; see audio_file.hpp. MIT
// licence; see port/README.md.

#include "audio_file.hpp"

#include "d012_mod_manifest.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
#include <libavutil/mem.h>
}

namespace d012::bgm_maker {
namespace {

std::string av_error(int code) {
    char text[AV_ERROR_MAX_STRING_SIZE]{};
    av_strerror(code, text, sizeof text);
    return text;
}

// A file opened by FFmpeg through the host's I/O (wide paths on Windows).
class Input {
public:
    ~Input() {
        if (format_ != nullptr) avformat_close_input(&format_);
        if (io_ != nullptr) {
            av_freep(&io_->buffer);
            avio_context_free(&io_);
        }
        if (file_ != nullptr) std::fclose(file_);
    }

    bool open(const std::filesystem::path &path, std::string &error) {
#if defined(_WIN32)
        file_ = _wfopen(path.c_str(), L"rb");
#else
        file_ = std::fopen(path.c_str(), "rb");
#endif
        if (file_ == nullptr) {
            error = "cannot open the file";
            return false;
        }
        std::error_code ec;
        size_ = static_cast<std::int64_t>(std::filesystem::file_size(path, ec));
        constexpr int kBufferSize = 64 * 1024;
        auto *buffer = static_cast<unsigned char *>(av_malloc(kBufferSize));
        io_ = buffer != nullptr ? avio_alloc_context(buffer, kBufferSize, 0, this, &read, nullptr, &seek) : nullptr;
        format_ = io_ != nullptr ? avformat_alloc_context() : nullptr;
        if (format_ == nullptr) {
            if (io_ == nullptr) av_free(buffer);
            error = "out of memory";
            return false;
        }
        format_->pb = io_;
        format_->flags |= AVFMT_FLAG_CUSTOM_IO;
        const std::string name = mod_manifest::utf8(path.filename());
        if (const int result = avformat_open_input(&format_, name.c_str(), nullptr, nullptr); result < 0) {
            error = "not a file FFmpeg can read (" + av_error(result) + ")";
            return false;
        }
        if (const int result = avformat_find_stream_info(format_, nullptr); result < 0) {
            error = "cannot read the streams (" + av_error(result) + ")";
            return false;
        }
        const AVCodec *codec = nullptr;
        stream_ = av_find_best_stream(format_, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);
        if (stream_ < 0 || codec == nullptr) {
            error = stream_ == AVERROR_DECODER_NOT_FOUND ? "no decoder for its audio in this build" : "no audio stream";
            return false;
        }
        return true;
    }

    [[nodiscard]] AVFormatContext *format() const noexcept { return format_; }
    [[nodiscard]] AVStream *stream() const noexcept { return format_->streams[stream_]; }

    // A tag of the file, else of its audio stream (names compared without
    // case, as FFmpeg does), as the game's host_audio.cpp looks them up.
    [[nodiscard]] const char *tag(const char *key) const {
        for (const AVDictionary *dictionary : {format_->metadata, stream()->metadata}) {
            const AVDictionaryEntry *entry = av_dict_get(dictionary, key, nullptr, 0);
            if (entry != nullptr && entry->value != nullptr) return entry->value;
        }
        return nullptr;
    }

    // A whole number of 0 or more, as host_audio.cpp's integer_tag reads it.
    [[nodiscard]] std::optional<std::int64_t> integer_tag(const char *key) const {
        for (const AVDictionary *dictionary : {format_->metadata, stream()->metadata}) {
            const AVDictionaryEntry *entry = av_dict_get(dictionary, key, nullptr, 0);
            if (entry == nullptr || entry->value == nullptr) continue;
            char *end = nullptr;
            const long long value = std::strtoll(entry->value, &end, 10);
            if (end != entry->value && value >= 0) return value;
        }
        return std::nullopt;
    }

private:
    static int read(void *opaque, std::uint8_t *buffer, int size) {
        auto *self = static_cast<Input *>(opaque);
        const std::size_t count = std::fread(buffer, 1u, static_cast<std::size_t>(size), self->file_);
        return count == 0u ? AVERROR_EOF : static_cast<int>(count);
    }
    static std::int64_t seek(void *opaque, std::int64_t offset, int whence) {
        auto *self = static_cast<Input *>(opaque);
        if ((whence & AVSEEK_SIZE) != 0) return self->size_;
        whence &= ~AVSEEK_FORCE;
#if defined(_WIN32)
        if (_fseeki64(self->file_, offset, whence) != 0) return -1;
        return _ftelli64(self->file_);
#else
        if (fseeko(self->file_, static_cast<off_t>(offset), whence) != 0) return -1;
        return static_cast<std::int64_t>(ftello(self->file_));
#endif
    }

    std::FILE *file_{};
    std::int64_t size_{-1};
    AVIOContext *io_{};
    AVFormatContext *format_{};
    int stream_{-1};
};

std::string lower(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return text;
}

std::string trimmed(const char *text) {
    std::string out = text != nullptr ? text : "";
    const auto first = out.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return {};
    return out.substr(first, out.find_last_not_of(" \t\r\n") - first + 1u);
}

std::string with_commas(long long value) {
    std::string digits = std::to_string(value);
    for (int at = static_cast<int>(digits.size()) - 3; at > 0; at -= 3) digits.insert(static_cast<std::size_t>(at), ",");
    return digits;
}

} // namespace

AudioInfo probe_audio(const std::filesystem::path &file) {
    AudioInfo info;
    const std::string extension = lower(mod_manifest::utf8(file.extension()));
    if (nintendo_stream::is_stream_extension(extension)) {
        std::string error;
        info.stream = nintendo_stream::read_header(file, error);
        if (!info.stream) {
            info.error = error;
            return info;
        }
        const nintendo_stream::StreamInfo &s = *info.stream;
        const char *platform = s.format == "BRSTM" ? "Wii" : s.format == "BCSTM" ? "3DS" : s.big_endian ? "Wii U" : "Switch";
        info.stream_description = s.format + " (" + platform + ", " + (s.big_endian ? "big-endian" : "little-endian") +
                                  "), " + nintendo_stream::codec_name(s.codec);
        if (s.loop_flag && s.loop_start < s.samples)
            info.loop = FileLoop{s.loop_start, static_cast<std::int64_t>(s.samples), "the " + s.format + " header"};
        else if (s.loop_flag)
            info.stream_problem = "the " + s.format + " header's loop starts at sample " + std::to_string(s.loop_start) +
                                  ", at or past the end of its " + std::to_string(s.samples) + " samples";
    }
    Input input;
    if (!input.open(file, info.error)) return info;
    const AVCodecParameters *codec = input.stream()->codecpar;
    info.codec = avcodec_get_name(codec->codec_id);
    info.rate = codec->sample_rate;
    info.channels = codec->ch_layout.nb_channels;
    const AVStream *stream = input.stream();
    if (stream->duration != AV_NOPTS_VALUE && stream->duration > 0)
        info.seconds = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    else if (input.format()->duration != AV_NOPTS_VALUE && input.format()->duration > 0)
        info.seconds = static_cast<double>(input.format()->duration) / AV_TIME_BASE;
    info.title = trimmed(input.tag("title"));
    // FFmpeg's own "loop_start" tag on Nintendo streams is in microseconds;
    // their header gave the loop above.
    if (!info.stream) {
        if (const auto start = input.integer_tag("LOOPSTART")) {
            const auto length = input.integer_tag("LOOPLENGTH");
            const auto end = input.integer_tag("LOOPEND");
            FileLoop loop{*start, std::nullopt, "the file's loop tags"};
            if (length && *length > 0) loop.end = *start + *length;
            else if (end) loop.end = *end;
            info.loop = loop;
        }
    }
    return info;
}

std::string describe(const AudioInfo &info) {
    if (!info.error.empty()) return info.error;
    std::string codec = info.stream ? info.stream_description : info.codec;
    if (!info.stream) {
        if (codec == "vorbis") codec = "Ogg Vorbis";
        else if (codec == "opus") codec = "Opus";
        else if (codec == "flac") codec = "FLAC";
        else if (codec.rfind("pcm_", 0) == 0) codec = "WAV (" + codec.substr(4u) + ")";
        else
            for (char &c : codec) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    std::string line = codec + ", " + with_commas(info.rate) + " Hz, ";
    line += info.channels == 1 ? "mono" : info.channels == 2 ? "stereo" : std::to_string(info.channels) + " channels";
    if (info.seconds > 0.0) {
        const long long total = std::llround(info.seconds);
        char length[32];
        std::snprintf(length, sizeof length, ", %lld:%02lld", total / 60, total % 60);
        line += length;
    }
    return line;
}

} // namespace d012::bgm_maker
