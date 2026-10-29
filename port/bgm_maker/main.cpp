// The Battle BGM Maker: a window for making a battle music mod, above all its
// loop points (docs/port/mod-folder.md, section 8). Its own program beside
// D012Native; it never runs the game's code. MIT licence; see port/README.md.
//
//   BgmMaker [mods folder]
//
// Test switches, for the tool's own checks (screenshots of its window):
//   D012_BGM_MAKER_THEME=light|dark   instead of the system's
//   D012_BGM_MAKER_OPEN=<folder>      opens that mod at the start ("+new": a new mod)
//   D012_BGM_MAKER_TAB=1..4           starts on that step
//   D012_BGM_MAKER_CAPTURE=<file.png> saves the window as a PNG after a few frames, then quits
//   D012_BGM_MAKER_SCRIPT=<file>      scripted input and captures (input_script.hpp)

#include "app.hpp"
#include "audio_file.hpp"
#include "input_script.hpp"
#include "mods_folder.hpp"
#include "window.hpp"

#include "d012_mod_manifest.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <string>

namespace {

constexpr const char *kUsage =
    "BgmMaker [mods folder]\n\n"
    "Makes and changes battle music mods for the Dissidia 012 PC port. Without a folder it works on the game's own "
    "mods folder.";

void message(const char *title, const std::string &text, bool error) {
    std::cerr << text << "\n";
    SDL_ShowSimpleMessageBox(error ? SDL_MESSAGEBOX_ERROR : SDL_MESSAGEBOX_INFORMATION, title, text.c_str(), nullptr);
}

} // namespace

int main(int argc, char **argv) {
    using namespace d012::bgm_maker;
    // For the tool's own checks: the samples a loop point counts in, as raw
    // 16-bit little-endian, to compare with another decoder's.
    if (argc == 4 && std::strcmp(argv[1], "--decode") == 0) {
        const DecodedAudio audio = decode_audio(d012::mod_manifest::path_from_utf8(argv[2]));
        if (!audio.error.empty()) {
            std::cerr << audio.error << "\n";
            return 1;
        }
        std::FILE *out = std::fopen(argv[3], "wb");
        if (out == nullptr) return 1;
        std::fwrite(audio.samples.data(), 2u, audio.samples.size(), out);
        std::fclose(out);
        std::cout << audio.frames << " frames at " << audio.rate << " Hz, " << audio.channels << " channel(s)\n";
        return 0;
    }
    std::optional<std::filesystem::path> chosen;
    for (int i = 1; i < argc; ++i) {
        const std::string argument = argv[i];
        if (argument == "--help" || argument == "-h" || argument == "/?") {
            message("Battle BGM Maker", kUsage, false);
            return 0;
        }
        if (argument.starts_with("-") || chosen) {
            message("Battle BGM Maker", "Unknown argument \"" + argument + "\".\n\n" + kUsage, true);
            return 2;
        }
        chosen = d012::mod_manifest::path_from_utf8(argument);
    }

    Window window;
    std::string error;
    if (const char *theme = std::getenv("D012_BGM_MAKER_THEME")) window.force_theme(std::strcmp(theme, "dark") == 0);
    if (!window.open("Battle BGM Maker", error)) {
        message("Battle BGM Maker", "The Battle BGM Maker cannot start: " + error + ".", true);
        return 1;
    }
    App app(window, find_mods_folder(chosen));
    if (const char *open = std::getenv("D012_BGM_MAKER_OPEN")) app.test_open(open);
    if (const char *tab = std::getenv("D012_BGM_MAKER_TAB")) app.test_tab(std::atoi(tab));
    if (const char *capture = std::getenv("D012_BGM_MAKER_CAPTURE"))
        window.capture_after(10, d012::mod_manifest::path_from_utf8(capture));
    InputScript script;
    if (const char *file = std::getenv("D012_BGM_MAKER_SCRIPT")) {
        if (!script.load(d012::mod_manifest::path_from_utf8(file), error)) {
            std::cerr << "[bgm-maker] D012_BGM_MAKER_SCRIPT: " << error << "\n";
            return 2;
        }
        window.set_script(&script);
    }
    window.run([&app] { return app.frame(); });
    return 0;
}
