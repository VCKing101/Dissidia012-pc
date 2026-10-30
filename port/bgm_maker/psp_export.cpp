// "Make a PSP / PPSSPP ISO"; see psp_export.hpp. MIT licence; see
// port/README.md.

#include "psp_export.hpp"

#include "audio_file.hpp"
#include "d012_json.hpp"
#include "psp_iso.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iterator>
#include <map>
#include <set>

extern "C" {
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
}

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;
namespace psp = d012::psp_iso;
using json::Value;

#if defined(_WIN32)
constexpr const char *kEncoderFile = "atracdenc.exe";
#else
constexpr const char *kEncoderFile = "atracdenc";
#endif

std::string lower(std::string_view text) {
    std::string out(text);
    for (char &c : out)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return out;
}

// mods/enabled.json as the game reads it (host/d012_mods.cpp read_enabled);
// what it cannot read leaves every mod on.
std::map<std::string, bool> read_enabled(const std::filesystem::path &mods) {
    std::map<std::string, bool> switches;
    std::ifstream in(mods / "enabled.json", std::ios::binary);
    if (!in) return switches;
    const std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    std::string error;
    const auto document = json::parse(text, error);
    if (!document || !document->is(Value::Kind::Object)) return switches;
    const Value *list = document->find("mods");
    if (list == nullptr || !list->is(Value::Kind::Object)) return switches;
    for (const auto &[id, value] : list->members)
        if (value.is(Value::Kind::Bool)) switches[id] = value.boolean;
    return switches;
}

// A track decoded as the game plays it (PortableKit host_audio.cpp): 44.1 kHz
// stereo through swresample.
std::optional<std::vector<std::int16_t>> game_samples(const DecodedAudio &audio, std::string &error) {
    if (audio.rate == psp::kRate && audio.channels == 2) return audio.samples;
    AVChannelLayout in_layout{}, out_layout{};
    av_channel_layout_default(&in_layout, audio.channels);
    av_channel_layout_default(&out_layout, 2);
    SwrContext *resampler = nullptr;
    int result = swr_alloc_set_opts2(&resampler, &out_layout, AV_SAMPLE_FMT_S16, psp::kRate, &in_layout,
                                     AV_SAMPLE_FMT_S16, audio.rate, 0, nullptr);
    if (result >= 0) result = swr_init(resampler);
    av_channel_layout_uninit(&in_layout);
    av_channel_layout_uninit(&out_layout);
    if (result < 0) {
        swr_free(&resampler);
        error = "it cannot be resampled to 44,100 Hz";
        return std::nullopt;
    }
    std::vector<std::int16_t> out;
    const auto convert = [&](const std::int16_t *in, int frames) {
        const int room = swr_get_out_samples(resampler, frames);
        if (room <= 0) return;
        const std::size_t at = out.size();
        out.resize(at + static_cast<std::size_t>(room) * 2u);
        auto *to = reinterpret_cast<std::uint8_t *>(out.data() + at);
        const auto *from = reinterpret_cast<const std::uint8_t *>(in);
        const int made = swr_convert(resampler, &to, room, in != nullptr ? &from : nullptr, frames);
        out.resize(at + static_cast<std::size_t>(std::max(made, 0)) * 2u);
    };
    constexpr int kChunk = 1 << 16;
    for (std::int64_t frame = 0; frame < audio.frames; frame += kChunk)
        convert(audio.samples.data() + frame * audio.channels,
                static_cast<int>(std::min<std::int64_t>(kChunk, audio.frames - frame)));
    convert(nullptr, 0);  // what the resampler still holds
    swr_free(&resampler);
    return out;
}

std::int64_t game_frames(const manifest::LoopPoint &point, int source_rate) {
    if (point.unit == manifest::LoopPoint::Unit::Microseconds)
        return std::llround(static_cast<double>(point.value) * psp::kRate / 1e6);
    if (source_rate == 0) return point.value;
    return std::llround(static_cast<double>(point.value) * psp::kRate / static_cast<double>(source_rate));
}

// The loop the game plays (host_audio.cpp resolve_loop), in frames at 44.1 kHz,
// the end excluded; none: it plays once.
std::optional<std::pair<std::int64_t, std::int64_t>> game_loop(const manifest::Loop &loop, const AudioInfo &info,
                                                               int source_rate, std::int64_t frames) {
    using Mode = manifest::Loop::Mode;
    Mode mode = loop.mode;
    manifest::LoopPoint start = loop.start;
    std::optional<manifest::LoopPoint> end = loop.end;
    if (mode == Mode::Auto) {
        if (info.loop) {
            mode = Mode::Range;
            start = {manifest::LoopPoint::Unit::Samples, info.loop->start};
            end.reset();
            if (info.loop->end) end = manifest::LoopPoint{manifest::LoopPoint::Unit::Samples, *info.loop->end};
        } else {
            mode = Mode::Whole;
        }
    }
    if (mode == Mode::None) return std::nullopt;
    if (mode == Mode::Range) {
        const std::int64_t first = game_frames(start, source_rate);
        const std::int64_t last = end ? game_frames(*end, source_rate) - 1 : frames - 1;
        if (first >= 0 && first < last && last < frames) return std::pair{first, last + 1};
    }
    return std::pair<std::int64_t, std::int64_t>{0, frames};
}

bool write_file(const std::filesystem::path &path, const psp::Bytes &data) {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    out.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(data.size()));
    return static_cast<bool>(out);
}

// Runs the encoder in `folder` on files named there (plain ASCII names, which
// it takes whatever the folder's own name is), without a window.
bool run_encoder(const std::filesystem::path &program, const std::filesystem::path &folder, const std::string &in,
                 const std::string &out, const std::atomic<bool> &cancel, std::string &error) {
#if defined(_WIN32)
    std::wstring line = L"\"" + program.wstring() + L"\" -e atrac3 --container riff -i " +
                        std::wstring(in.begin(), in.end()) + L" -o " + std::wstring(out.begin(), out.end());
    STARTUPINFOW startup{};
    startup.cb = sizeof startup;
    PROCESS_INFORMATION process{};
    if (!CreateProcessW(program.c_str(), line.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr,
                        folder.c_str(), &startup, &process)) {
        error = "the encoder (" + manifest::utf8(program.filename()) + ") cannot be started";
        return false;
    }
    CloseHandle(process.hThread);
    while (WaitForSingleObject(process.hProcess, 100) == WAIT_TIMEOUT)
        if (cancel) TerminateProcess(process.hProcess, 1);  // our own child, by its handle
    DWORD code = 1;
    GetExitCodeProcess(process.hProcess, &code);
    CloseHandle(process.hProcess);
    if (code != 0 && !cancel) error = "the encoder failed (exit " + std::to_string(code) + ")";
    return code == 0;
#else
    const std::string file = manifest::utf8(program);
    const char *arguments[] = {file.c_str(), "-e", "atrac3", "--container", "riff", "-i", in.c_str(), "-o", out.c_str(), nullptr};
    const std::string where = manifest::utf8(folder);
    const SDL_PropertiesID properties = SDL_CreateProperties();
    SDL_SetPointerProperty(properties, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, const_cast<char **>(arguments));
    SDL_SetStringProperty(properties, SDL_PROP_PROCESS_CREATE_WORKING_DIRECTORY_STRING, where.c_str());
    SDL_SetNumberProperty(properties, SDL_PROP_PROCESS_CREATE_STDOUT_NUMBER, SDL_PROCESS_STDIO_NULL);
    SDL_Process *process = SDL_CreateProcessWithProperties(properties);
    SDL_DestroyProperties(properties);
    if (process == nullptr) {
        error = "the encoder cannot be started (" + std::string(SDL_GetError()) + ")";
        return false;
    }
    int code = 1;
    while (!SDL_WaitProcess(process, false, &code)) {
        if (cancel) SDL_KillProcess(process, true);
        SDL_Delay(100);
    }
    SDL_DestroyProcess(process);
    if (code != 0 && !cancel) error = "the encoder failed (exit " + std::to_string(code) + ")";
    return code == 0;
#endif
}

// One track to a file the game plays: decoded, its loop, encoded, Sony's header.
bool encode_track(const PspTrack &track, int index, const std::filesystem::path &folder,
                  const std::filesystem::path &encoder, const std::atomic<bool> &cancel, std::filesystem::path &made,
                  std::string &error) {
    const AudioInfo info = probe_audio(track.file);
    const DecodedAudio audio = decode_audio(track.file);
    if (!audio.error.empty()) {
        error = audio.error;
        return false;
    }
    auto samples = game_samples(audio, error);
    if (!samples) return false;
    const auto frames = static_cast<std::int64_t>(samples->size() / 2u);
    auto loop = game_loop(track.loop, info, audio.rate, frames);
    if (loop && loop->second - loop->first < psp::kMinLoopSamples) {
        error = "its loop is shorter than " + std::to_string(psp::kMinLoopSamples) + " samples (0.14 s), too short for the PSP";
        return false;
    }
    const std::string number = std::to_string(index);
    const std::string in = "in" + number + ".wav", raw = "enc" + number + ".at3";
    if (!write_file(folder / in, psp::wave_file(psp::encoder_input(*samples, loop)))) {
        error = "the temporary folder cannot be written (is the disk full?)";
        return false;
    }
    samples.reset();
    const bool encoded = run_encoder(encoder, folder, in, raw, cancel, error);
    std::error_code ec;
    std::filesystem::remove(folder / in, ec);
    if (!encoded) return false;
    psp::Bytes output;
    {
        std::ifstream file(folder / raw, std::ios::binary);
        output.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    }
    std::filesystem::remove(folder / raw, ec);
    const auto at3 = psp::at3_file(output, frames, loop, error);
    if (!at3) return false;
    made = folder / psp::track_file_name(index);
    if (!write_file(made, *at3)) {
        error = "the temporary folder cannot be written (is the disk full?)";
        return false;
    }
    return true;
}

std::filesystem::path program_folder() {
    const char *base = SDL_GetBasePath();
    return base == nullptr ? std::filesystem::path() : manifest::path_from_utf8(base);
}

} // namespace

std::string default_short_name(std::string_view series_name) {
    std::string name(series_name);
    while (!name.empty() && name.back() == ' ') name.pop_back();
    while (!name.empty() && name.front() == ' ') name.erase(name.begin());
    const std::string prefix = "final fantasy";
    if (lower(name).rfind(prefix, 0) == 0) {
        std::string rest = name.substr(prefix.size());
        while (!rest.empty() && rest.front() == ' ') rest.erase(rest.begin());
        name = "FF" + rest;
    }
    if (name.size() > manifest::kMaxShortNameLength) name.resize(manifest::kMaxShortNameLength);
    return name;
}

std::string psp_track_name(std::string_view short_name, std::string_view title) {
    return std::string(short_name) + " - " + std::string(title);
}

PspPlan psp_plan(const std::filesystem::path &mods) {
    PspPlan plan;
    std::error_code ec;
    if (mods.empty() || !std::filesystem::is_directory(mods, ec)) return plan;
    const auto switches = read_enabled(mods);
    std::vector<std::filesystem::path> folders;
    for (const auto &entry : std::filesystem::directory_iterator(mods, ec))
        if (entry.is_directory(ec)) folders.push_back(entry.path());
    std::sort(folders.begin(), folders.end());
    struct Loaded {
        std::string id;
        manifest::Series series;
    };
    std::vector<Loaded> all;
    std::set<std::string> ids;
    for (const auto &folder : folders) {
        const std::string name = manifest::utf8(folder.filename());
        const manifest::Result read = manifest::read(folder);
        if (!read.manifest.loaded) {
            if (std::filesystem::exists(folder / "mod.json", ec)) plan.left_out.push_back(name + ": its mod.json has errors");
            continue;
        }
        if (!ids.insert(read.manifest.id).second) {
            plan.left_out.push_back(name + ": another folder has the id \"" + read.manifest.id + "\"");
            continue;
        }
        const auto on = switches.find(read.manifest.id);
        if (on != switches.end() && !on->second) {
            plan.left_out.push_back(name + ": off in enabled.json");
            continue;
        }
        for (const auto &series : read.manifest.series) all.push_back({read.manifest.id, series});
    }
    std::sort(all.begin(), all.end(), [](const Loaded &a, const Loaded &b) {
        const std::string ida = a.id + ":" + a.series.key, idb = b.id + ":" + b.series.key;
        return a.series.order != b.series.order ? a.series.order < b.series.order : ida < idb;
    });
    // Numbers must be unique: on a clash the id that sorts later is left out (as the game does).
    std::map<std::uint32_t, std::string> numbers;
    for (const Loaded &loaded : all)
        for (const auto &track : loaded.series.tracks) {
            const std::string id = loaded.id + ":" + track.key;
            auto [at, inserted] = numbers.emplace(manifest::id_number(id), id);
            if (!inserted && id < at->second) at->second = id;
        }
    for (const Loaded &loaded : all) {
        const std::string short_name =
            loaded.series.short_name.empty() ? default_short_name(loaded.series.name) : loaded.series.short_name;
        for (const auto &track : loaded.series.tracks) {
            const std::string id = loaded.id + ":" + track.key;
            if (numbers.at(manifest::id_number(id)) != id) continue;
            plan.tracks.push_back({id, psp_track_name(short_name, track.title), track.file, track.loop});
        }
    }
    return plan;
}

std::filesystem::path encoder_program(const ModsFolder &mods) {
    std::error_code ec;
    const std::filesystem::path beside = program_folder() / kEncoderFile;
    if (std::filesystem::is_regular_file(beside, ec)) return beside;
    if (!mods.checkout.empty()) {
        const std::filesystem::path built =
            mods.checkout / "tools" / "local" / "port" / "build-atracdenc" / "src" / kEncoderFile;
        if (std::filesystem::is_regular_file(built, ec)) return built;
    }
    return {};
}

std::filesystem::path game_iso(const ModsFolder &mods) {
    std::error_code ec;
    for (const std::filesystem::path &file :
         {program_folder() / "game" / "disc.iso",
          mods.checkout.empty() ? std::filesystem::path() : mods.checkout / "build" / "port" / "game" / "disc.iso"})
        if (!file.empty() && std::filesystem::is_regular_file(file, ec)) return file;
    return {};
}

PspExport::~PspExport() {
    cancel_ = true;
    if (worker_.joinable()) worker_.join();
}

bool PspExport::start(const std::filesystem::path &original, const std::filesystem::path &out, const ModsFolder &mods,
                      std::string &error) {
    if (busy_) {
        error = "an ISO is being made already";
        return false;
    }
    if (worker_.joinable()) worker_.join();
    cancel_ = false;
    busy_ = true;
    {
        std::lock_guard lock(mutex_);
        status_ = Status{Phase::Checking, 0, 0, {}};
    }
    done_ = 0;
    worker_ = std::thread([this, original, out, mods] { run(original, out, mods); });
    return true;
}

void PspExport::cancel() { cancel_ = true; }

bool PspExport::busy() { return busy_; }

PspExport::Status PspExport::status() {
    std::lock_guard lock(mutex_);
    Status status = status_;
    if (status.phase == Phase::Checking || status.phase == Phase::Encoding || status.phase == Phase::Writing)
        status.done = done_;
    return status;
}

void PspExport::set_phase(Phase phase, std::uint64_t total) {
    std::lock_guard lock(mutex_);
    done_ = 0;
    status_.phase = phase;
    status_.total = total;
}

void PspExport::finish(Phase phase, std::string message) {
    {
        std::lock_guard lock(mutex_);
        status_.phase = cancel_ && phase == Phase::Failed ? Phase::Cancelled : phase;
        status_.message = std::move(message);
    }
    busy_ = false;
}

void PspExport::run(std::filesystem::path original, std::filesystem::path out, ModsFolder mods) {
    const psp::Progress progress = [this](std::uint64_t done, std::uint64_t) {
        done_ = done;
        return !cancel_;
    };
    std::string error;
    // What goes in, and the original's checks.
    const PspPlan plan = psp_plan(mods.path);
    if (plan.tracks.empty()) return finish(Phase::Failed, "no mod in the mods folder has a track the game would play");
    const std::filesystem::path encoder = encoder_program(mods);
    if (encoder.empty()) return finish(Phase::Failed, std::string("the encoder (") + kEncoderFile + ") is not beside the Battle BGM Maker");
    std::error_code ec;
    set_phase(Phase::Checking, std::filesystem::file_size(original, ec));
    if (!psp::check_retail(original, progress, error)) return finish(Phase::Failed, "Your ISO: " + error + ".");
    const auto disc = psp::read_disc_music(original, error);
    if (!disc) return finish(Phase::Failed, "Your ISO: " + error + ".");
    if (static_cast<int>(plan.tracks.size()) > disc->room())
        return finish(Phase::Failed, "The game's list has room for " + std::to_string(disc->room()) + " more tracks; the mods have " +
                                         std::to_string(plan.tracks.size()) + ". Turn some mods off in enabled.json.");

    // Encoded in a folder of its own in the temporary folder, on workers.
    const std::filesystem::path folder = std::filesystem::temp_directory_path(ec) / "d012-bgm-maker" /
        ("psp-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(folder, ec);
    if (ec) return finish(Phase::Failed, "The temporary folder cannot be made.");
    set_phase(Phase::Encoding, plan.tracks.size());
    std::vector<std::filesystem::path> made(plan.tracks.size());
    std::vector<std::string> errors(plan.tracks.size());
    std::atomic<std::size_t> next{0};
    std::atomic<bool> failed{false};
    const unsigned workers = std::clamp(std::thread::hardware_concurrency() / 2u, 1u, 6u);
    std::vector<std::thread> pool;
    for (unsigned w = 0; w < workers; ++w)
        pool.emplace_back([&] {
            for (std::size_t i; !cancel_ && !failed && (i = next++) < plan.tracks.size();) {
                if (encode_track(plan.tracks[i], static_cast<int>(i), folder, encoder, cancel_, made[i], errors[i])) {
                    ++done_;
                } else {
                    failed = true;
                    cancel_ = true;  // stops the other workers' encoders
                }
            }
        });
    for (std::thread &t : pool) t.join();
    const auto clean = [&] { std::filesystem::remove_all(folder, ec); };
    for (std::size_t i = 0; i < plan.tracks.size(); ++i)
        if (!errors[i].empty() && errors[i] != "cancelled") {
            clean();
            cancel_ = false;
            return finish(Phase::Failed, plan.tracks[i].id + " (" + manifest::utf8(plan.tracks[i].file.filename()) + "): " +
                                             errors[i] + ".");
        }
    if (cancel_) {
        clean();
        return finish(Phase::Cancelled, "Cancelled; nothing was written.");
    }

    // The image.
    std::vector<psp::Track> tracks;
    std::uint64_t bytes = std::filesystem::file_size(original, ec);
    for (std::size_t i = 0; i < plan.tracks.size(); ++i) {
        tracks.push_back({plan.tracks[i].name, made[i]});
        bytes += std::filesystem::file_size(made[i], ec);
    }
    const auto space = std::filesystem::space(out.parent_path().empty() ? std::filesystem::path(".") : out.parent_path(), ec);
    if (!ec && space.available < bytes + (16u << 20u)) {
        clean();
        return finish(Phase::Failed, "There is not enough room for the new ISO there (it needs " +
                                         std::to_string((bytes >> 20u) + 1u) + " MB).");
    }
    set_phase(Phase::Writing, bytes);
    const bool written = psp::make_iso(original, out, tracks, progress, error);
    clean();
    if (!written) return finish(Phase::Failed, error == "cancelled" ? "Cancelled; nothing was written." : "Not made: " + error + ".");
    finish(Phase::Done, "Made " + manifest::utf8(out) + " with " + std::to_string(tracks.size()) + " track(s) added.");
}

} // namespace d012::bgm_maker
