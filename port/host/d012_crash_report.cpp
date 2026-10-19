// D012_CRASH_DUMP=<dir>: on an unhandled exception (an access violation and
// the like, in any thread, at any time including the C runtime's exit), write
// <dir>/crash_<pid>.dmp, a minidump with every thread's stack, and log the
// faulting thread's stack with symbols to stderr. Diagnosis only; nothing
// happens unless the variable is set. Windows only: DbgHelp is part of
// Windows. MIT licence; see port/README.md.

#if defined(_WIN32)

#include <windows.h>

#include <dbghelp.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#pragma comment(lib, "dbghelp.lib")

namespace {

std::string g_dump_dir;

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
        if (SymFromAddr(process, pc, &displacement, symbol))
            std::fprintf(stderr, "  #%02d %s!%s+0x%llx\n", depth, module, symbol->Name,
                         static_cast<unsigned long long>(displacement));
        else
            std::fprintf(stderr, "  #%02d %s+0x%llx\n", depth, module, static_cast<unsigned long long>(pc - base));
    }
}

LONG WINAPI on_unhandled(EXCEPTION_POINTERS *info) {
    static volatile LONG entered = 0;
    if (InterlockedExchange(&entered, 1) != 0) return EXCEPTION_CONTINUE_SEARCH;
    const EXCEPTION_RECORD &record = *info->ExceptionRecord;
    std::fprintf(stderr, "[crash] unhandled exception 0x%08lx at %p in thread %lu", record.ExceptionCode,
                 record.ExceptionAddress, GetCurrentThreadId());
    if (record.ExceptionCode == EXCEPTION_ACCESS_VIOLATION && record.NumberParameters >= 2)
        std::fprintf(stderr, " (%s 0x%llx)", record.ExceptionInformation[0] == 0   ? "read"
                                             : record.ExceptionInformation[0] == 1 ? "write"
                                                                                   : "execute",
                     static_cast<unsigned long long>(record.ExceptionInformation[1]));
    std::fprintf(stderr, "\n");
    std::fflush(stderr);
    const std::string path = g_dump_dir + "\\crash_" + std::to_string(GetCurrentProcessId()) + ".dmp";
    HANDLE file = CreateFileA(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION exception{GetCurrentThreadId(), info, FALSE};
        const auto type = static_cast<MINIDUMP_TYPE>(MiniDumpWithThreadInfo | MiniDumpWithIndirectlyReferencedMemory |
                                                     MiniDumpWithUnloadedModules | MiniDumpWithHandleData);
        const BOOL written =
            MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, type, &exception, nullptr, nullptr);
        CloseHandle(file);
        std::fprintf(stderr, "[crash] minidump %s %s\n", path.c_str(), written ? "written" : "failed");
    }
    log_stack(*info->ContextRecord);
    std::fflush(stderr);
    return EXCEPTION_CONTINUE_SEARCH;
}

const bool g_installed = [] {
    const char *dir = std::getenv("D012_CRASH_DUMP");
    if (dir == nullptr || *dir == '\0') return false;
    g_dump_dir = dir;
    CreateDirectoryA(dir, nullptr);
    SetUnhandledExceptionFilter(&on_unhandled);
    std::fprintf(stderr, "[crash] unhandled exceptions write a minidump to %s\n", dir);
    return true;
}();

} // namespace

#endif
