// The crash report: on an unhandled exception (an access violation and the
// like, in any thread, at any time including the C runtime's exit) or an
// abort, log the exception and the faulting thread's stack to stderr, which
// in a player's install is data/logs/D012Native.log, and write a minidump
// with every thread's stack beside the log (data/logs/D012Native-crash.dmp,
// replaced by the next crash). Always on, so a player's crash report says
// where it happened without any setting (issue 7: a crash in the shop left a
// log that just stopped).
//
// D012_CRASH_DUMP=<dir> writes <dir>/crash_<pid>.dmp instead, one per run, for
// the test tools (check_quit_crash.py). With neither it nor D012_DATA_DIR set,
// only the log lines are written.
//
// D012_TEST_CRASH=<seconds>[,abort], for checking the report: a thread of
// its own reads address 0 (or calls abort()) that many seconds after start.
//
// Stack lines carry a function name when the module has symbols DbgHelp reads
// (D012Native.pdb beside an MSVC build); otherwise module+offset, which
// llvm-symbolizer turns into a name with the same build's unstripped binary.
//
// On Linux: a fatal signal (SIGSEGV, SIGBUS, SIGILL, SIGFPE) or abort() logs
// the signal, the address, the thread and the stack, each frame as
// module+offset (D012Native, game-code/main.so, an overlay library; a name
// too when the module exports one). The unstripped programs of a release are
// kept in build/port/installer-linux/symbols/<version> (tools/port/linux_setup.sh):
//   llvm-symbolizer --obj=<symbols>/D012Native 0x<offset>
// The game's code is compiled on the player's machine and not stripped, so
// their own game-code/*.so names its frames. The signal is then raised again
// with its default action, so the system keeps a core dump as for any crash
// (SteamOS: `coredumpctl list D012Native`). Only the log is written: the log
// of a copy started from Steam or a menu is <data>/logs/D012Native.log
// (host/d012_portable.cpp). D012_TEST_CRASH works as on Windows.
//
// MIT licence; see port/README.md.

#if defined(_WIN32)

#include <windows.h>

#include <dbghelp.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#pragma comment(lib, "dbghelp.lib")

namespace {

volatile LONG g_entered = 0;

void log_stack(CONTEXT context) {
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
    SymInitialize(process, nullptr, TRUE);
    STACKFRAME64 frame{};
    frame.AddrPC.Offset = context.Rip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context.Rbp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context.Rsp;
    frame.AddrStack.Mode = AddrModeFlat;
    alignas(SYMBOL_INFO) char buffer[sizeof(SYMBOL_INFO) + 512]{};
    auto *symbol = reinterpret_cast<SYMBOL_INFO *>(buffer);
    for (int depth = 0; depth < 48; ++depth) {
        if (!StackWalk64(IMAGE_FILE_MACHINE_AMD64, process, thread, &frame, &context, nullptr,
                         SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
            break;
        const DWORD64 pc = frame.AddrPC.Offset;
        if (pc == 0) break;
        char module[MAX_PATH] = "?";
        const DWORD64 base = SymGetModuleBase64(process, pc);
        if (base != 0) {
            char path[MAX_PATH]{};
            GetModuleFileNameA(reinterpret_cast<HMODULE>(base), path, MAX_PATH);
            const char *slash = std::strrchr(path, '\\');
            std::snprintf(module, sizeof module, "%s", slash != nullptr ? slash + 1 : path);
        }
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = 511;
        DWORD64 displacement = 0;
        // Names from a PDB only: for a module without one (the llvm-mingw
        // build) DbgHelp offers the nearest export, which misleads.
        IMAGEHLP_MODULE64 info{};
        info.SizeOfStruct = sizeof info;
        const bool pdb = SymGetModuleInfo64(process, pc, &info) && info.SymType == SymPdb;
        if (pdb && SymFromAddr(process, pc, &displacement, symbol))
            std::fprintf(stderr, "  #%02d %s!%s+0x%llx (%s+0x%llx)\n", depth, module, symbol->Name,
                         static_cast<unsigned long long>(displacement), module,
                         static_cast<unsigned long long>(pc - base));
        else
            std::fprintf(stderr, "  #%02d %s+0x%llx\n", depth, module, static_cast<unsigned long long>(pc - base));
    }
}

// Where the minidump goes: D012_CRASH_DUMP's folder, else the log's.
std::string dump_path() {
    const char *dir = std::getenv("D012_CRASH_DUMP");
    if (dir != nullptr && *dir != '\0')
        return std::string(dir) + "\\crash_" + std::to_string(GetCurrentProcessId()) + ".dmp";
    const char *data = std::getenv("D012_DATA_DIR");
    if (data == nullptr || *data == '\0') return {};
    const std::string logs = std::string(data) + "\\logs";
    CreateDirectoryA(logs.c_str(), nullptr);
    return logs + "\\D012Native-crash.dmp";
}

void write_dump(EXCEPTION_POINTERS *info) {
    const std::string path = dump_path();
    if (path.empty()) return;
    HANDLE file = CreateFileA(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        std::fprintf(stderr, "[crash] cannot write the minidump %s\n", path.c_str());
        return;
    }
    MINIDUMP_EXCEPTION_INFORMATION exception{GetCurrentThreadId(), info, FALSE};
    const auto type = static_cast<MINIDUMP_TYPE>(MiniDumpWithThreadInfo | MiniDumpWithIndirectlyReferencedMemory |
                                                 MiniDumpWithUnloadedModules | MiniDumpWithHandleData);
    const BOOL written = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, type,
                                           info != nullptr ? &exception : nullptr, nullptr, nullptr);
    CloseHandle(file);
    std::fprintf(stderr, "[crash] minidump %s %s\n", path.c_str(), written ? "written" : "failed");
}

LONG WINAPI on_unhandled(EXCEPTION_POINTERS *info) {
    if (InterlockedExchange(&g_entered, 1) != 0) return EXCEPTION_CONTINUE_SEARCH;
    std::fflush(stdout);
    const EXCEPTION_RECORD &record = *info->ExceptionRecord;
    char module[MAX_PATH] = "?";
    HMODULE base = nullptr;
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           static_cast<LPCSTR>(record.ExceptionAddress), &base)) {
        char path[MAX_PATH]{};
        GetModuleFileNameA(base, path, MAX_PATH);
        const char *slash = std::strrchr(path, '\\');
        std::snprintf(module, sizeof module, "%s", slash != nullptr ? slash + 1 : path);
    }
    std::fprintf(stderr, "[crash] unhandled exception 0x%08lx at %p (%s+0x%llx) in thread %lu",
                 record.ExceptionCode, record.ExceptionAddress, module,
                 static_cast<unsigned long long>(static_cast<const char *>(record.ExceptionAddress) -
                                                 reinterpret_cast<const char *>(base)),
                 GetCurrentThreadId());
    if (record.ExceptionCode == EXCEPTION_ACCESS_VIOLATION && record.NumberParameters >= 2)
        std::fprintf(stderr, " (%s 0x%llx)", record.ExceptionInformation[0] == 0   ? "read"
                                             : record.ExceptionInformation[0] == 1 ? "write"
                                                                                   : "execute",
                     static_cast<unsigned long long>(record.ExceptionInformation[1]));
    std::fprintf(stderr, "\n");
    std::fflush(stderr);
    write_dump(info);
    log_stack(*info->ContextRecord);
    std::fflush(stderr);
    return EXCEPTION_CONTINUE_SEARCH;
}

// abort() (std::terminate, a failed assertion) never reaches the filter: log
// the stack it was called from, and a minidump, then let it end the process.
void on_abort(int) {
    if (InterlockedExchange(&g_entered, 1) != 0) return;
    std::fflush(stdout);
    std::fprintf(stderr, "[crash] abort() in thread %lu\n", GetCurrentThreadId());
    CONTEXT context{};
    RtlCaptureContext(&context);
    EXCEPTION_RECORD record{};
    record.ExceptionCode = STATUS_FATAL_APP_EXIT;
    record.ExceptionAddress = reinterpret_cast<void *>(context.Rip);
    EXCEPTION_POINTERS info{&record, &context};
    write_dump(&info);
    log_stack(context);
    std::fflush(stderr);
}

DWORD WINAPI test_crash(void *parameter) {
    const char *text = static_cast<const char *>(parameter);
    Sleep(static_cast<DWORD>(std::atof(text) * 1000.0));
    if (std::strstr(text, "abort") != nullptr) std::abort();
    return static_cast<DWORD>(*static_cast<volatile int *>(nullptr));
}

const bool g_installed = [] {
    SetUnhandledExceptionFilter(&on_unhandled);
    std::signal(SIGABRT, &on_abort);
    if (const char *test = std::getenv("D012_TEST_CRASH"); test != nullptr && *test != '\0') {
        static const std::string copy = test;
        const HANDLE thread = CreateThread(nullptr, 0, &test_crash, const_cast<char *>(copy.c_str()), 0, nullptr);
        if (thread != nullptr) CloseHandle(thread);
    }
    const char *dir = std::getenv("D012_CRASH_DUMP");
    if (dir != nullptr && *dir != '\0') {
        CreateDirectoryA(dir, nullptr);
        std::fprintf(stderr, "[crash] unhandled exceptions write a minidump to %s\n", dir);
    }
    return true;
}();

} // namespace

#elif defined(__linux__)

#include <dlfcn.h>
#include <execinfo.h>
#include <sys/syscall.h>
#include <ucontext.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <csignal>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <thread>

namespace {

volatile sig_atomic_t g_entered = 0;
// The signal handler's own stack, so a stack overflow can still be reported.
alignas(16) char g_signal_stack[64 * 1024];

// The handler formats into a buffer and writes it straight to stderr: stdio's
// locks may be held by the thread that crashed.
void say(const char *format, ...) __attribute__((format(printf, 1, 2)));
void say(const char *format, ...) {
    char line[512];
    va_list args;
    va_start(args, format);
    int length = std::vsnprintf(line, sizeof line, format, args);
    va_end(args);
    if (length <= 0) return;
    if (std::size_t(length) >= sizeof line) {  // cut short: still one line
        length = int(sizeof line) - 1;
        line[length - 1] = '\n';
    }
    (void)!write(STDERR_FILENO, line, std::size_t(length));
}

// "module+0xoffset", with the module's file name only (it is named in full once, by where()).
void describe(void *pc, char *out, std::size_t size) {
    Dl_info info{};
    if (dladdr(pc, &info) == 0 || info.dli_fbase == nullptr) {
        std::snprintf(out, size, "%p", pc);
        return;
    }
    const char *slash = info.dli_fname != nullptr ? std::strrchr(info.dli_fname, '/') : nullptr;
    const char *module = slash != nullptr ? slash + 1 : (info.dli_fname != nullptr ? info.dli_fname : "?");
    const auto offset = static_cast<unsigned long>(static_cast<char *>(pc) - static_cast<char *>(info.dli_fbase));
    if (info.dli_sname != nullptr && info.dli_saddr != nullptr)
        std::snprintf(out, size, "%s+0x%lx (%.160s+0x%lx)", module, offset, info.dli_sname,
                      static_cast<unsigned long>(static_cast<char *>(pc) - static_cast<char *>(info.dli_saddr)));
    else
        std::snprintf(out, size, "%s+0x%lx", module, offset);
}

void log_stack(void *pc) {
    void *frames[64];
    const int count = backtrace(frames, 64);
    // The frames above the one that faulted are this handler's and the
    // kernel's signal trampoline: start at the faulting address when found.
    int first = 0;
    for (int i = 0; i < count; ++i)
        if (frames[i] == pc) {
            first = i;
            break;
        }
    char line[384];
    for (int i = first, depth = 0; i < count; ++i, ++depth) {
        describe(frames[i], line, sizeof line);
        say("  #%02d %s\n", depth, line);
    }
    // Where each module of the stack is, for reading the offsets.
    const char *seen[64]{};
    int modules = 0;
    for (int i = first; i < count && modules < 64; ++i) {
        Dl_info info{};
        if (dladdr(frames[i], &info) == 0 || info.dli_fname == nullptr) continue;
        bool known = false;
        for (int m = 0; m < modules; ++m) known = known || std::strcmp(seen[m], info.dli_fname) == 0;
        if (known) continue;
        seen[modules++] = info.dli_fname;
        say("  module %s at %p\n", info.dli_fname, info.dli_fbase);
    }
}

const char *signal_name(int sig) {
    switch (sig) {
    case SIGSEGV: return "SIGSEGV, an invalid memory access";
    case SIGBUS: return "SIGBUS, a bad memory access";
    case SIGILL: return "SIGILL, an illegal instruction";
    case SIGFPE: return "SIGFPE, an arithmetic fault";
    case SIGABRT: return "SIGABRT, abort()";
    default: return "?";
    }
}

void on_signal(int sig, siginfo_t *info, void *context) {
    if (g_entered == 0) {
        g_entered = 1;
        void *pc = nullptr;
#if defined(__x86_64__)
        pc = reinterpret_cast<void *>(static_cast<ucontext_t *>(context)->uc_mcontext.gregs[REG_RIP]);
#elif defined(__aarch64__)
        pc = reinterpret_cast<void *>(static_cast<ucontext_t *>(context)->uc_mcontext.pc);
#endif
        char where[384];
        describe(pc, where, sizeof where);
        const long thread = syscall(SYS_gettid);
        if (sig == SIGABRT)
            say("[crash] abort() in thread %ld\n", thread);
        else if (sig == SIGSEGV || sig == SIGBUS)
            say("[crash] signal %d (%s) at %s in thread %ld (address %p, code %d)\n", sig, signal_name(sig), where,
                thread, info->si_addr, info->si_code);
        else
            say("[crash] signal %d (%s) at %s in thread %ld\n", sig, signal_name(sig), where, thread);
        // abort() is called, not faulted: its stack starts in this handler.
        log_stack(sig == SIGABRT ? nullptr : pc);
        say("[crash] the system may keep a core dump of it (SteamOS: coredumpctl list D012Native)\n");
    }
    // The default action now: the process ends as it would have, core dump included.
    std::signal(sig, SIG_DFL);
    raise(sig);
}

void test_crash(std::string text) {
    std::this_thread::sleep_for(std::chrono::duration<double>(std::atof(text.c_str())));
    if (text.find("abort") != std::string::npos) std::abort();
    (void)*static_cast<volatile int *>(nullptr);
}

const bool g_installed = [] {
    // backtrace() loads libgcc_s on its first call: done here, not in the handler.
    void *warm[1];
    (void)backtrace(warm, 1);
    stack_t stack{};
    stack.ss_sp = g_signal_stack;
    stack.ss_size = sizeof g_signal_stack;
    sigaltstack(&stack, nullptr);
    struct sigaction action{};
    action.sa_sigaction = &on_signal;
    action.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&action.sa_mask);
    for (const int sig : {SIGSEGV, SIGBUS, SIGILL, SIGFPE, SIGABRT}) sigaction(sig, &action, nullptr);
    if (const char *test = std::getenv("D012_TEST_CRASH"); test != nullptr && *test != '\0')
        std::thread(test_crash, std::string(test)).detach();
    return true;
}();

} // namespace

#endif
