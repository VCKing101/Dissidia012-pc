cmake_minimum_required(VERSION 3.20)
# The native PC port of Dissidia 012 Duodecim Final Fantasy (ULUS10566), on
# PortableKit. See README.md for the build and run steps.
project(D012Port VERSION 0.1.0 LANGUAGES CXX)

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
# framework does not name; the profile keeps its copy in port/config.
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config/nids.csv" "${D012_WORK_DIR}/config/nids.csv" COPYONLY)

portablekit_add_game(D012Native
    PROFILE_DIR "${D012_WORK_DIR}"
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
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_loading_layout.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_crash_report.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_test_switches.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_portable.cpp")
target_include_directories(D012Native PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/host")
# A release build (tools/local/port/build-d012-release, for the packaged
# folder of tools/port/package_deck.py) runs its recompiled code only, with no
# interpreter fallback unless D012_INTERPRETER is set (host/d012_portable.cpp).
if(PORTABLEKIT_RELEASE)
    set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_portable.cpp" APPEND PROPERTY
        COMPILE_DEFINITIONS D012_RELEASE_BUILD=1)
endif()

# Decompiled game functions (C), run as native overrides (port/decomp/).
enable_language(C)
add_library(d012_decomp STATIC "${CMAKE_CURRENT_SOURCE_DIR}/decomp/bgm_table.c"
                               "${CMAKE_CURRENT_SOURCE_DIR}/decomp/bgm_selector.c"
                               "${CMAKE_CURRENT_SOURCE_DIR}/decomp/battle_camera.c"
                               "${CMAKE_CURRENT_SOURCE_DIR}/decomp/pad.c"
                               "${CMAKE_CURRENT_SOURCE_DIR}/decomp/pad_walk.c")
target_link_libraries(D012Native PRIVATE d012_decomp)
