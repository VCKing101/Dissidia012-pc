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
    # PortableKit's main() becomes d012_portablekit_main, behind the port's
    # own, which adds --compile-game (host/d012_compile_game.cpp).
    set(d012_corpus NO_CORPUS HOST_MAIN_NAME d012_portablekit_main)
    set(d012_game_code "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_game_code.cpp"
                       "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_compile_game.cpp")
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
            "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_mod_manifest.cpp"
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
    # A windowed program: started from a shortcut, no console opens behind the
    # game. What it prints goes to data/logs/ then (host/d012_portable.cpp).
    set_target_properties(D012Native PROPERTIES WIN32_EXECUTABLE TRUE)
    # What --compile-game needs beside the program, so bin/ is laid out as an
    # installation (the toolchain aside: D012_TOOLCHAIN_DIR, or toolchain/):
    # the recompiler, and in sdk/ the three headers the game's code includes,
    # the two entry-point templates and the dispatch calls.
    # A target of its own, so a new psp_recomp is copied whether or not the
    # program relinks.
    set(d012_sdk "${CMAKE_BINARY_DIR}/bin/sdk")
    add_custom_target(D012Native_installer_files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:psp_recomp>" "${CMAKE_BINARY_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${d012_sdk}/include/psprecomp"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${D012_PORTABLEKIT_DIR}/include/psprecomp/corpus_abi.hpp"
                "${D012_PORTABLEKIT_DIR}/include/psprecomp/allegrex_context.hpp" "${d012_sdk}/include/psprecomp"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${D012_PORTABLEKIT_DIR}/host/overlay_module.hpp" "${d012_sdk}/include"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${D012_PORTABLEKIT_DIR}/host/corpus_module.cpp.in" "${D012_PORTABLEKIT_DIR}/host/overlay_module.cpp.in"
                "${CMAKE_CURRENT_SOURCE_DIR}/config/dispatch-calls.txt" "${d012_sdk}"
        VERBATIM)
    add_dependencies(D012Native_installer_files psp_recomp D012Native)
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

# The Battle BGM Maker (bgm_maker/): a program of its own beside D012Native,
# for making battle music mods. It checks a mod with the game's own manifest
# rules (host/d012_mod_manifest.cpp, which the game builds too) and draws with
# PortableKit's copy of Dear ImGui; it has none of the game's code, so the
# installer ships it as it is. SDL3 and Vulkan are the targets
# portablekit_add_game() found above.
add_library(d012_manifest STATIC "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_json.cpp"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_mod_manifest.cpp"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/host/d012_nintendo_stream.cpp"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/model.cpp"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/names.cpp"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/checks.cpp")
target_compile_features(d012_manifest PUBLIC cxx_std_20)
target_include_directories(d012_manifest PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/host" "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker")

set(d012_imgui "${D012_PORTABLEKIT_DIR}/third_party/imgui")
add_executable(BgmMaker WIN32
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/app.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/audio_file.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/decoder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/importer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/input_script.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/mods_folder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/player.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/suggest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/tab_loops.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/tab_music.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/theme.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/waveform.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/window.cpp"
    "${d012_imgui}/imgui.cpp"
    "${d012_imgui}/imgui_draw.cpp"
    "${d012_imgui}/imgui_tables.cpp"
    "${d012_imgui}/imgui_widgets.cpp"
    "${d012_imgui}/backends/imgui_impl_sdl3.cpp"
    "${d012_imgui}/backends/imgui_impl_vulkan.cpp")
target_include_directories(BgmMaker PRIVATE "${d012_imgui}" "${d012_imgui}/backends"
                                            "${D012_PORTABLEKIT_DIR}/third_party")
target_link_libraries(BgmMaker PRIVATE d012_manifest SDL3::SDL3 Vulkan::Vulkan Threads::Threads)
# The game's FFmpeg, so a file reads the same in the tool and the game. Its
# target is PortableKit's (portablekit_use_ffmpeg() reads variables set in
# PortableKit's own directory, so the target is linked here as it is).
if(NOT TARGET portablekit_ffmpeg)
    message(FATAL_ERROR "BgmMaker needs PortableKit's FFmpeg (PORTABLEKIT_FFMPEG=bundled or system)")
endif()
target_link_libraries(BgmMaker PRIVATE portablekit_ffmpeg)
set_target_properties(BgmMaker PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
if(D012_INSTALLER_BUILD)
    # Beside the installed game, whose data/ is always there.
    target_compile_definitions(BgmMaker PRIVATE D012_INSTALLER_BUILD=1)
endif()
# The DLLs it runs with, in bin/ even when only BgmMaker is built.
if(WIN32 AND TARGET SDL3::SDL3-shared)
    add_custom_command(TARGET BgmMaker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_FILE_DIR:BgmMaker>")
endif()
if(MINGW)
    get_filename_component(d012_toolchain_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
    foreach(runtime_dll libc++.dll libunwind.dll)
        if(EXISTS "${d012_toolchain_bin}/${runtime_dll}")
            add_custom_command(TARGET BgmMaker POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${d012_toolchain_bin}/${runtime_dll}"
                        "$<TARGET_FILE_DIR:BgmMaker>")
        endif()
    endforeach()
endif()

# The port's own tests (tools/port/pk_ctest.cmd <build dir>): the manifest
# rules, the JSON writer and the BGM Maker's model. No game needed.
enable_testing()
add_executable(d012_tests "${CMAKE_CURRENT_SOURCE_DIR}/tests/d012_tests.cpp"
                          "${CMAKE_CURRENT_SOURCE_DIR}/bgm_maker/suggest.cpp")
target_link_libraries(d012_tests PRIVATE d012_manifest Threads::Threads)
set_target_properties(d012_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_test(NAME d012_tests COMMAND d012_tests)
