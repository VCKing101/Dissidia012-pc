// The Android app's own compile of the game's code (the player's app,
// D012_INSTALLER_BUILD): the APK carries no game code, so the phone makes it
// from the player's disc, as the PC setups do (host/d012_compile_game.cpp),
// with a compiler the APK carries. It runs as the profile's prepare_game
// hook: after the disc is set up, before the game's code is loaded.
//
// What the APK carries for it:
//   lib/arm64-v8a/libclang.so, liblld.so, libpsp_recomp.so
//       clang 23 and lld (tools/port/android_compiler.sh) and the recompiler,
//       as "libraries": Android runs no program an app wrote into its storage,
//       only those in its native library folder.
//   assets/toolchain.tar, assets/toolchain.sha256
//       clang's own headers and the Android parts it compiles with
//       (tools/port/android_runtime.sh), and sdk/: unpacked once into
//       toolchain/ in the app's storage, and again when the APK carries
//       another (toolchain/.stamp holds the SHA-256 it came from).
//
// toolchain/bin/clang++ and ld.lld are links to the two programs, made again
// at every start: the native library folder moves with every update.
//
// Every start asks --compile-game's keys first (a second or two). Only when
// they say the code must be made does the setup screen show it, and the
// object cache (game-code-cache/) means an update compiles only the parts
// whose C++ changed.
//
// MIT licence; see port/README.md.

#if defined(PORTABLEKIT_ANDROID_APP) && defined(D012_INSTALLER_BUILD)

#include "d012_compile_screens.hpp"

#include "app_paths.hpp"
#include "install/installer.hpp"
#include "install/user_data.hpp"

#include <SDL3/SDL.h>

#include <unistd.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace d012::compile_game {
int run_in_app(bool check_only, std::function<void(const std::string &, unsigned)> listener,
               std::function<bool()> stop);
int needs_compile_code();
}

namespace d012::device_compile {
namespace {

namespace fs = std::filesystem;

constexpr const char *kToolchainAsset = "toolchain.tar";
constexpr const char *kStampAsset = "toolchain.sha256";

fs::path toolchain_dir() { return portablekit::install::user_data_directory() / "toolchain"; }

std::string read_asset_text(const char *name) {
    std::size_t size = 0;
    void *data = SDL_LoadFile(name, &size);
    if (data == nullptr) return {};
    std::string text(static_cast<const char *>(data), size);
    SDL_free(data);
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r' || text.back() == ' ')) text.pop_back();
    return text;
}

std::string read_text(const fs::path &file) {
    std::ifstream in(file);
    std::string text;
    std::getline(in, text);
    return text;
}

std::uint64_t octal(const char *field, std::size_t size) {
    std::uint64_t value = 0;
    for (std::size_t i = 0; i < size && field[i] != '\0' && field[i] != ' '; ++i)
        if (field[i] >= '0' && field[i] <= '7') value = value * 8u + static_cast<std::uint64_t>(field[i] - '0');
    return value;
}

// assets/toolchain.tar (ustar: files and folders only) into toolchain/, by
// way of toolchain.new/, so a half-unpacked one is never used.
void unpack(const std::string &stamp, const std::function<void(std::uint64_t, std::uint64_t)> &progress) {
    SDL_IOStream *in = SDL_IOFromFile(kToolchainAsset, "rb");
    if (in == nullptr) throw std::runtime_error("the app's compiler is missing from it (" + std::string(SDL_GetError()) + ")");
    const Sint64 total = SDL_GetIOSize(in);
    const fs::path target = toolchain_dir();
    const fs::path staged = fs::path(target).concat(".new");
    std::error_code ec;
    fs::remove_all(staged, ec);
    fs::create_directories(staged, ec);
    std::array<char, 512> header{};
    std::vector<char> buffer(1u << 20);
    std::uint64_t done = 0;
    for (;;) {
        if (SDL_ReadIO(in, header.data(), header.size()) != header.size()) break;
        done += header.size();
        if (header[0] == '\0') break;  // the end: two empty blocks
        std::string name(header.data(), strnlen(header.data(), 100));
        const std::string prefix(header.data() + 345, strnlen(header.data() + 345, 155));
        if (!prefix.empty()) name = prefix + "/" + name;
        const std::uint64_t size = octal(header.data() + 124, 12);
        const char type = header[156];
        const fs::path relative = fs::path(name).lexically_normal();
        if (relative.empty() || relative.is_absolute() || *relative.begin() == "..") {
            SDL_CloseIO(in);
            throw std::runtime_error("the app's compiler is damaged (" + name + ")");
        }
        if (type == '5') {
            fs::create_directories(staged / relative, ec);
        } else if (type == '0' || type == '\0') {
            fs::create_directories((staged / relative).parent_path(), ec);
            std::ofstream out(staged / relative, std::ios::binary | std::ios::trunc);
            for (std::uint64_t left = size; left > 0;) {
                const std::size_t chunk = static_cast<std::size_t>(std::min<std::uint64_t>(left, buffer.size()));
                if (SDL_ReadIO(in, buffer.data(), chunk) != chunk) {
                    SDL_CloseIO(in);
                    throw std::runtime_error("the app's compiler could not be read");
                }
                out.write(buffer.data(), static_cast<std::streamsize>(chunk));
                left -= chunk;
            }
            if (!out) {
                SDL_CloseIO(in);
                throw std::runtime_error("unpacking the app's compiler failed: check that the phone has free space");
            }
        }
        // Data is padded to whole blocks.
        const std::uint64_t padded = (size + 511u) / 512u * 512u;
        if (type == '0' || type == '\0') {
            if (padded > size) SDL_SeekIO(in, static_cast<Sint64>(padded - size), SDL_IO_SEEK_CUR);
        } else if (padded > 0) {
            SDL_SeekIO(in, static_cast<Sint64>(padded), SDL_IO_SEEK_CUR);
        }
        done += padded;
        progress(done, total > 0 ? static_cast<std::uint64_t>(total) : done);
    }
    SDL_CloseIO(in);
    { std::ofstream(staged / ".stamp") << stamp << "\n"; }
    fs::remove_all(target, ec);
    fs::rename(staged, target, ec);
    if (ec) throw std::runtime_error("could not put the app's compiler in place: " + ec.message());
}

// toolchain/bin/clang++ and ld.lld, links to the programs in the native
// library folder, which moves with every update of the app.
void link_programs() {
    const fs::path bin = toolchain_dir() / "bin";
    std::error_code ec;
    fs::create_directories(bin, ec);
    const fs::path libs = portablekit::executable_directory();
    for (const auto &[name, program] : {std::pair{"clang++", "libclang.so"}, std::pair{"ld.lld", "liblld.so"}}) {
        fs::remove(bin / name, ec);
        fs::create_symlink(libs / program, bin / name, ec);
        if (ec) std::cerr << "[device-compile] cannot link " << name << ": " << ec.message() << std::endl;
    }
}

} // namespace

bool prepare(portablekit::install::InstallerUi *given, const fs::path &, const fs::path &) {
    const std::string stamp = read_asset_text(kStampAsset);
    const bool fresh_toolchain = stamp.empty() || read_text(toolchain_dir() / ".stamp") != stamp;
    if (!fresh_toolchain) {
        link_programs();
        const int code = d012::compile_game::run_in_app(true, {}, {});
        if (code == 0) return true;
        std::cout << "[device-compile] the game's code must be made (" << code << ")" << std::endl;
    }

    // The installer's own screens when it asks (it is setting up), or ours.
    const compile_screens::Result result = compile_screens::run(
        given, "Making the game from your disc",
        [&](portablekit::install::InstallerUi *ui, const compile_screens::Hooks &hooks) {
            if (fresh_toolchain) {
                if (stamp.empty()) throw std::runtime_error("the app's compiler is missing from it");
                unpack(stamp, [&](std::uint64_t done, std::uint64_t total) {
                    if (ui) ui->progress("Unpacking the compiler", done, total);
                });
            }
            link_programs();
            return d012::compile_game::run_in_app(false, hooks.listener, hooks.stop);
        });
    if (result.outcome == compile_screens::Outcome::Done) return true;
    if (result.outcome == compile_screens::Outcome::Failed)
        portablekit::install::report_problem(
            "The game could not be made",
            "Making the game's code from your disc did not succeed:\n\n" + result.message +
                "\n\nStart the app again to try again. The log is game-code-build.log in the app's data.",
            false);
    return false;
}

} // namespace d012::device_compile

#endif
