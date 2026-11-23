// Package mods: a mod's files served in place of entries of the game's
// PACKAGE.BIN; see d012_package_mods.hpp. The plan (which bytes the game
// reads where) is d012_package_index.cpp's; this file reads the mods'
// folders and hands the game the plan through PortableKit's file providers.
// MIT licence; see port/README.md.

#include "d012_package_mods.hpp"

#include "d012_model_memory.hpp"
#include "d012_package_index.hpp"

#include "hle/hle_common.hpp"
#include "install/user_data.hpp"
#include "portablekit/hle_extension.hpp"
#include "profile.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>

namespace d012::package_mods {
namespace {

namespace pix = package_index;
using Filter = portablekit::hle_extension::FileFilter;

std::string utf8(const std::filesystem::path &path) { return portablekit::install::path_to_utf8(path); }

std::string lower(std::string_view text) {
    std::string out(text);
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return out;
}

std::string hex8(std::uint32_t value) {
    char text[9];
    std::snprintf(text, sizeof text, "%08X", value);
    return text;
}

struct File {
    std::uint32_t crc{};
    std::string name;  // "<mod id>/<path in its package folder>", for the log
    std::shared_ptr<const std::vector<std::uint8_t>> bytes;
};

struct State {
    std::vector<File> files;  // in the order they win
    const File *largest_model{};  // the biggest obj/*.gmo or *.objx among them
    std::vector<std::pair<std::string, LateSource>> late;  // asked at the first open of the package
    bool tried{};             // the plan was made (or could not be)
    std::shared_ptr<const pix::Plan> plan;  // null: nothing to serve
    long trace_left{};        // D012_TRACE_PACKAGE_MODS: reads of mod files still to log
};

State &state() {
    static State s;
    return s;
}

// charmod.py's own notes in its output folder: not package entries.
bool charmod_note(std::string_view relative) {
    return relative == "roster.txt" || (relative.starts_with("readme_charmod_") && relative.ends_with(".json"));
}

// by_crc/<8 hex digits>.bin: the entry with that CRC.
std::optional<std::uint32_t> by_crc(std::string_view relative) {
    constexpr std::string_view kFolder = "by_crc/";
    if (!relative.starts_with(kFolder) || !relative.ends_with(".bin") || relative.size() != kFolder.size() + 12u)
        return std::nullopt;
    const std::string_view digits = relative.substr(kFolder.size(), 8u);
    std::uint32_t value = 0u;
    const auto [end, error] = std::from_chars(digits.data(), digits.data() + digits.size(), value, 16);
    if (error != std::errc{} || end != digits.data() + digits.size()) return std::nullopt;
    return value;
}

std::shared_ptr<const std::vector<std::uint8_t>> read_whole(const std::filesystem::path &path, std::string &error) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        error = "cannot be opened";
        return nullptr;
    }
    auto bytes = std::make_shared<std::vector<std::uint8_t>>((std::istreambuf_iterator<char>(in)),
                                                             std::istreambuf_iterator<char>());
    if (in.bad()) {
        error = "cannot be read";
        return nullptr;
    }
    return bytes;
}

// Every file of every source, the first source to name an entry keeping it.
void collect(const std::vector<Source> &sources) {
    std::map<std::uint32_t, std::size_t> owner;  // CRC -> its file in state().files
    std::vector<std::size_t> models;             // into state().files
    std::uint64_t total = 0u;
    for (const Source &source : sources) {
        std::vector<std::filesystem::path> paths;
        std::error_code ec;
        for (auto it = std::filesystem::recursive_directory_iterator(source.folder, ec);
             !ec && it != std::filesystem::recursive_directory_iterator(); it.increment(ec))
            if (it->is_regular_file(ec)) paths.push_back(it->path());
        if (ec)
            std::cerr << "[mods] warning: " << source.mod_id << ": its package folder cannot be read whole ("
                      << ec.message() << ")\n";
        std::sort(paths.begin(), paths.end());
        for (const auto &path : paths) {
            const std::string inside = utf8(path.lexically_relative(source.folder).generic_u8string());
            const std::string relative = lower(inside);
            const std::string name = source.mod_id + "/" + inside;
            if (charmod_note(relative)) continue;
            File file;
            file.name = name;
            if (const auto crc = by_crc(relative)) {
                file.crc = *crc;
            } else {
                file.crc = pix::path_crc(relative);
            }
            std::string error;
            file.bytes = read_whole(path, error);
            if (!file.bytes) {
                std::cerr << "[mods] error: " << name << " " << error << "; it is left out\n";
                continue;
            }
            if (const auto taken = owner.find(file.crc); taken != owner.end()) {
                // charmod.py writes main.bin into every mod it builds, the
                // same bytes each time: the same file twice is no clash.
                const File &first = state().files[taken->second];
                if (*first.bytes != *file.bytes)
                    std::cerr << "[mods] error: " << name << " replaces the same package entry as " << first.name
                              << "; it is left out\n";
                continue;
            }
            if (relative.starts_with("obj/") && (relative.ends_with(".gmo") || relative.ends_with(".objx")))
                models.push_back(state().files.size());
            owner.emplace(file.crc, state().files.size());
            total += file.bytes->size();
            state().files.push_back(std::move(file));
        }
    }
    for (const std::size_t i : models) {
        const File &file = state().files[i];
        if (!state().largest_model || file.bytes->size() > state().largest_model->bytes->size())
            state().largest_model = &file;
    }
    std::cout << "[mods] package: " << state().files.size() << " file(s) from " << sources.size() << " mod(s), "
              << total << " bytes\n";
}

// The late sources' files, after the mods': a mod's file of the same entry
// stays.
void collect_late() {
    State &s = state();
    const auto taken = [&s](std::uint32_t crc) {
        return std::any_of(s.files.begin(), s.files.end(), [crc](const File &file) { return file.crc == crc; });
    };
    for (const auto &[name, source] : s.late) {
        std::size_t added = 0u;
        std::uint64_t total = 0u;
        for (LateFile &given : source(taken)) {
            File file;
            const std::string relative = lower(given.path);
            const auto crc = by_crc(relative);
            file.crc = crc ? *crc : pix::path_crc(relative);
            file.name = name + "/" + given.path;
            file.bytes = std::move(given.bytes);
            if (!file.bytes || taken(file.crc)) continue;
            total += file.bytes->size();
            s.files.push_back(std::move(file));
            ++added;
        }
        if (added != 0u)
            std::cout << "[mods] package: " << added << " file(s) from " << name << ", " << total << " bytes" << std::endl;
    }
    s.late.clear();
}

// The plan for this index, made once, with its problems said.
void make_plan(std::span<const std::uint8_t> index_bytes, std::uint64_t package_size) {
    State &s = state();
    s.tried = true;
    collect_late();
    if (s.files.empty()) return;
    std::string error;
    const auto index = pix::parse(index_bytes, error);
    if (!index) {
        std::cerr << "[mods] error: the game's PACKAGE_INFO.BIN cannot be read (" << error
                  << "); no package mod is used\n";
        return;
    }
    std::vector<pix::Replacement> replacements;
    for (const File &file : s.files) replacements.push_back({file.crc, file.bytes, file.name});
    auto made = std::make_shared<pix::Plan>(pix::plan(*index, package_size, replacements));
    const auto name_of = [&](std::uint32_t crc) {
        for (const File &file : s.files)
            if (file.crc == crc) return file.name;
        return hex8(crc);
    };
    for (const std::uint32_t crc : made->missing)
        std::cerr << "[mods] warning: " << name_of(crc)
                  << ": the game's package has no such entry (a new file, which this build cannot add yet, or a "
                     "misspelt path); it is left out\n";
    for (const std::uint32_t crc : made->too_big)
        std::cerr << "[mods] error: " << name_of(crc) << " is 256 MiB or more, too big for a package entry; it is left out\n";
    std::cout << "[mods] package: " << made->in_place + made->relocated << " entr"
              << (made->in_place + made->relocated == 1u ? "y" : "ies") << " replaced, " << made->in_place
              << " in place and " << made->relocated << " moved past the end (" << made->relocated_bytes
              << " bytes); PACKAGE.BIN is " << made->package_size << " bytes as the game sees it" << std::endl;
    if (!made->regions.empty()) s.plan = std::move(made);
}

// The raw bytes of the open file `fd`.
std::vector<std::uint8_t> read_all(std::uint32_t fd) {
    std::vector<std::uint8_t> out;
    std::uint8_t chunk[65536];
    for (std::uint64_t at = 0u;;) {
        const std::size_t got = portablekit::read_open_file(fd, at, chunk, sizeof chunk);
        out.insert(out.end(), chunk, chunk + got);
        at += got;
        if (got < sizeof chunk) return out;
    }
}

// How long a file is, `has` telling whether it has a byte at an offset:
// `hint` when the file ends there (the end of the index's last entry), else
// found by probing.
std::uint64_t size_of(const std::function<bool(std::uint64_t)> &has, std::uint64_t hint) {
    if (hint != 0u && has(hint - 1u) && !has(hint)) return hint;
    std::uint64_t low = 0u, high = 1u;  // the size is in [low, high]
    while (has(high)) {
        low = high + 1u;
        high *= 2u;
    }
    while (low < high) {
        const std::uint64_t mid = low + (high - low) / 2u;
        if (has(mid)) low = mid + 1u;
        else high = mid;
    }
    return low;
}

class BytesFilter final : public Filter {
public:
    explicit BytesFilter(std::vector<std::uint8_t> bytes) : bytes_(std::move(bytes)) {}
    [[nodiscard]] std::uint64_t size() const override { return bytes_.size(); }
    std::size_t read(std::uint64_t offset, std::uint8_t *destination, std::size_t length) override {
        if (offset >= bytes_.size()) return 0u;
        const std::size_t count = static_cast<std::size_t>(std::min<std::uint64_t>(length, bytes_.size() - offset));
        std::memcpy(destination, bytes_.data() + offset, count);
        return count;
    }

private:
    std::vector<std::uint8_t> bytes_;
};

class PackageFilter final : public Filter {
public:
    // The package's own bytes come from `fd`, or from `base` when another
    // part of the port serves the package (index_over / package_over).
    PackageFilter(std::uint32_t fd, std::shared_ptr<const pix::Plan> plan, std::shared_ptr<Filter> base = nullptr)
        : fd_(fd), plan_(std::move(plan)), base_(std::move(base)) {}
    [[nodiscard]] std::uint64_t size() const override { return plan_->package_size; }
    std::size_t read(std::uint64_t offset, std::uint8_t *destination, std::size_t length) override {
        trace(offset, length);
        return pix::read(*plan_, offset, destination, length, [this](std::uint64_t at, std::uint8_t *out, std::size_t n) {
            if (base_) return base_->read(at, out, n);
            return portablekit::read_open_file(fd_, at, out, n);
        });
    }

private:
    // D012_TRACE_PACKAGE_MODS=<n>: the first n reads that touch a mod's file.
    void trace(std::uint64_t offset, std::size_t length) {
        long &left = state().trace_left;
        if (left <= 0) return;
        for (const pix::Region &region : plan_->regions) {
            if (region.offset >= offset + length || region.offset + region.length <= offset) continue;
            --left;
            std::cout << "[mods] package read at " << offset << ", " << length << " bytes: " << region.name
                      << " from byte " << (offset > region.offset ? offset - region.offset : 0u) << std::endl;
            return;
        }
    }

    std::uint32_t fd_;
    std::shared_ptr<const pix::Plan> plan_;
    std::shared_ptr<Filter> base_;
};

// "disc0:/PSP_GAME/USRDIR/DATA/<name>", in any case.
bool is_data_file(std::string_view path, std::string_view name) {
    const std::string text = lower(path);
    return text == "disc0:/psp_game/usrdir/data/" + std::string(name);
}

std::shared_ptr<Filter> serve(std::string_view path, std::uint32_t fd) {
    State &s = state();
    if (fd == 0u) return nullptr;
    const bool index = is_data_file(path, "package_info.bin");
    if (!index && !is_data_file(path, "package.bin")) return nullptr;
    if (!s.tried) {
        // The plan needs the index and the package's real size; whichever
        // the game opens first, the other is read beside it.
        const std::string folder(path.substr(0u, path.rfind('/') + 1u));
        std::vector<std::uint8_t> index_bytes;
        std::function<bool(std::uint64_t)> has;
        std::uint8_t byte = 0u;
        if (index) {
            index_bytes = read_all(fd);
            has = [&](std::uint64_t at) { return portablekit::read_guest_file(folder + "PACKAGE.BIN", at, &byte, 1u) == 1; };
        } else {
            index_bytes.resize(1u << 20u);
            const std::int64_t got =
                portablekit::read_guest_file(folder + "PACKAGE_INFO.BIN", 0u, index_bytes.data(), index_bytes.size());
            index_bytes.resize(got > 0 ? static_cast<std::size_t>(got) : 0u);
            has = [&](std::uint64_t at) { return portablekit::read_open_file(fd, at, &byte, 1u) == 1u; };
        }
        std::string error;
        const auto parsed = pix::parse(index_bytes, error);
        make_plan(index_bytes, size_of(has, parsed ? pix::end_sector(*parsed) * pix::kSector : 0u));
    }
    if (!s.plan) return nullptr;
    if (index) return std::make_shared<BytesFilter>(s.plan->index);
    return std::make_shared<PackageFilter>(fd, s.plan);
}

} // namespace

void add_late_source(std::string name, LateSource source) { state().late.emplace_back(std::move(name), std::move(source)); }

void install(const std::vector<Source> &sources) {
    static bool installed = false;
    if (installed || (sources.empty() && state().late.empty())) return;
    installed = true;
    if (portablekit::env("NO_PACKAGE_MODS") != nullptr) {
        std::cout << "[mods] package mods off (" << portablekit::env_name("NO_PACKAGE_MODS") << ")\n";
        state().late.clear();
        return;
    }
    if (const char *trace = portablekit::env("TRACE_PACKAGE_MODS")) state().trace_left = std::atol(trace);
    if (!sources.empty()) collect(sources);
    if (state().files.empty() && state().late.empty()) return;
    // The game loads each model file through a block of its own size; a
    // bigger one needs that block, and the memory around it, made bigger.
    if (const File *model = state().largest_model) {
        model_memory::fit(model->bytes->size());
        if (model->bytes->size() > model_memory::block_size())
            std::cerr << "[mods] warning: " << model->name << " is " << model->bytes->size()
                      << " bytes, more than the model block (" << model_memory::block_size()
                      << " bytes); it will not load\n";
    }
    portablekit::add_file_provider(&serve);
}

std::vector<std::uint8_t> index_over(const std::vector<std::uint8_t> &base, std::uint64_t base_size) {
    State &s = state();
    if (s.files.empty() && s.late.empty()) return base;
    if (!s.tried) make_plan(base, base_size);
    return s.plan ? s.plan->index : base;
}

std::shared_ptr<Filter> package_over(std::shared_ptr<Filter> base) {
    State &s = state();
    if (!s.plan || !base) return base;
    return std::make_shared<PackageFilter>(0u, s.plan, std::move(base));
}

} // namespace d012::package_mods
