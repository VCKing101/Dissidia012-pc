// Bartz's moves from the first Dissidia in the game; see
// d012_original_moves_feature.hpp. MIT licence; see port/README.md.

#include "d012_original_moves_feature.hpp"

#include "d012_decomp_harness.hpp"
#include "d012_dlc.hpp"
#include "d012_dlc_packs.hpp"
#include "d012_dlc_packs_plan.hpp"
#include "d012_json.hpp"
#include "d012_model_memory.hpp"
#include "d012_net.hpp"
#include "d012_original_moves.hpp"
#include "d012_package_index.hpp"
#include "d012_package_mods.hpp"

#include "hle/hle_common.hpp"
#include "install/user_data.hpp"
#include "patched_edition.hpp"
#include "portablekit/hle_extension.hpp"
#include "profile.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace d012 {
// The recipe (port/features/bartz-original-moves.json), compiled into the
// program by port/CMakeLists.txt so every copy has the same one.
extern const unsigned char kOriginalMovesRecipe[];
extern const std::size_t kOriginalMovesRecipeSize;
} // namespace d012

namespace d012::original_moves {
namespace {

namespace pix = package_index;

constexpr const char *kPackage = "disc0:/PSP_GAME/USRDIR/DATA/PACKAGE.BIN";
constexpr const char *kPackageInfo = "disc0:/PSP_GAME/USRDIR/DATA/PACKAGE_INFO.BIN";
// How the online check names this: both players have the moves or neither.
constexpr const char *kMatchItem = "Bartz's original moves";
// How much bigger the battle's sound and effect heaps are made: with the
// moves, Bartz's sound bank is 178 KB bigger and his effect package 384 KB, and
// a battle may load them for both fighters and for two assists.
constexpr std::uint32_t kSoundRoom = 0x100000u;
constexpr std::uint32_t kEffectRoom = 0x200000u;
// Changes with the builder: a cache made by an older one is not used.
constexpr unsigned kBuilderVersion = 1u;
// His first look, which the build gives the moves like the others.
constexpr const char *kFirstLook = "obj/P_FIV100_1P.gmo";

struct State {
    std::optional<json::Value> recipe;
    std::uint32_t recipe_crc{};
    bool active{};
    // His first look as built, which his DLC looks are fitted to, and how many
    // animations the build adds.
    std::shared_ptr<const Bytes> first_look;
    std::size_t added{};
};
State &state() {
    static State s;
    return s;
}

// An entry's CRC is of its path in lower case.
std::uint32_t entry_crc(std::string_view path) {
    std::string text(path);
    for (char &c : text)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return pix::path_crc(text);
}

std::string hex8(std::uint32_t value) {
    char text[9];
    std::snprintf(text, sizeof text, "%08X", static_cast<unsigned>(value));
    return text;
}

void left_out(const std::string &why) { std::cout << "[original-moves] left out: " << why << std::endl; }

std::optional<Bytes> read_host_file(const std::filesystem::path &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::nullopt;
    Bytes bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    if (in.bad()) return std::nullopt;
    return bytes;
}

// The disc's own PACKAGE_INFO.BIN, whatever another part of the port serves
// the game in its place.
Bytes read_index() {
    Bytes bytes(4u << 20u);
    const std::int64_t got = portablekit::read_guest_file(kPackageInfo, 0u, bytes.data(), bytes.size());
    bytes.resize(got > 0 ? static_cast<std::size_t>(got) : 0u);
    return bytes;
}

// Where a build for this recipe and this disc is kept. The disc is told by
// its index: the same index is the same package layout.
std::filesystem::path cache_folder(std::uint32_t index_crc) {
    return portablekit::install::user_data_directory() / "cache" / "original-moves" /
           (hex8(state().recipe_crc) + "-" + hex8(index_crc) + "-" + std::to_string(kBuilderVersion));
}

// The files of an earlier build: every file the recipe says a build comes to,
// with its size and CRC. Nothing when any is missing or differs.
std::optional<std::vector<File>> load_cache(const std::filesystem::path &folder, const std::vector<Mark> &wanted) {
    std::error_code ec;
    if (!std::filesystem::exists(folder / "done", ec)) return std::nullopt;
    std::vector<File> files;
    for (std::size_t i = 0u; i < wanted.size(); ++i) {
        auto bytes = read_host_file(folder / (std::to_string(i) + ".bin"));
        if (!bytes || bytes->size() != wanted[i].size || crc32(*bytes) != wanted[i].crc) return std::nullopt;
        files.push_back({wanted[i].path, std::move(*bytes)});
    }
    return files;
}

// Keeps a build for the next start. A cache that cannot be written only means
// the next start builds again.
void save_cache(const std::filesystem::path &folder, const std::vector<Mark> &wanted, const std::vector<File> &files) {
    std::error_code ec;
    std::filesystem::remove_all(folder.parent_path(), ec);  // builds for other recipes or discs
    std::filesystem::create_directories(folder, ec);
    if (ec) return;
    for (std::size_t i = 0u; i < wanted.size(); ++i) {
        const auto file = std::find_if(files.begin(), files.end(), [&](const File &f) { return f.path == wanted[i].path; });
        if (file == files.end()) return;
        std::ofstream out(folder / (std::to_string(i) + ".bin"), std::ios::binary | std::ios::trunc);
        out.write(reinterpret_cast<const char *>(file->bytes.data()), static_cast<std::streamsize>(file->bytes.size()));
        if (!out) return;
    }
    std::ofstream(folder / "done") << "built by the port from the game's disc; safe to delete\n";
}

// Builds the files from the disc. Nothing, with the reason logged, when the
// disc is not the recipe's or the build is not what the recipe says.
std::optional<std::vector<File>> build_from_disc(const pix::Index &index) {
    const json::Value &recipe = *state().recipe;
    std::map<std::string, Bytes, std::less<>> disc;
    const auto read = [&](std::string_view path) -> std::optional<Bytes> {
        if (const auto found = disc.find(path); found != disc.end()) return found->second;
        const std::uint32_t crc = entry_crc(path);
        const auto entry = std::lower_bound(index.entries.begin(), index.entries.end(), crc,
                                            [](const pix::Entry &e, std::uint32_t wanted) { return e.crc < wanted; });
        if (entry == index.entries.end() || entry->crc != crc) return std::nullopt;
        Bytes bytes(entry->size);
        const std::int64_t got =
            portablekit::read_guest_file(kPackage, std::uint64_t(entry->sector) * pix::kSector, bytes.data(), bytes.size());
        if (got != static_cast<std::int64_t>(bytes.size())) return std::nullopt;
        return disc.emplace(std::string(path), std::move(bytes)).first->second;
    };
    for (const Mark &mark : inputs(recipe)) {
        const auto bytes = read(mark.path);
        if (!bytes || bytes->size() != mark.size || crc32(*bytes) != mark.crc) {
            left_out("the disc's " + mark.path + " is not the file the moves are built from (a disc mod changes it, or this "
                     "is another release of the game)");
            return std::nullopt;
        }
    }
    Built built = build(recipe, read);
    for (const std::string &note : built.notes) std::cout << "[original-moves] " << note << "\n";
    std::string problem;
    if (!check(recipe, built, problem)) {
        left_out("the build is not as it must be (" + problem + "); please report this");
        return std::nullopt;
    }
    return std::move(built.files);
}

// Who the abilities are given by (mod_abilities::Grant::mod_id).
constexpr const char *kGrantOwner = "the port";

// The files for the package, asked for when the game first opens it.
std::vector<package_mods::LateFile> build_for_package(const std::function<bool(std::uint32_t)> &taken) {
    State &s = state();
    std::vector<package_mods::LateFile> out;
    try {
        const json::Value &recipe = *s.recipe;
        const std::vector<Mark> wanted = outputs(recipe);
        for (const Mark &mark : wanted) {
            if (taken(entry_crc(mark.path))) {
                left_out("a mod replaces " + mark.path + ", which the moves change too");
                return out;
            }
            if (mark.size > model_memory::block_size() && mark.path.starts_with("obj/")) {
                left_out(mark.path + " would be bigger than the game's model block");
                return out;
            }
        }
        const Bytes index_bytes = read_index();
        std::string error;
        const auto index = pix::parse(index_bytes, error);
        if (!index) {
            left_out("the disc's PACKAGE_INFO.BIN cannot be read (" + error + ")");
            return out;
        }
        const auto started = std::chrono::steady_clock::now();
        const std::filesystem::path folder = cache_folder(crc32(index_bytes));
        std::optional<std::vector<File>> files = load_cache(folder, wanted);
        const bool cached = files.has_value();
        if (!files) {
            files = build_from_disc(*index);
            if (!files) return out;
            save_cache(folder, wanted, *files);
        }
        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count();
        for (File &file : *files) {
            auto bytes = std::make_shared<const std::vector<std::uint8_t>>(std::move(file.bytes));
            if (file.path == kFirstLook) s.first_look = bytes;
            out.push_back({std::move(file.path), std::move(bytes)});
        }
        s.added = added_animations(recipe);
        s.active = true;
        net::battle::must_match(kMatchItem, "on, recipe " + hex8(s.recipe_crc));
        char took[32];
        std::snprintf(took, sizeof took, "%.2f", seconds);
        std::cout << "[original-moves] Bartz's original moves: " << out.size() << " files "
                  << (cached ? "from the cache" : "built from the disc") << " in " << took << " s" << std::endl;
    } catch (const std::exception &failure) {
        left_out(failure.what());
        out.clear();
    }
    return out;
}

// The same, and with the moves left out, their abilities are not given: the
// game's ability table is the disc's, and a side file from a start that had
// them must not put them back into a save.
std::vector<package_mods::LateFile> files_for_package(const std::function<bool(std::uint32_t)> &taken) {
    std::vector<package_mods::LateFile> out = build_for_package(taken);
    if (!state().active) mod_abilities::withdraw(kGrantOwner);
    return out;
}

// --- His looks from DLC ------------------------------------------------------
// A DLC look is a model file of its own on the memory stick (or in a DLC
// pack), read by the hash of its name. As the game opens one of his as a
// fighter's, it is served with the moves' animations added (fit_look); one
// that cannot take them is served, in a battle, as his first look, so that he
// never runs a move he has no animation for, and both copies of an online
// battle run it alike whatever look each draws. Outside a battle (the menus'
// previews) it stays as it is.

struct Look {
    std::string model;                        // "P_FIV100_4P"
    std::optional<LookFit> fit;               // once the file has been seen
    std::shared_ptr<const Bytes> with_moves;  // with moves_added
    bool told_in_battle{};
};
struct Looks {
    bool scanned{};
    std::map<std::uint32_t, Look> by_name;  // by the hash of obj/<model>.gmo
};
Looks &looks() {
    static Looks l;
    return l;
}

// The battle overlay is in the overlay window (battle 0x08B2F2E4's first two
// instructions, as d012_versus.cpp tells a battle).
bool battle_on() {
    constexpr std::uint32_t kSetAiMode = 0x08B2F2E4u;
    return decomp::live_load32(kSetAiMode) == 0x27BDFFF0u && decomp::live_load32(kSetAiMode + 4u) == 0x2CA60006u;
}

std::string lower(std::string_view text) {
    std::string out(text);
    for (char &c : out) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

// A DLC file as the game finds it: the memory stick's own, else a pack's.
std::optional<Bytes> dlc_file(std::uint32_t crc) {
    char name[16];
    std::snprintf(name, sizeof name, "%08x.edat", static_cast<unsigned>(crc));
    if (auto bytes = read_host_file(dlc::folder() / name)) return bytes;
    return dlc_packs::served_file(crc);
}

// His fighter looks among the DLC's costume rows.
void scan(Looks &l) {
    for (int slot = 0; slot < 256; ++slot) {
        char row_path[32];
        std::snprintf(row_path, sizeof row_path, "dlc/obj/dlc_%03doe.bin", slot);
        const auto row = dlc_file(dlc_packs::name_crc(row_path));
        if (!row) continue;
        if (const auto model = fighter_look_model(*row))
            l.by_name[dlc_packs::name_crc("obj/" + *model + ".gmo")].model = *model;
    }
}

Bytes read_open(std::uint32_t fd) {
    Bytes bytes;
    std::uint8_t chunk[64 * 1024];
    for (std::uint64_t at = 0u;;) {
        const std::size_t got = portablekit::hle_extension::read_open_file(fd, at, chunk);
        if (got == 0u) break;
        bytes.insert(bytes.end(), chunk, chunk + got);
        at += got;
    }
    return bytes;
}

class ServedLook final : public portablekit::hle_extension::FileFilter {
public:
    explicit ServedLook(std::shared_ptr<const Bytes> bytes) : bytes_(std::move(bytes)) {}
    [[nodiscard]] std::uint64_t size() const override { return bytes_->size(); }
    std::size_t read(std::uint64_t offset, std::uint8_t *destination, std::size_t length) override {
        if (offset >= bytes_->size()) return 0u;
        const std::size_t count = static_cast<std::size_t>(std::min<std::uint64_t>(length, bytes_->size() - offset));
        std::copy_n(bytes_->begin() + static_cast<std::ptrdiff_t>(offset), count, destination);
        return count;
    }

private:
    std::shared_ptr<const Bytes> bytes_;
};

std::shared_ptr<portablekit::hle_extension::FileFilter> serve_look(std::string_view path, std::uint32_t fd) {
    State &s = state();
    if (!s.active || s.first_look == nullptr) return nullptr;
    const std::size_t slash = path.rfind('/');
    if (slash == std::string_view::npos) return nullptr;
    const auto crc = dlc_packs::crc_of_file_name(path.substr(slash + 1u));
    if (!crc) return nullptr;
    static const std::string folder = lower("ms0:/PSP/GAME/" + std::string(portablekit::active_disc_id()) + "/");
    if (lower(path.substr(0u, slash + 1u)) != folder) return nullptr;
    Looks &l = looks();
    if (!l.scanned) {
        l.scanned = true;
        scan(l);
    }
    const auto found = l.by_name.find(*crc);
    if (found == l.by_name.end()) return nullptr;
    Look &look = found->second;
    if (!look.fit) {
        std::optional<Bytes> bytes = fd != 0u ? std::optional<Bytes>(read_open(fd)) : dlc_packs::served_file(*crc);
        if (!bytes || bytes->empty()) return nullptr;  // no file: the game draws his first look itself
        FittedLook fitted;
        try {
            fitted = fit_look(*bytes, *s.first_look, s.added, model_memory::block_size());
        } catch (const std::exception &failure) {
            fitted.fit = LookFit::cannot;
            fitted.why = failure.what();
        }
        look.fit = fitted.fit;
        const std::string name = look.model + " (" + hex8(*crc) + ".edat)";
        if (fitted.fit == LookFit::moves_added) {
            look.with_moves = std::make_shared<const Bytes>(std::move(fitted.bytes));
            std::cout << "[original-moves] DLC look " << name << ": the moves' animations added (" << look.with_moves->size()
                      << " bytes)" << std::endl;
        } else if (fitted.fit == LookFit::has_moves) {
            std::cout << "[original-moves] DLC look " << name << ": has the moves' animations" << std::endl;
        } else {
            std::cout << "[original-moves] DLC look " << name << ": the moves cannot go in (" << fitted.why
                      << "); in a battle he wears his first look" << std::endl;
        }
    }
    switch (*look.fit) {
    case LookFit::has_moves: return nullptr;
    case LookFit::moves_added: return std::make_shared<ServedLook>(look.with_moves);
    case LookFit::cannot:
        if (!battle_on()) return nullptr;
        if (!look.told_in_battle) {
            look.told_in_battle = true;
            std::cout << "[original-moves] DLC look " << look.model << " drawn in this battle as his first look" << std::endl;
        }
        return std::make_shared<ServedLook>(s.first_look);
    }
    return nullptr;
}

} // namespace

void install(std::vector<mod_abilities::Grant> &grants) {
    static bool installed = false;
    if (installed) return;
    installed = true;
    State &s = state();
    net::battle::must_match(kMatchItem, "off");
    if (portablekit::env("NO_ORIGINAL_MOVES") != nullptr) {
        std::cout << "[original-moves] off (" << portablekit::env_name("NO_ORIGINAL_MOVES") << ")\n";
        return;
    }
    if (portablekit::env("NO_PACKAGE_MODS") != nullptr) {
        std::cout << "[original-moves] off (" << portablekit::env_name("NO_PACKAGE_MODS") << ")\n";
        return;
    }
    // A disc mod plays its own files (Emendatus changes Bartz's moveset), so
    // the moves would not match them; and the memory their heaps grow into is
    // the mod's (d012_hle.cpp). What plays, as PortableKit opened it: a mod
    // that cannot be opened leaves the game's own disc, and the moves, in.
    if (portablekit::patched::active()) {
        left_out("a disc mod is on");
        return;
    }
    const std::span<const std::uint8_t> bytes(kOriginalMovesRecipe, kOriginalMovesRecipeSize);
    std::string error;
    s.recipe = json::parse(std::string_view(reinterpret_cast<const char *>(bytes.data()), bytes.size()), error);
    if (!s.recipe) {
        left_out("the recipe cannot be read (" + error + ")");
        return;
    }
    // Line endings aside, as the port's source hash takes them: a checkout's
    // recipe has LF or CRLF by its settings, and both players must agree.
    Bytes text;
    for (const std::uint8_t byte : bytes)
        if (byte != 0x0Du) text.push_back(byte);  // CR
    s.recipe_crc = crc32(text);
    try {
        // D012_TEST_ORIGINAL_MOVES=1: all eight from level 1, to try them on any save.
        const bool all_now = portablekit::env("TEST_ORIGINAL_MOVES") != nullptr;
        for (const auto &[id, learn] : abilities(*s.recipe)) grants.push_back({id, all_now ? 1 : learn, kGrantOwner});
    } catch (const Error &failure) {
        left_out(failure.what());
        s.recipe.reset();
        return;
    }
    package_mods::add_late_source("the port's original moves", &files_for_package);
    // Room for his bigger sound bank and effect package, so that the battle's
    // own sounds and effects loaded after them still fit
    // (d012_model_memory.hpp). Made now, before the game runs, whether or not
    // the moves are used in the end.
    if (portablekit::env("NO_BATTLE_ROOM") == nullptr) model_memory::fit_battle_heaps(kSoundRoom, kEffectRoom);
}

bool active() { return state().active; }

void install_looks() {
    static bool installed = false;
    if (installed) return;
    installed = true;
    if (portablekit::env("NO_ORIGINAL_MOVES_LOOKS") != nullptr) {
        std::cout << "[original-moves] DLC looks left as they are (" << portablekit::env_name("NO_ORIGINAL_MOVES_LOOKS")
                  << ")\n";
        return;
    }
    portablekit::hle_extension::add_file_provider(serve_look);
}

} // namespace d012::original_moves
