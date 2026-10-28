// The installer build's compile step: `D012Native --compile-game` makes the
// game's code on the player's PC from their own disc, with the recompiler and
// the compiler the installer ships, and puts it in game-code/ beside the
// program, where host/d012_game_code.cpp loads it. It is the C++ form of the
// developer pipeline's generate step (tools/port/setup/build.py and
// tools/port/overlay_meta.py), so the installer needs no Python, CMake or Git.
//
//   D012Native --compile-game [--iso PATH] [--opt N] [--jobs N] [--force] [--keep-work]
//
// --keep-work leaves game-code.new/ (the C++ and the objects), for comparing
// with the developer pipeline's output.
//
// The folder it works in is the program's own (the portable layout of
// host/d012_portable.cpp):
//
//   game/EBOOT.ELF          the decrypted executable `--install <iso>` made
//   psp_recomp.exe          the recompiler
//   toolchain/bin/clang++   llvm-mingw, trimmed (D012_TOOLCHAIN_DIR names
//                           another; D012_RECOMPILER another recompiler)
//   sdk/                    the three headers the game's code includes, the
//                           two entry-point templates and dispatch-calls.txt
//   game-code/              the result: main.dll, overlays/*.dll, key.txt
//
// Steps: the three overlays and LIBSUPPREACC.PRX are read from the disc and
// checked by SHA-256; each overlay's identity (base, size, FNV-1a 64 of its
// first 4,096 bytes, the PRX relocated to its load address with PortableKit's
// own loader) names its corpus, as overlay_meta.py does; psp_recomp runs five
// times (the dispatch-call rows of port/config/dispatch-calls.txt passed to
// each); clang compiles every unit, in parallel, and links main.dll and one
// library per overlay. Every library links on its own (-static): the game's
// code reaches the program only through the corpus ABI. The result is built
// in game-code.new/ and replaces game-code/ only when everything succeeded.
//
// key.txt is a SHA-256 over everything the code is made from (the inputs'
// hashes, the recompiler, the compiler, the headers, the dispatch calls, the
// optimisation level and the ABI versions). A run whose key matches the
// existing game-code/ does nothing, so the installer can run it after every
// update and only a change to the game's code compiles again.
//
// Progress goes to stdout and to game-code-status.txt beside the program
// (key=value lines: state running|done|failed, step, done, total, message),
// which the installer's wizard reads; the tools' output goes to
// game-code-build.log.
//
// MIT licence; see port/README.md.

#include "app_paths.hpp"
#include "install/user_data.hpp"
#include "kernel/iso_image.hpp"
#include "overlay_module.hpp"
#include "profile.hpp"

#include "psprecomp/corpus_abi.hpp"
#include "psprecomp/elf32.hpp"
#include "psprecomp/guest_memory.hpp"
#include "psprecomp/sha256.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <fcntl.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
extern char **environ;
#endif

namespace d012::compile_game {
namespace {

namespace fs = std::filesystem;

constexpr std::uint32_t kMainBase = 0x08804000u;
constexpr std::uint32_t kPrxBase = 0x09B81000u;  // where host/d012_prx.cpp loads LIBSUPPREACC.PRX
constexpr std::uint32_t kHashWindow = 4096u;     // overlay_meta.py's default identity window

struct DiscFile {
    const char *path;
    const char *name;  // the corpus's name
    const char *sha256;
};

// The disc's files the game's code is made from besides the executable, as in
// tools/port/setup/game.py.
constexpr DiscFile kDiscFiles[] = {
    {"PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF", "menu",
     "f021f96fa05da5907563a28c232966aa8a344526d6b8bdee728053ec75224d0a"},
    {"PSP_GAME/USRDIR/DATA/OVL_BATTLE_APP.ELF", "battle",
     "1f96cfb3be771ff6e94741ae3916fab340df74e36ede07ca39545d015f4c49dc"},
    {"PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF", "extra",
     "f6b7be5e15c28774985417f1ec2c64f6655e79cc3c9dce7600fa7f6bacabca01"},
    {"PSP_GAME/USRDIR/DATA/MODULE/LIBSUPPREACC.PRX", "suppreacc",
     "aed4d30f80b1e83561bdfdff4ddfc47dead5b29694be314e253bee69f61f8778"},
};

struct Options {
    fs::path iso;
    int opt = 1;
    unsigned jobs = 0;
    bool force = false;
    bool keep_work = false;
};

struct Paths {
    fs::path folder, game_dir, eboot, iso, recompiler, compiler, sdk, out, work, status, log;
};

struct Error : std::runtime_error {
    using std::runtime_error::runtime_error;
};

std::string text(const fs::path &path) {
    const std::u8string u = path.u8string();
    return {u.begin(), u.end()};
}

fs::path path_of(const char *utf8) {
    return fs::path(std::u8string(utf8, utf8 + std::char_traits<char>::length(utf8)));
}

std::string hex32(std::uint32_t value) {
    char buffer[11];
    std::snprintf(buffer, sizeof buffer, "0x%08X", value);
    return buffer;
}

std::vector<std::uint8_t> read_all(const fs::path &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw Error("cannot read " + text(path));
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

void write_all(const fs::path &path, std::string_view data) {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
    if (!out) throw Error("cannot write " + text(path));
}

// Progress, for the console and for the wizard, which polls the status file.
class Progress {
public:
    explicit Progress(fs::path status) : status_(std::move(status)) {}

    void step(const std::string &name, unsigned total) {
        const std::lock_guard<std::mutex> guard(lock_);
        step_ = name;
        done_ = 0;
        total_ = total;
        std::cout << "[compile-game] " << name << std::endl;
        write_locked("running", name);
    }
    void advance() {
        const std::lock_guard<std::mutex> guard(lock_);
        ++done_;
        if (total_ > 20 && done_ % 10 != 0 && done_ != total_) {
            write_locked("running", step_);
            return;
        }
        std::cout << "[compile-game]   " << done_ << "/" << total_ << std::endl;
        write_locked("running", step_);
    }
    void finish(const std::string &state, const std::string &message) {
        const std::lock_guard<std::mutex> guard(lock_);
        std::cout << "[compile-game] " << message << std::endl;
        write_locked(state, message);
    }

private:
    void write_locked(const std::string &state, const std::string &message) {
        std::ostringstream body;
        body << "state=" << state << "\nstep=" << step_ << "\ndone=" << done_ << "\ntotal=" << total_
             << "\nmessage=" << message << "\n";
        const fs::path partial = status_.string() + ".partial";
        std::error_code ec;
        {
            std::ofstream out(partial, std::ios::binary | std::ios::trunc);
            out << body.str();
        }
        fs::rename(partial, status_, ec);
    }

    fs::path status_;
    std::mutex lock_;
    std::string step_;
    unsigned done_ = 0, total_ = 0;
};

// Runs a program and waits; its output is appended to `log`. Returns the exit
// status, or -1 when it could not start.
int run_program(const std::vector<std::string> &argv, const fs::path &log) {
#if defined(_WIN32)
    auto widen = [](const std::string &s) {
        std::wstring w(static_cast<std::size_t>(MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0)), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, w.data(), static_cast<int>(w.size()));
        w.pop_back();
        return w;
    };
    // The command line as the C runtime parses it back: quotes around each
    // argument, backslashes doubled only before a quote.
    std::wstring line;
    for (const std::string &arg : argv) {
        if (!line.empty()) line += L' ';
        line += L'"';
        const std::wstring w = widen(arg);
        std::size_t slashes = 0;
        for (const wchar_t c : w) {
            if (c == L'\\') {
                ++slashes;
                continue;
            }
            line.append(c == L'"' ? slashes * 2 + 1 : slashes, L'\\');
            slashes = 0;
            line += c;
        }
        line.append(slashes * 2, L'\\');
        line += L'"';
    }
    SECURITY_ATTRIBUTES inherit{sizeof inherit, nullptr, TRUE};
    HANDLE out = CreateFileW(log.wstring().c_str(), FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, &inherit,
                             OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    STARTUPINFOW startup{};
    startup.cb = sizeof startup;
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = INVALID_HANDLE_VALUE;
    startup.hStdOutput = out;
    startup.hStdError = out;
    PROCESS_INFORMATION process{};
    // Below normal priority keeps the PC responsive while every core compiles.
    const BOOL started = CreateProcessW(nullptr, line.data(), nullptr, nullptr, TRUE,
                                        CREATE_NO_WINDOW | BELOW_NORMAL_PRIORITY_CLASS, nullptr, nullptr, &startup,
                                        &process);
    if (out != INVALID_HANDLE_VALUE) CloseHandle(out);
    if (!started) return -1;
    WaitForSingleObject(process.hProcess, INFINITE);
    DWORD code = 1;
    GetExitCodeProcess(process.hProcess, &code);
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return static_cast<int>(code);
#else
    std::vector<char *> args;
    for (const std::string &arg : argv) args.push_back(const_cast<char *>(arg.c_str()));
    args.push_back(nullptr);
    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);
    posix_spawn_file_actions_addopen(&actions, 1, log.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    posix_spawn_file_actions_adddup2(&actions, 1, 2);
    pid_t pid = 0;
    const int failed = posix_spawn(&pid, args[0], &actions, nullptr, args.data(), environ);
    posix_spawn_file_actions_destroy(&actions);
    if (failed != 0) return -1;
    int status = 0;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
#endif
}

void run_checked(const std::vector<std::string> &argv, const Paths &paths, const std::string &what) {
    {
        std::ofstream log(paths.log, std::ios::app);
        log << "\n$";
        for (const std::string &arg : argv) log << " " << arg;
        log << "\n";
    }
    const int code = run_program(argv, paths.log);
    if (code == -1) throw Error("cannot start " + argv.front() + " (" + what + ")");
    if (code != 0) throw Error(what + " failed (exit code " + std::to_string(code) + "); see " + text(paths.log));
}

std::uint64_t fnv1a64(const std::uint8_t *data, std::size_t size) {
    std::uint64_t value = 0xCBF29CE484222325ull;
    for (std::size_t i = 0; i < size; ++i) value = (value ^ data[i]) * 0x100000001B3ull;
    return value;
}

// An overlay as the game has it in memory: its load address and bytes.
struct Image {
    std::uint32_t base{};
    std::vector<std::uint8_t> bytes;
};

Image load_image(const fs::path &file) {
    const psprecomp::Elf32Image elf = psprecomp::Elf32Image::from_file(file);
    Image image;
    if (elf.is_psp_prx()) {
        // Relocated to its load address, as the game sees it.
        std::uint32_t end = 0;
        for (const auto &segment : elf.segments())
            if (segment.type == 1u) end = std::max(end, segment.vaddr + segment.memory_size);
        psprecomp::GuestMemory memory;
        (void)elf.load_and_relocate(memory, kPrxBase);
        image.base = kPrxBase;
        image.bytes.resize(end);
        memory.copy_out(kPrxBase, image.bytes);
        return image;
    }
    // A Dissidia overlay: one PT_LOAD at the overlay window, type 0xFFA8.
    const psprecomp::ElfSegment *load = nullptr;
    for (const auto &segment : elf.segments())
        if (segment.type == 1u) {
            if (load != nullptr) throw Error(text(file) + " has more than one loadable segment");
            load = &segment;
        }
    if (elf.type() != 0xFFA8u || load == nullptr) throw Error(text(file) + " is not an overlay this port knows");
    image.base = load->vaddr;
    image.bytes.assign(elf.bytes().begin() + load->offset, elf.bytes().begin() + load->offset + load->file_size);
    image.bytes.resize(load->memory_size, 0u);
    return image;
}

std::vector<std::string> dispatch_calls(const std::string &dispatch_text, const std::string &name) {
    std::vector<std::string> args;
    std::istringstream lines(dispatch_text);
    std::string line;
    while (std::getline(lines, line)) {
        std::istringstream words(line.substr(0, line.find('#')));
        std::string who, address;
        if (words >> who >> address && who == name) args.insert(args.end(), {"--dispatch-call", address});
    }
    return args;
}

std::string replace_all(std::string text, const std::string &from, const std::string &to) {
    for (std::size_t at = text.find(from); at != std::string::npos; at = text.find(from, at + to.size()))
        text.replace(at, from.size(), to);
    return text;
}

unsigned default_jobs() {
    const unsigned cpus = std::max(1u, std::thread::hardware_concurrency());
    std::uint64_t memory = 0;
#if defined(_WIN32)
    MEMORYSTATUSEX status{};
    status.dwLength = sizeof status;
    if (GlobalMemoryStatusEx(&status)) memory = status.ullTotalPhys;
#endif
    // A unit's clang took under 1 GB at -O1; leave room for the rest of the PC.
    const unsigned by_memory = memory == 0 ? cpus : static_cast<unsigned>(std::max<std::uint64_t>(1, memory / (1536ull << 20)));
    return std::max(1u, std::min(cpus, by_memory));
}

Paths find_paths(const Options &options) {
    Paths p;
    p.folder = portablekit::executable_directory();
    const char *game_dir = std::getenv("D012_GAME_DIR");
    p.game_dir = game_dir != nullptr && *game_dir != '\0' ? path_of(game_dir) : p.folder / "game";
    p.eboot = p.game_dir / "EBOOT.ELF";
    const char *recompiler = std::getenv("D012_RECOMPILER");
    p.recompiler = recompiler != nullptr && *recompiler != '\0' ? path_of(recompiler) : p.folder / "psp_recomp.exe";
    const char *toolchain = std::getenv("D012_TOOLCHAIN_DIR");
    const fs::path toolchain_dir = toolchain != nullptr && *toolchain != '\0' ? path_of(toolchain) : p.folder / "toolchain";
#if defined(_WIN32)
    p.compiler = toolchain_dir / "bin" / "clang++.exe";
#else
    p.compiler = toolchain_dir / "bin" / "clang++";
#endif
    p.sdk = p.folder / "sdk";
    p.out = p.folder / "game-code";
    p.work = p.folder / "game-code.new";
    p.status = p.folder / "game-code-status.txt";
    p.log = p.folder / "game-code-build.log";
    if (!options.iso.empty()) {
        p.iso = options.iso;
    } else {
        for (const fs::path &where : {p.game_dir, portablekit::install::user_data_directory()})
            if (const auto installation = portablekit::install::find_installation(where)) {
                p.iso = installation->disc_image;
                break;
            }
    }
    return p;
}

void check_inputs(const Paths &p) {
    std::error_code ec;
    if (!fs::is_regular_file(p.eboot, ec))
        throw Error("the decrypted executable is missing (" + text(p.eboot) + "): run --install <your ISO> first");
    if (psprecomp::sha256_file(p.eboot) != portablekit::game().executable_sha256)
        throw Error(text(p.eboot) + " is not the ULUS10566 v1.00 executable this port is made for");
    if (p.iso.empty() || !fs::is_regular_file(p.iso, ec))
        throw Error("the disc image is not known; pass --iso <your ULUS10566 .iso>");
    for (const fs::path &tool : {p.recompiler, p.compiler})
        if (!fs::is_regular_file(tool, ec)) throw Error(text(tool) + " is missing; reinstall the port");
    for (const char *name : {"corpus_module.cpp.in", "overlay_module.cpp.in", "dispatch-calls.txt"})
        if (!fs::is_regular_file(p.sdk / name, ec)) throw Error(text(p.sdk / name) + " is missing; reinstall the port");
}

// Everything the game's code is made from, as one SHA-256.
std::string code_key(const Paths &p, const Options &options, const std::vector<std::string> &disc_hashes) {
    std::ostringstream key;
    key << "corpus-abi=" << psprecomp::kCorpusAbiVersion << "\noverlay-abi=" << portablekit::kOverlayAbiVersion
        << "\nopt=" << options.opt << "\neboot=" << portablekit::game().executable_sha256 << "\n";
    for (const std::string &hash : disc_hashes) key << "disc=" << hash << "\n";
    key << "recompiler=" << psprecomp::sha256_file(p.recompiler) << "\ncompiler=" << psprecomp::sha256_file(p.compiler)
        << "\n";
    for (const auto &entry : fs::recursive_directory_iterator(p.sdk))
        if (entry.is_regular_file())
            key << "sdk " << text(fs::relative(entry.path(), p.sdk)) << "=" << psprecomp::sha256_file(entry.path())
                << "\n";
    const std::string all = key.str();
    return psprecomp::sha256_bytes(std::span(reinterpret_cast<const std::uint8_t *>(all.data()), all.size()));
}

struct Library {
    std::string name;  // main.dll, or overlays/<prefix>.dll
    fs::path generated;
    std::vector<std::string> include_dirs;
    std::vector<fs::path> objects;
};

int run(const Options &options) {
    const Paths p = find_paths(options);
    Progress progress(p.status);
    const auto started = std::chrono::steady_clock::now();
    try {
        check_inputs(p);
        std::error_code ec;
        fs::remove(p.log, ec);

        // 1. The disc's files.
        progress.step("Reading the overlays from your disc", std::size(kDiscFiles));
        fs::remove_all(p.work, ec);
        fs::create_directories(p.work / "disc", ec);
        std::vector<std::string> disc_hashes;
        std::vector<fs::path> disc_paths;
        {
            portablekit::IsoImage disc(p.iso);
            for (const DiscFile &file : kDiscFiles) {
                const auto entry = disc.find(file.path);
                if (!entry || entry->directory) throw Error(std::string("the disc has no ") + file.path);
                std::vector<std::uint8_t> bytes(entry->size);
                if (disc.read(std::uint64_t(entry->lba) * portablekit::IsoImage::kSectorSize, bytes) != bytes.size())
                    throw Error(std::string("cannot read ") + file.path + " from the disc");
                const std::string hash = psprecomp::sha256_bytes(bytes);
                if (hash != file.sha256)
                    throw Error(std::string(file.path) + " on the disc is not the ULUS10566 v1.00 file (SHA-256 " +
                                hash + ")");
                const fs::path target = p.work / "disc" / fs::path(file.path).filename();
                write_all(target, std::string_view(reinterpret_cast<const char *>(bytes.data()), bytes.size()));
                disc_hashes.push_back(hash);
                disc_paths.push_back(target);
                progress.advance();
            }
        }

        const std::string key = code_key(p, options, disc_hashes);
        if (!options.force && fs::is_regular_file(p.out / "main.dll", ec)) {
            std::ifstream existing(p.out / "key.txt");
            std::string have;
            std::getline(existing, have);
            if (have == key) {
                fs::remove_all(p.work, ec);
                progress.finish("done", "The game's code is already compiled and up to date.");
                return 0;
            }
        }

        // 2. The C++ code, from psp_recomp.
        const auto bytes = read_all(p.sdk / "dispatch-calls.txt");
        const std::string dispatch(bytes.begin(), bytes.end());
        const std::string recompiler = text(p.recompiler);
        std::vector<Library> libraries;
        progress.step("Recompiling the game's code to C++", 1 + std::size(kDiscFiles));
        {
            Library main{"main.dll", p.work / "generated", {}, {}};
            std::vector<std::string> args{recompiler, text(p.eboot), "--auto", text(main.generated), hex32(kMainBase)};
            const auto rows = dispatch_calls(dispatch, "main");
            args.insert(args.end(), rows.begin(), rows.end());
            run_checked(args, p, "psp_recomp for the main executable");
            const auto entry = read_all(p.sdk / "corpus_module.cpp.in");
            write_all(main.generated / "main_module.cpp", std::string_view(reinterpret_cast<const char *>(entry.data()), entry.size()));
            libraries.push_back(std::move(main));
            progress.advance();
        }
        const auto template_bytes = read_all(p.sdk / "overlay_module.cpp.in");
        const std::string overlay_template(template_bytes.begin(), template_bytes.end());
        for (std::size_t i = 0; i < std::size(kDiscFiles); ++i) {
            const Image image = load_image(disc_paths[i]);
            const std::uint32_t code_size = std::min<std::uint32_t>(
                kHashWindow - portablekit::kOverlayHeaderBytes,
                static_cast<std::uint32_t>(image.bytes.size()) - portablekit::kOverlayHeaderBytes);
            const std::uint64_t hash = fnv1a64(image.bytes.data(), portablekit::kOverlayHeaderBytes + code_size);
            char prefix[64];
            std::snprintf(prefix, sizeof prefix, "ovl%08X_%s_%016llX", image.base, kDiscFiles[i].name,
                          static_cast<unsigned long long>(hash));
            char hash_text[24];
            std::snprintf(hash_text, sizeof hash_text, "0x%016llX", static_cast<unsigned long long>(hash));
            Library overlay{std::string("overlays/") + prefix + ".dll", p.work / "overlays" / prefix, {}, {}};
            std::vector<std::string> args{recompiler, text(disc_paths[i]), "--auto", text(overlay.generated),
                                          hex32(image.base), "--prefix", prefix};
            const auto rows = dispatch_calls(dispatch, kDiscFiles[i].name);
            args.insert(args.end(), rows.begin(), rows.end());
            run_checked(args, p, std::string("psp_recomp for the ") + kDiscFiles[i].name + " overlay");
            std::string entry = overlay_template;
            entry = replace_all(entry, "@PORTABLEKIT_OVERLAY_PREFIX@", prefix);
            entry = replace_all(entry, "@PORTABLEKIT_OVERLAY_BASE@", hex32(image.base));
            entry = replace_all(entry, "@PORTABLEKIT_OVERLAY_SIZE@", std::to_string(image.bytes.size()));
            entry = replace_all(entry, "@PORTABLEKIT_OVERLAY_CODE_SIZE@", std::to_string(code_size));
            entry = replace_all(entry, "@PORTABLEKIT_OVERLAY_HASH@", hash_text);
            entry = replace_all(entry, "@PORTABLEKIT_OVERLAY_NAME@", kDiscFiles[i].name);
            write_all(overlay.generated / (std::string(prefix) + "_module.cpp"), entry);
            overlay.include_dirs.push_back(text(overlay.generated));
            libraries.push_back(std::move(overlay));
            progress.advance();
        }

        // 3. Compile every unit, in parallel.
        struct Unit {
            fs::path source, object;
            const Library *library;
        };
        std::vector<Unit> units;
        fs::create_directories(p.work / "objects", ec);
        for (Library &library : libraries) {
            std::vector<fs::path> sources;
            for (const auto &entry : fs::directory_iterator(library.generated))
                if (entry.path().extension() == ".cpp") sources.push_back(entry.path());
            std::sort(sources.begin(), sources.end());
            for (const fs::path &source : sources) {
                const fs::path object = p.work / "objects" / ("u" + std::to_string(units.size()) + ".o");
                library.objects.push_back(object);
                units.push_back({source, object, &library});
            }
        }
        const unsigned jobs = options.jobs != 0 ? options.jobs : default_jobs();
        progress.step("Compiling the game's code (" + std::to_string(units.size()) + " parts, " +
                          std::to_string(jobs) + " at a time)",
                      static_cast<unsigned>(units.size()));
        const std::string compiler = text(p.compiler);
        const std::string sdk_include = text(p.sdk / "include");
        std::atomic<std::size_t> next{0};
        std::atomic<bool> failed{false};
        std::string failure;
        std::mutex failure_lock;
        std::vector<std::thread> workers;
        for (unsigned j = 0; j < std::min<std::size_t>(jobs, units.size()); ++j)
            workers.emplace_back([&] {
                for (std::size_t i = next++; i < units.size() && !failed; i = next++) {
                    const Unit &unit = units[i];
                    std::vector<std::string> args{compiler, "-std=c++20", "-O" + std::to_string(options.opt), "-g0",
                                                  "-fvisibility=hidden", "-fvisibility-inlines-hidden", "-w",
                                                  "-DNDEBUG", "-I", sdk_include, "-I", text(unit.library->generated)};
                    args.insert(args.end(), {"-c", text(unit.source), "-o", text(unit.object)});
                    const fs::path unit_log = p.work / "objects" / ("u" + std::to_string(i) + ".log");
                    const int code = run_program(args, unit_log);
                    if (code != 0) {
                        const std::lock_guard<std::mutex> guard(failure_lock);
                        if (!failed.exchange(true)) {
                            failure = "compiling " + text(unit.source.filename()) + " failed (exit code " +
                                      std::to_string(code) + "); see " + text(unit_log);
                        }
                        return;
                    }
                    std::error_code remove_ec;
                    fs::remove(unit_log, remove_ec);
                    progress.advance();
                }
            });
        for (std::thread &worker : workers) worker.join();
        if (failed) throw Error(failure);

        // 4. Link: every library on its own, with its own C++ runtime.
        progress.step("Linking the game's code", static_cast<unsigned>(libraries.size()));
        fs::create_directories(p.work / "result" / "overlays", ec);
        for (const Library &library : libraries) {
            const fs::path rsp = p.work / "objects" / (fs::path(library.name).stem().string() + ".rsp");
            {
                std::ofstream out(rsp, std::ios::binary);
                for (const fs::path &object : library.objects) {
                    std::string name = text(object);
                    std::replace(name.begin(), name.end(), '\\', '/');
                    out << '"' << name << "\"\n";
                }
            }
            run_checked({compiler, "-shared", "-static", "-static-libgcc", "-static-libstdc++", "-o",
                         text(p.work / "result" / library.name), "@" + text(rsp)},
                        p, "linking " + library.name);
            progress.advance();
        }
        write_all(p.work / "result" / "key.txt", key + "\n");

        // 5. Replace game-code/ with the result.
        const fs::path old = p.folder / "game-code.old";
        fs::remove_all(old, ec);
        if (fs::exists(p.out, ec)) {
            fs::rename(p.out, old, ec);
            if (ec) throw Error("cannot replace " + text(p.out) + " (is the game running?): " + ec.message());
        }
        fs::rename(p.work / "result", p.out, ec);
        if (ec) throw Error("cannot move the result to " + text(p.out) + ": " + ec.message());
        fs::remove_all(old, ec);
        if (!options.keep_work) fs::remove_all(p.work, ec);
        const auto seconds =
            std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - started).count();
        progress.finish("done", "The game's code is compiled (" + std::to_string(units.size()) + " parts in " +
                                    std::to_string(seconds / 60) + " min " + std::to_string(seconds % 60) + " s).");
        return 0;
    } catch (const std::exception &e) {
        progress.finish("failed", e.what());
        return 1;
    }
}

} // namespace

int main(int argc, char **argv) {
    Options options;
    for (int i = 2; i < argc; ++i) {
        const std::string_view arg = argv[i];
        auto value = [&]() -> const char * {
            if (i + 1 >= argc) {
                std::cerr << "--compile-game: " << arg << " needs a value\n";
                std::exit(2);
            }
            return argv[++i];
        };
        if (arg == "--iso") {
            options.iso = path_of(value());
        } else if (arg == "--opt") {
            options.opt = std::clamp(std::atoi(value()), 0, 3);
        } else if (arg == "--jobs") {
            options.jobs = static_cast<unsigned>(std::max(0, std::atoi(value())));
        } else if (arg == "--force") {
            options.force = true;
        } else if (arg == "--keep-work") {
            options.keep_work = true;
        } else {
            std::cerr << "--compile-game: unknown option " << arg
                      << "\nusage: D012Native --compile-game [--iso PATH] [--opt N] [--jobs N] [--force] [--keep-work]\n";
            return 2;
        }
    }
    return run(options);
}

} // namespace d012::compile_game

// The program's entry point in the installer build: PortableKit's main() is
// compiled as d012_portablekit_main (portablekit_add_game(... HOST_MAIN_NAME)).
int d012_portablekit_main(int argc, char **argv);

int main(int argc, char **argv) {
    if (argc >= 2 && std::string_view(argv[1]) == "--compile-game") return d012::compile_game::main(argc, argv);
    return d012_portablekit_main(argc, argv);
}
