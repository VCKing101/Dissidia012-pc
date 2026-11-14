// The installer build's compile step: `D012Native --compile-game` makes the
// game's code on the player's PC from their own disc, with the recompiler and
// the compiler the installer ships, and puts it in game-code/ beside the
// program, where host/d012_game_code.cpp loads it. It is the C++ form of the
// developer pipeline's generate step (tools/port/setup/build.py and
// tools/port/overlay_meta.py), so the installer needs no Python, CMake or Git.
//
//   D012Native --compile-game [--iso PATH] [--opt N] [--jobs N] [--force] [--keep-work] [--launcher-progress]
//
// --keep-work leaves game-code.new/ (the C++ and the objects), for comparing
// with the developer pipeline's output.
//
// The folder it works in is the program's own (the portable layout of
// host/d012_portable.cpp):
//
//   data/EBOOT.ELF          the decrypted executable `--install <iso>` made,
//                           and data/settings.ini naming the disc image (or
//                           game/EBOOT.ELF and game/disc.iso: D012_GAME_DIR)
//   psp_recomp.exe          the recompiler
//   toolchain/bin/clang++   llvm-mingw, trimmed; on Linux clang and lld with
//                           a sysroot that clang++.cfg beside it names
//                           (D012_TOOLCHAIN_DIR names another;
//                           D012_RECOMPILER another recompiler)
//   sdk/                    the three headers the game's code includes, the
//                           two entry-point templates, dispatch-calls.txt and
//                           d012_math.cpp
//   game-code/              the result: main.dll, overlays/*.dll, key.txt
//                           (main.so, overlays/*.so on Linux)
//   dissidia012.png         on Linux, the disc's ICON0.PNG, for the menu entry
//
// Steps: the three overlays and LIBSUPPREACC.PRX are read from the disc and
// checked by SHA-256; each overlay's identity (base, size, FNV-1a 64 of its
// first 4,096 bytes, the PRX relocated to its load address with PortableKit's
// own loader) names its corpus, as overlay_meta.py does; psp_recomp runs five
// times (the dispatch-call rows of port/config/dispatch-calls.txt passed to
// each); clang compiles every unit, in parallel, and links main.dll and one
// library per overlay. Every library links on its own (-static): the game's
// code reaches the program only through the corpus ABI. Each also gets the
// port's own sinf, cosf, asinf, log2f and exp2f (d012_math.cpp), so its maths
// round alike on every machine, as the program's do; linked on its own, it
// would take the C runtime's (docs/port/communication-mode.md, section 13). The result is built
// in game-code.new/ and replaces game-code/ only when everything succeeded.
//
// key.txt holds two SHA-256 keys, so the installer can run this after every
// update and only a change to the game's code compiles again:
//
//   line 1     the input key: everything the code is made from (the inputs'
//              hashes, the recompiler, the compiler, the sdk/ files, the
//              optimisation level and the ABI versions). When it matches, the
//              run stops after reading the disc, in about a second.
//   source=    the source key: everything clang and the linker are given (each
//              file psp_recomp wrote and the entry points beside it, the sdk/
//              files, the compiler and its flags). When the input key differs
//              but psp_recomp writes the same C++ as before (a new recompiler
//              whose changes are to the runtime it shares with the program, not
//              to the code it writes), the libraries stand and only key.txt is
//              rewritten: the run costs the five recompiles only (64 s
//              against 3 min 31 s for a whole compile, 10 November 2026).
//
// A key.txt of 0.7.0 or before has the first line only, so the first update
// from one compiles in full.
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
#include <utility>
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

// The game's code as this system loads it (PortableKit's host/corpus_library.cpp).
#if defined(_WIN32)
constexpr const char *kLibrary = ".dll";
constexpr const char *kRecompiler = "psp_recomp.exe";
#else
constexpr const char *kLibrary = ".so";
constexpr const char *kRecompiler = "psp_recomp";
#endif

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
    // --prepare: RecompLauncher runs the setup, so progress is printed as "progress <0 to 1> <text>".
    bool launcher = false;
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

// Progress, for the console and for the wizard, which reads the lines
// "[compile-game] <percent>% <message>" from stdout (or polls the status
// file). The percentage weighs the four steps by their measured share of a
// compile: the disc 2%, psp_recomp 25%, clang 70%, linking 3%.
class Progress {
public:
    Progress(fs::path status, bool launcher) : status_(std::move(status)), launcher_(launcher) {}

    void step(const std::string &name, unsigned total) {
        const std::lock_guard<std::mutex> guard(lock_);
        if (!step_.empty()) start_ += kWeights[std::min<std::size_t>(index_++, std::size(kWeights) - 1)];
        step_ = name;
        done_ = 0;
        total_ = total;
        print_locked();
        write_locked("running", name);
    }
    void advance() {
        const std::lock_guard<std::mutex> guard(lock_);
        ++done_;
        if (total_ <= 20 || done_ % 4 == 0 || done_ == total_) print_locked();
        write_locked("running", step_);
    }
    void finish(const std::string &state, const std::string &message) {
        const std::lock_guard<std::mutex> guard(lock_);
        if (state == "done") start_ = 100, done_ = total_ = 0;
        print_line(message);
        write_locked(state, message);
    }

private:
    static constexpr unsigned kWeights[] = {2u, 25u, 70u, 3u};

    unsigned percent() const {
        const unsigned weight = kWeights[std::min<std::size_t>(index_, std::size(kWeights) - 1)];
        return std::min(100u, start_ + (total_ == 0 ? 0u : weight * done_ / total_));
    }
    void print_locked() {
        std::string text = step_;
        if (total_ > 1) text += " (" + std::to_string(done_) + "/" + std::to_string(total_) + ")";
        print_line(text);
    }
    void print_line(const std::string &text) const {
        if (!launcher_) {
            std::cout << "[compile-game] " << percent() << "% " << text << std::endl;
            return;
        }
        // The launcher's setup screen shows --install's share first (see prepare() below), then this.
        const double fraction = (kInstallShare + (1.0 - kInstallShare) * percent() / 100.0);
        char number[16];
        std::snprintf(number, sizeof number, "%.3f", fraction);
        std::cout << "progress " << number << " " << text << std::endl;
    }
    void write_locked(const std::string &state, const std::string &message) {
        std::ostringstream body;
        body << "state=" << state << "\npercent=" << percent() << "\nstep=" << step_ << "\ndone=" << done_
             << "\ntotal=" << total_ << "\nmessage=" << message << "\n";
        const fs::path partial = status_.string() + ".partial";
        std::error_code ec;
        {
            std::ofstream out(partial, std::ios::binary | std::ios::trunc);
            out << body.str();
        }
        fs::rename(partial, status_, ec);
    }

    static constexpr double kInstallShare = 0.05;

    fs::path status_;
    bool launcher_ = false;
    std::mutex lock_;
    std::string step_;
    unsigned done_ = 0, total_ = 0;
    std::size_t index_ = 0;
    unsigned start_ = 0;  // the percentage the current step starts at
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
#else
    const long pages = sysconf(_SC_PHYS_PAGES), page = sysconf(_SC_PAGE_SIZE);
    if (pages > 0 && page > 0) memory = std::uint64_t(pages) * std::uint64_t(page);
#endif
    // A unit's clang took under 1 GB at -O1; leave room for the rest of the PC.
    const unsigned by_memory = memory == 0 ? cpus : static_cast<unsigned>(std::max<std::uint64_t>(1, memory / (1536ull << 20)));
    return std::max(1u, std::min(cpus, by_memory));
}

Paths find_paths(const Options &options) {
    Paths p;
    p.folder = portablekit::executable_directory();
    // The executable and the disc image as the game finds them: a game/
    // folder when D012_GAME_DIR names one (game/disc.iso), otherwise what
    // `--install <iso>` set up in the data directory (host/d012_portable.cpp).
    if (const char *game_dir = std::getenv("D012_GAME_DIR"); game_dir != nullptr && *game_dir != '\0') {
        p.game_dir = path_of(game_dir);
        p.eboot = p.game_dir / "EBOOT.ELF";
        p.iso = p.game_dir / "disc.iso";
    } else {
        p.game_dir = portablekit::install::user_data_directory();
        p.eboot = p.game_dir / "EBOOT.ELF";
        if (const auto installation = portablekit::install::find_installation(p.game_dir)) {
            p.eboot = installation->executable;
            p.iso = installation->disc_image;
        }
    }
    if (!options.iso.empty()) p.iso = options.iso;
    const char *recompiler = std::getenv("D012_RECOMPILER");
    p.recompiler = recompiler != nullptr && *recompiler != '\0' ? path_of(recompiler) : p.folder / kRecompiler;
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
    return p;
}

void check_inputs(const Paths &p) {
    std::error_code ec;
    if (!fs::is_regular_file(p.eboot, ec))
        throw Error("the decrypted executable is missing (" + text(p.eboot) + "): run --install <your ISO> first");
    if (p.iso.empty()) throw Error("the disc image is not known; pass --iso <your ULUS10566 or ULES01505 .iso>");
    if (psprecomp::sha256_file(p.eboot) != portablekit::game().executable_sha256)
        throw Error(text(p.eboot) + " is not the v1.00 executable this port is made for");
    if (p.iso.empty() || !fs::is_regular_file(p.iso, ec))
        throw Error("the disc image is not known; pass --iso <your ULUS10566 or ULES01505 .iso>");
    for (const fs::path &tool : {p.recompiler, p.compiler})
        if (!fs::is_regular_file(tool, ec)) throw Error(text(tool) + " is missing; reinstall the port");
    for (const char *name : {"corpus_module.cpp.in", "overlay_module.cpp.in", "dispatch-calls.txt", "d012_math.cpp"})
        if (!fs::is_regular_file(p.sdk / name, ec)) throw Error(text(p.sdk / name) + " is missing; reinstall the port");
}

// What clang is given for each unit, and the linker for each library, besides
// the files' paths.
std::vector<std::string> compile_flags(const Options &options) {
    std::vector<std::string> flags{"-std=c++20", "-O" + std::to_string(options.opt), "-g0", "-fvisibility=hidden",
                                   "-fvisibility-inlines-hidden", "-w", "-DNDEBUG"};
#if !defined(_WIN32)
    flags.push_back("-fPIC");
#endif
    return flags;
}

std::vector<std::string> link_flags() {
#if defined(_WIN32)
    return {"-shared", "-static", "-static-libgcc", "-static-libstdc++"};
#else
    // Its C++ runtime linked in and kept to itself, as PortableKit links a
    // release's overlay libraries (cmake/PortableKit.cmake).
    return {"-shared", "-fuse-ld=lld", "-static-libgcc", "-static-libstdc++", "-Wl,--exclude-libs,ALL"};
#endif
}

// The files under a folder, by their paths relative to it with '/' between
// names, in name order: the same list on every system and file system.
std::vector<std::pair<std::string, fs::path>> files_under(const fs::path &folder) {
    std::vector<std::pair<std::string, fs::path>> files;
    for (const auto &entry : fs::recursive_directory_iterator(folder))
        if (entry.is_regular_file()) {
            const std::u8string relative = fs::relative(entry.path(), folder).generic_u8string();
            files.emplace_back(std::string(relative.begin(), relative.end()), entry.path());
        }
    std::sort(files.begin(), files.end());
    return files;
}

std::string sha256_text(const std::string &text) {
    return psprecomp::sha256_bytes(std::span(reinterpret_cast<const std::uint8_t *>(text.data()), text.size()));
}

// The compiler, its configuration and sdk/, which both keys name.
void add_tools(std::ostringstream &key, const Paths &p) {
    key << "compiler=" << psprecomp::sha256_file(p.compiler) << "\n";
    // On Linux the compiler's target and sysroot come from the configuration
    // file beside it (tools/port/linux_toolchain.sh).
    std::error_code config_ec;
    if (const fs::path config = fs::path(p.compiler).concat(".cfg"); fs::is_regular_file(config, config_ec))
        key << "compiler-config=" << psprecomp::sha256_file(config) << "\n";
    for (const auto &[name, path] : files_under(p.sdk)) key << "sdk " << name << "=" << psprecomp::sha256_file(path) << "\n";
}

// The input key: everything the game's code is made from, as one SHA-256.
std::string code_key(const Paths &p, const Options &options, const std::vector<std::string> &disc_hashes) {
    std::ostringstream key;
    key << "corpus-abi=" << psprecomp::kCorpusAbiVersion << "\noverlay-abi=" << portablekit::kOverlayAbiVersion
        << "\nopt=" << options.opt << "\neboot=" << portablekit::game().executable_sha256 << "\n";
    for (const std::string &hash : disc_hashes) key << "disc=" << hash << "\n";
    key << "recompiler=" << psprecomp::sha256_file(p.recompiler) << "\n";
    add_tools(key, p);
    return sha256_text(key.str());
}

struct Library {
    std::string name;  // main.dll, or overlays/<prefix>.dll (.so on Linux)
    fs::path generated;
    std::vector<std::string> include_dirs;
    std::vector<fs::path> objects;
};

// The source key: everything clang and the linker are given, as one SHA-256.
// The same C++ compiled by the same compiler with the same flags and headers
// is the same code, whatever wrote the C++.
std::string source_key(const Paths &p, const Options &options, const std::vector<Library> &libraries) {
    std::ostringstream key;
    key << "corpus-abi=" << psprecomp::kCorpusAbiVersion << "\noverlay-abi=" << portablekit::kOverlayAbiVersion
        << "\ncompile";
    for (const std::string &flag : compile_flags(options)) key << " " << flag;
    key << "\nlink";
    for (const std::string &flag : link_flags()) key << " " << flag;
    key << "\n";
    add_tools(key, p);
    for (const Library &library : libraries) {
        key << "library " << library.name << "\n";
        for (const auto &[name, path] : files_under(library.generated))
            key << "  " << name << "=" << psprecomp::sha256_file(path) << "\n";
    }
    return sha256_text(key.str());
}

// game-code/key.txt: the input key on its first line, then source=<the
// source key> (none in a key.txt of 0.7.0 or before).
struct StoredKeys {
    std::string input, source;
};

StoredKeys read_keys(const fs::path &file) {
    StoredKeys keys;
    std::ifstream in(file);
    std::string line;
    for (bool first = true; std::getline(in, line); first = false) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (first) keys.input = line;
        else if (line.rfind("source=", 0) == 0) keys.source = line.substr(7);
    }
    return keys;
}

std::string keys_text(const std::string &input, const std::string &source) {
    return input + "\nsource=" + source + "\n";
}

// A line of this run's own in game-code-build.log, among the tools' output.
void note(const Paths &p, const std::string &line) {
    std::ofstream log(p.log, std::ios::app);
    log << "\n[compile-game] " << line << "\n";
}

int run(const Options &options) {
    const Paths p = find_paths(options);
    Progress progress(p.status, options.launcher);
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
                    throw Error(std::string(file.path) + " on the disc is not the v1.00 file (SHA-256 " +
                                hash + ")");
                const fs::path target = p.work / "disc" / fs::path(file.path).filename();
                write_all(target, std::string_view(reinterpret_cast<const char *>(bytes.data()), bytes.size()));
                disc_hashes.push_back(hash);
                disc_paths.push_back(target);
                progress.advance();
            }
#if !defined(_WIN32)
            // The menu entry's icon, the disc's own (the Linux setup carries
            // none of the game's, tools/port/linux_setup/AppRun); not needed,
            // so a disc without it is no error.
            if (const auto icon = disc.find("PSP_GAME/ICON0.PNG"); icon && !icon->directory) {
                std::vector<std::uint8_t> bytes(icon->size);
                if (disc.read(std::uint64_t(icon->lba) * portablekit::IsoImage::kSectorSize, bytes) == bytes.size())
                    write_all(p.folder / "dissidia012.png",
                              std::string_view(reinterpret_cast<const char *>(bytes.data()), bytes.size()));
            }
#endif
        }

        const std::string key = code_key(p, options, disc_hashes);
        StoredKeys stored;
        if (!options.force && fs::is_regular_file(p.out / (std::string("main") + kLibrary), ec))
            stored = read_keys(p.out / "key.txt");
        if (!stored.input.empty() && stored.input == key) {
            note(p, "the input key matches game-code/key.txt: nothing to do");
            fs::remove_all(p.work, ec);
            progress.finish("done", "The game's code is already compiled and up to date.");
            return 0;
        }

        // 2. The C++ code, from psp_recomp.
        const auto bytes = read_all(p.sdk / "dispatch-calls.txt");
        const std::string dispatch(bytes.begin(), bytes.end());
        const std::string recompiler = text(p.recompiler);
        std::vector<Library> libraries;
        progress.step("Recompiling the game's code to C++", 1 + std::size(kDiscFiles));
        {
            Library main{std::string("main") + kLibrary, p.work / "generated", {}, {}};
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
            Library overlay{std::string("overlays/") + prefix + kLibrary, p.work / "overlays" / prefix, {}, {}};
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

        // 3. The C++ as the existing game-code/ was compiled from: its
        // libraries stand, and only key.txt changes.
        const std::string source = source_key(p, options, libraries);
        if (!stored.source.empty() && stored.source == source &&
            std::all_of(libraries.begin(), libraries.end(), [&](const Library &library) {
                std::error_code exists_ec;
                return fs::is_regular_file(p.out / library.name, exists_ec);
            })) {
            const fs::path partial = p.out / "key.txt.partial";
            write_all(partial, keys_text(key, source));
            fs::rename(partial, p.out / "key.txt", ec);
            if (ec) throw Error("cannot write " + text(p.out / "key.txt") + ": " + ec.message());
            note(p, "the input key changed but the source key matches game-code/key.txt: the C++ is unchanged, "
                    "so the libraries stand");
            if (!options.keep_work) fs::remove_all(p.work, ec);
            progress.finish("done", "The game's code is already compiled and up to date.");
            return 0;
        }
        note(p, stored.source.empty() ? "no source key in game-code/key.txt: compiling"
                                      : "the source key differs from game-code/key.txt: compiling");

        // 4. Compile every unit, in parallel.
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
            sources.push_back(p.sdk / "d012_math.cpp");
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
        const std::vector<std::string> flags = compile_flags(options);
        std::atomic<std::size_t> next{0};
        std::atomic<bool> failed{false};
        std::string failure;
        std::mutex failure_lock;
        std::vector<std::thread> workers;
        for (unsigned j = 0; j < std::min<std::size_t>(jobs, units.size()); ++j)
            workers.emplace_back([&] {
                for (std::size_t i = next++; i < units.size() && !failed; i = next++) {
                    const Unit &unit = units[i];
                    std::vector<std::string> args{compiler};
                    args.insert(args.end(), flags.begin(), flags.end());
                    args.insert(args.end(), {"-I", sdk_include, "-I", text(unit.library->generated), "-c",
                                             text(unit.source), "-o", text(unit.object)});
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

        // 5. Link: every library on its own, with its own C++ runtime.
        progress.step("Linking the game's code", static_cast<unsigned>(libraries.size()));
        fs::create_directories(p.work / "result" / "overlays", ec);
        const std::vector<std::string> linking = link_flags();
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
            std::vector<std::string> args{compiler};
            args.insert(args.end(), linking.begin(), linking.end());
            args.insert(args.end(), {"-o", text(p.work / "result" / library.name), "@" + text(rsp)});
            run_checked(args, p, "linking " + library.name);
            progress.advance();
        }
        write_all(p.work / "result" / "key.txt", keys_text(key, source));

        // 6. Replace game-code/ with the result.
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
        } else if (arg == "--launcher-progress") {
            options.launcher = true;
        } else {
            std::cerr << "--compile-game: unknown option " << arg
                      << "\nusage: D012Native --compile-game [--iso PATH] [--opt N] [--jobs N] [--force] [--keep-work] "
                         "[--launcher-progress]\n";
            return 2;
        }
    }
    return run(options);
}

} // namespace d012::compile_game

// The program's entry point in the installer build: PortableKit's main() is
// compiled as d012_portablekit_main (portablekit_add_game(... HOST_MAIN_NAME)).
int d012_portablekit_main(int argc, char **argv);

namespace {

// PortableKit's main restarts the program on Linux to raise the stack (host/main.cpp, ensure_main_stack), with the
// arguments it was given. Called here with made-up arguments, that restart would become the whole run, so the marker
// that says "already raised" is set around such a call and cleared before the game itself starts.
void set_stack_marker(bool on) {
#if defined(__linux__)
    const std::string marker = portablekit::env_name("STACK_RAISED");
    if (on) setenv(marker.c_str(), "1", 1);
    else unsetenv(marker.c_str());
#else
    (void)on;
#endif
}

// The same path, as the Windows setup compares them (AlreadyInstalledFrom in tools/port/installer/dissidia012.iss):
// both made absolute and normal, and on Windows without regard to case.
bool same_path(const std::filesystem::path &a, const std::filesystem::path &b) {
    std::error_code ec;
    const std::filesystem::path x = std::filesystem::absolute(a, ec).lexically_normal();
    const std::filesystem::path y = std::filesystem::absolute(b, ec).lexically_normal();
#if defined(_WIN32)
    const std::wstring &l = x.native(), &r = y.native();
    return CompareStringOrdinal(l.c_str(), static_cast<int>(l.size()), r.c_str(), static_cast<int>(r.size()), TRUE) ==
           CSTR_EQUAL;
#else
    return x == y;
#endif
}

// True when `--install` has already set up this disc image: data/EBOOT.ELF is there and settings.ini's disc_image
// names this file. Then the install would only decrypt the same executable again.
bool installed_from(const std::string &iso) {
    try {
        const auto installation =
            portablekit::install::find_installation(portablekit::install::user_data_directory());
        return installation && same_path(installation->disc_image, portablekit::install::path_from_utf8(iso));
    } catch (const std::exception &) {
        return false;
    }
}

} // namespace

// The installer's `--install <iso> --in-place`: decrypts the executable and remembers where the disc image is.
// Skipped when that disc image is already the one set up.
static int install_from(char *program, std::string &iso) {
    if (installed_from(iso)) return 0;
    std::vector<char *> install{program, const_cast<char *>("--install"), iso.data(), const_cast<char *>("--in-place"),
                                nullptr};
    set_stack_marker(true);
    const int code = d012_portablekit_main(static_cast<int>(install.size()) - 1, install.data());
    set_stack_marker(false);
    return code;
}

// `D012Native --game <iso>` plays from that disc image wherever it is, for launchers, scripts and Steam shortcuts:
// the setup's first step (skipped when the image is the one already set up), then the game as usual. The game's
// code must already be compiled (the setup, or --prepare, does that). Only this exact form is taken. On Linux the
// game's start may restart the program with the larger stack; it restarts as a plain start, which plays the copy
// just set up.
static int play_from(char *program, std::string iso) {
    const int install_code = install_from(program, iso);
    if (install_code != 0) return install_code;
    std::vector<char *> start{program, nullptr};
    return d012_portablekit_main(1, start.data());
}

// Launcher installs are updated by the launcher, so the game's own check at start, which points to the release
// page and its setup, is turned off the first time. A choice the player makes in the game's options stays.
static void leave_updates_to_the_launcher() {
    try {
        const std::filesystem::path dir = portablekit::install::user_data_directory();
        portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
        if (entries.contains("updates.check_at_start")) return;
        entries["updates.check_at_start"] = "0";
        portablekit::install::write_settings_file(dir, entries);
    } catch (const std::exception &e) {
        std::cerr << "--prepare: settings.ini not written for updates.check_at_start: " << e.what() << "\n";
    }
}

// `D012Native --prepare --game <iso>` is the setup RecompLauncher runs without a window (recomplauncher.json,
// "prepare"): the two steps of the installer, `--install <iso> --in-place` and `--compile-game`, one after the other,
// printing "progress <0 to 1> <text>" lines. The first is skipped and the second quick when nothing changed, so it runs again on every change
// of the player's copy. Without --prepare nothing here is reached.
static int prepare(int argc, char **argv) {
    std::string iso;
    for (int i = 2; i < argc; ++i) {
        const std::string_view arg = argv[i];
        if (arg == "--game" && i + 1 < argc) {
            iso = argv[++i];
        } else {
            std::cerr << "usage: " << argv[0] << " --prepare --game <your ULUS10566 or ULES01505 .iso>\n";
            return 2;
        }
    }
    if (iso.empty()) {
        std::cerr << "--prepare needs --game <your ULUS10566 or ULES01505 .iso>\n";
        return 2;
    }
    std::cout << "progress 0.000 Reading your disc and decrypting the game's program" << std::endl;
    const int install_code = install_from(argv[0], iso);
    if (install_code != 0) return install_code;
    leave_updates_to_the_launcher();
    std::vector<char *> compile{argv[0], const_cast<char *>("--compile-game"),
                                const_cast<char *>("--launcher-progress"), nullptr};
    return d012::compile_game::main(static_cast<int>(compile.size()) - 1, compile.data());
}

int main(int argc, char **argv) {
    if (argc == 3 && std::string_view(argv[1]) == "--game") return play_from(argv[0], argv[2]);
    if (argc >= 2 && std::string_view(argv[1]) == "--prepare") return prepare(argc, argv);
    if (argc >= 2 && std::string_view(argv[1]) == "--compile-game") return d012::compile_game::main(argc, argv);
    return d012_portablekit_main(argc, argv);
}
