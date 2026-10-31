// Downloadable content: what the game will find, said once at start.
//
// The game reads DLC from ms0:/PSP/GAME/ULUS10566/<name hash>.edat (main
// 0x088626AC; docs/research/character-assets.md section 7) and nothing else:
// music packs, costumes and the Aerith key, each file looked up by the hash of
// its name when the game wants it. The port only reports here, so that a
// player who added DLC can see in the log whether it is where the game looks:
//
//   [dlc] 222 files in <ms0>/PSP/GAME/ULUS10566
//   [dlc] none in <ms0>/PSP/GAME/ULUS10566
//
// and, for .edat files in another folder of PSP/GAME (a copy named after the
// European release, ULES01505, is the usual one), that the game never reads
// them there. Files still in PSP's EDATA form (they start "\0PSPEDAT") are
// counted apart: PortableKit's sceNpDrmEdataSetupKey does not decrypt, and
// the port has been tried with decrypted DLC only (31 October 2026: the USA
// DLC's 13 music packs and 43 costumes, docs/port/bgm-playlist-design.md).
//
// The memory stick is <prefix>_MEMORY_STICK_DIR when set (run_d012.py,
// play_d012.ps1 and the packaged and installer builds set it,
// d012_portable.cpp), else <data dir>/ms0 as PortableKit's main uses.
//
// MIT licence; see port/README.md.

#include "d012_dlc.hpp"

#include "install/user_data.hpp"
#include "profile.hpp"

#include <array>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace d012::dlc {
namespace {

constexpr const char *kDiscId = "ULUS10566";

std::string utf8(const std::filesystem::path &path) { return portablekit::install::path_to_utf8(path); }

std::string lower(std::string text) {
    for (char &c : text) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return text;
}

std::filesystem::path memory_stick() {
    if (const char *dir = portablekit::env("MEMORY_STICK_DIR"); dir != nullptr && *dir != '\0')
        return portablekit::install::path_from_utf8(dir);
    try {
        return portablekit::install::user_data_directory() / "ms0";
    } catch (const std::exception &) {
        return {};
    }
}

struct Count {
    int files = 0;
    int encrypted = 0;  // still PSP EDATA ("\0PSPEDAT")
};

Count count_edat(const std::filesystem::path &folder) {
    Count count;
    std::error_code ec;
    for (const auto &entry : std::filesystem::directory_iterator(folder, ec)) {
        if (!entry.is_regular_file(ec) || lower(utf8(entry.path().extension())) != ".edat") continue;
        ++count.files;
        std::array<char, 8> head{};
        std::ifstream in(entry.path(), std::ios::binary);
        if (in.read(head.data(), head.size()) && std::string(head.data(), head.size()) == std::string("\0PSPEDAT", 8))
            ++count.encrypted;
    }
    return count;
}

void report() {
    const std::filesystem::path ms0 = memory_stick();
    if (ms0.empty()) return;
    const std::filesystem::path games = ms0 / "PSP" / "GAME";
    const std::filesystem::path ours = games / kDiscId;
    std::error_code ec;
    const Count found = std::filesystem::is_directory(ours, ec) ? count_edat(ours) : Count{};
    if (found.files == 0) std::cout << "[dlc] none in " << utf8(ours) << "\n";
    else std::cout << "[dlc] " << found.files << " files in " << utf8(ours) << "\n";
    if (found.encrypted != 0)
        std::cout << "[dlc] " << found.encrypted << (found.encrypted == 1 ? " of them is" : " of them are")
                  << " still encrypted, as a PSP keeps them; the port has "
                  << "been tried with decrypted DLC only and may not read these\n";
    for (const auto &entry : std::filesystem::directory_iterator(games, ec)) {
        if (!entry.is_directory(ec) || entry.path().filename() == kDiscId) continue;
        const Count other = count_edat(entry.path());
        if (other.files == 0) continue;
        std::cout << "[dlc] warning: " << other.files << " DLC files in " << utf8(entry.path())
                  << " are never read: the game reads DLC only from a folder named " << kDiscId
                  << " (rename the folder to use them)\n";
    }
}

} // namespace

void install(portablekit::HleRegistrar &) {
    static bool installed = false;
    if (installed) return;
    installed = true;
    report();
}

} // namespace d012::dlc
