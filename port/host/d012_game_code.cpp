// The installer build (D012_INSTALLER_BUILD): a program that carries none of
// the game's code. The installer recompiles it on the player's PC from their
// own disc and puts it beside the program:
//
//   <folder>/game-code/main.dll        the main executable's corpus, a corpus
//                                      library (PortableKit's
//                                      host/corpus_library.hpp)
//   <folder>/game-code/overlays/*.dll  the overlay libraries, found through
//                                      D012_OVERLAY_DIR (host/d012_portable.cpp)
//
// D012_GAME_CODE_DIR names another folder. PortableKit calls
// register_generated_functions() where a port's build would have linked the
// corpus (portablekit_add_game(... NO_CORPUS)).
//
// MIT licence; see port/README.md.

#include "app_paths.hpp"
#include "corpus_library.hpp"
#include "profile.hpp"

#include "psprecomp/corpus_abi.hpp"

#include <SDL3/SDL.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

namespace d012::game_code {

std::filesystem::path directory() {
    if (const char *dir = std::getenv("D012_GAME_CODE_DIR"); dir != nullptr && *dir != '\0')
        return std::filesystem::path(std::u8string(dir, dir + std::char_traits<char>::length(dir)));
    return portablekit::executable_directory() / "game-code";
}

namespace {

void say(const std::string &message) {
    std::cerr << "[game-code] " << message << "\n";
    // The program is started from a shortcut, with no console to read.
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Dissidia 012 (PC port)", message.c_str(), nullptr);
}

} // namespace
} // namespace d012::game_code

namespace psprecomp {

void register_generated_functions(CorpusRuntime &runtime) {
    const std::filesystem::path library = d012::game_code::directory() / "main.dll";
    std::error_code ec;
    if (!std::filesystem::is_regular_file(library, ec)) {
        d012::game_code::say("The game's code is not compiled yet (" + library.string() +
                             " is missing).\nRun the setup again to compile it from your disc.");
        return;
    }
    std::string error;
    const auto corpus =
        portablekit::open_corpus_library(library, portablekit::game().executable_sha256, error);
    if (!corpus) {
        d012::game_code::say("The compiled game code in " + library.string() + " cannot be used: " + error +
                             "\nRun the setup again to compile it from your disc.");
        return;
    }
    corpus->register_all(runtime);
    std::cout << "[game-code] " << library.string() << std::endl;
}

} // namespace psprecomp
