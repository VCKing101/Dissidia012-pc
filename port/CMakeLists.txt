cmake_minimum_required(VERSION 3.20)
# The native PC port of Dissidia 012 Duodecim Final Fantasy (ULUS10566), on
# PortableKit. See README.md for the build and run steps.
project(D012Port VERSION 0.1.0 LANGUAGES C CXX)

get_filename_component(D012_REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
# PortableKit is not a submodule here: it is a local clone (on its local
# "dissidia" branch), named by this cache variable.
set(D012_PORTABLEKIT_DIR "${D012_REPO_ROOT}/tools/local/port/PortableKit" CACHE PATH
    "PortableKit checkout to build against")
# Everything derived from the game stays out of port/: the AOT corpus
# (generated/), the overlay corpora (overlays/) and the game directory
# (game/) live here, under the repository's ignored build/.
set(D012_WORK_DIR "${D012_REPO_ROOT}/build/port" CACHE PATH
    "Directory for generated/, overlays/ and game/ (never committed)")

# The installer build: the program without the game's code, which the
# installer compiles on the player's PC from their disc into game-code/ beside
# the program (host/d012_game_code.cpp). It reads no generated/ or overlays/,
# so it builds without the game. Build it with llvm-mingw, the compiler the
# installer ships, so the program and the game code share one C++ runtime.
option(D012_INSTALLER_BUILD "Build the program without the game's code, for the installer" OFF)

set(PSPRECOMP_BUILD_TESTS OFF CACHE BOOL "Build reusable PSPRecomp tests")
# The sources are UTF-8 (PortableKit's menus and font samples have Japanese
# text). MSVC otherwise reads them in the PC's ANSI code page, and on a Windows
# set to Chinese, Japanese or Korean that stops the build (C2001). Set before
# PortableKit so its targets get it too.
if(MSVC)
    add_compile_options(/utf-8)
endif()
add_subdirectory("${D012_PORTABLEKIT_DIR}" portablekit)

# portablekit_add_game() reads <profile>/config/nids.csv for NIDs the
# framework does not name; the profile keeps its copy in port/config. The
# installer build's profile directory holds only that file, so a developer's
# corpus and overlays in D012_WORK_DIR are never built into it.
if(D012_INSTALLER_BUILD)
    set(d012_profile_dir "${CMAKE_BINARY_DIR}/installer-profile")
    set(d012_corpus NO_CORPUS)
    set(d012_game_code "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_game_code.cpp")
else()
    set(d012_profile_dir "${D012_WORK_DIR}")
    set(d012_corpus)
    set(d012_game_code)
endif()
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config/nids.csv" "${d012_profile_dir}/config/nids.csv" COPYONLY)

portablekit_add_game(D012Native ${d012_corpus}
    PROFILE_DIR "${d012_profile_dir}"
    SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_profile.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_hle.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_prx.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_json.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_mods.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_nintendo_stream.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_decomp_harness.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_decomp_bgm.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_decomp_pad.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_bgm_playlist.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_camera.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_split_view.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_versus.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_versus_menu.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_loading_layout.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_crash_report.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_test_switches.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_portable.cpp"
            ${d012_game_code})
target_include_directories(D012Native PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/host")
# A release build (tools/local/port/build-d012-release, for the packaged
# folder of tools/port/package_deck.py) runs its recompiled code only, with no
# interpreter fallback unless D012_INTERPRETER is set (host/d012_portable.cpp).
if(PORTABLEKIT_RELEASE)
    set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_portable.cpp" APPEND PROPERTY
        COMPILE_DEFINITIONS D012_RELEASE_BUILD=1)
endif()
if(D012_INSTALLER_BUILD)
    set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_portable.cpp" APPEND PROPERTY
        COMPILE_DEFINITIONS D012_INSTALLER_BUILD=1)
endif()

# Decompiled game functions (C), run as native overrides (port/decomp/). C is
# enabled in project() above: SDL3's CMake package compiles a C test program
# when PortableKit looks for it, under MinGW.
add_library(d012_decomp STATIC "${CMAKE_CURRENT_SOURCE_DIR}/decomp/bgm_table.c"
                               "${CMAKE_CURRENT_SOURCE_DIR}/decomp/bgm_selector.c"
                               "${CMAKE_CURRENT_SOURCE_DIR}/decomp/battle_camera.c"
                               "${CMAKE_CURRENT_SOURCE_DIR}/decomp/pad.c"
                               "${CMAKE_CURRENT_SOURCE_DIR}/decomp/pad_walk.c")
target_link_libraries(D012Native PRIVATE d012_decomp)
