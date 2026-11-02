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
// Windows only: DbgHelp is part of Windows. MIT licence; see port/README.md.

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

#endif
