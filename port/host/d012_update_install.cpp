// Installing a newer version from the menu (d012_update_install.hpp).
//
// Only when this build carries the release key's public half
// (d012::kUpdateSigningKey), and only in a copy the Windows setup installed
// (unins000.exe beside the program), a copy the Linux setup installed
// (setup.ini beside the program, in a folder it can write) or the player's
// Android app (the installer build, D012_INSTALLER_BUILD). Elsewhere the System
// page keeps "Open the download page": RecompLauncher updates its own copies.
//
// The steps, on a thread of their own while the game goes on:
//  1. SHA256SUMS and SHA256SUMS.sig are fetched, and the list is refused unless
//     the signature is the release key's. The list names the program with its
//     version, so an older release's list cannot pass for this one.
//  2. The program (the Windows setup, the Android app, the Linux AppImage) is
//     downloaded to update-download/, as <name>.part, which a later attempt
//     continues: beside the program on Windows and Linux, in the app's own
//     storage on Android.
//  3. Its SHA-256 must be the one the signed list gives. Only then does it get
//     its own name, and only a file with its own name is ever installed.
//
// Windows: when the player chooses to install, update-download/pending.txt is
// written with the version, the setup starts, and the game closes. The setup
// (tools/port/installer/dissidia012.iss, /UPDATE=1) waits for the game to end,
// replaces the program, compiles the game's code when it must, deletes
// pending.txt and starts the game. A run that finds pending.txt naming its own
// version knows the setup stopped between replacing the program and finishing
// the game's code, and offers to finish; naming another version, the program
// was never replaced and nothing needs doing.
//
// Android: the app goes to Android's package installer (PortableKit's
// android::install_package), which asks the player to confirm, stops the game
// and installs the app over itself, keeping its data; Android also checks that
// it is signed with the same key. The player opens the game again, which
// compiles the game's code first only when the new version's differs
// (host/d012_device_compile.cpp). The next start deletes the download.
//
// Linux: the AppImage unpacks itself (--appimage-extract, which needs no FUSE)
// into update-download/squashfs-root/, and its VERSION must be the release's.
// When the player chooses to install, each of the program's files and folders
// (the AppImage's payload/, what the setup copies: tools/port/linux_setup/AppRun)
// is moved into update-old/ and the new one into its place. Linux lets a
// running program be moved, never written over. setup.ini gets the new version,
// and the game quits and starts again (PortableKit's restart on exit, in its
// install/installer.hpp: under Steam, Steam starts it again once it has ended,
// the one way that worked in the Steam Deck's Gaming Mode; elsewhere it
// starts the new program as it exits). The new program finds update-old/ (prepare_after_update): it
// compiles the game's code on the setup's screens when the new version's
// differs, then deletes update-old/ and the download.
//
// Tests: D012_UPDATE_ASSETS=<folder> takes those of the three files that are in
// the folder from there instead of GitHub; every check is made all the same.
// (On Android, test-environment.txt sets it: host/d012_portable.cpp.)
//
// MIT licence; see port/README.md.

#include "d012_update_install.hpp"

#include "d012_updates.hpp"
#include "d012_version.hpp"

#include "app_paths.hpp"
#include "hle/hle_common.hpp"
#include "psprecomp/sha256.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "ui/ui.hpp"
#endif

#if defined(_WIN32)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <windows.h>
#include <winhttp.h>
#elif defined(PORTABLEKIT_ANDROID_APP) && defined(D012_INSTALLER_BUILD)
#include "install/user_data.hpp"
#include "platform/android_jni.hpp"

#include <SDL3/SDL_system.h>
#include <jni.h>
#elif defined(__linux__) && defined(D012_INSTALLER_BUILD)
#include "d012_curl.hpp"

#include "install/installer.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include <SDL3/SDL.h>
#endif

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <functional>
#include <memory>
#endif

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

// Where a copy can install an update itself.
#if defined(_WIN32)
#define D012_INSTALLS_UPDATES 1
#define D012_INSTALLS_ON_WINDOWS 1
#elif defined(PORTABLEKIT_ANDROID_APP) && defined(D012_INSTALLER_BUILD)
#define D012_INSTALLS_UPDATES 1
#define D012_INSTALLS_ON_ANDROID 1
#elif defined(__linux__) && defined(D012_INSTALLER_BUILD)
#define D012_INSTALLS_UPDATES 1
#define D012_INSTALLS_ON_LINUX 1
#endif

#if defined(D012_INSTALLS_ON_LINUX)
namespace d012::compile_game {
// host/d012_compile_game.cpp: --compile-game's steps in this process.
int run_here(bool check_only, std::function<void(const std::string &, unsigned)> listener);
}
#endif

namespace d012::update_install {

namespace rules = update_rules;

namespace {

#if defined(D012_INSTALLS_UPDATES)

namespace fs = std::filesystem;

#if defined(D012_INSTALLS_ON_WINDOWS)
// Room for the setup, what it unpacks and the game's code it may compile.
constexpr std::uint64_t kSpaceBesidesProgram = 1500ull * 1024u * 1024u;
// The setups' names: rules::setup_file_name.
constexpr std::string_view kProgramPrefix = "Dissidia012-PC-Setup-";
constexpr std::string_view kProgramSuffix = ".exe";
constexpr const char *kPendingFile = "pending.txt";
#elif defined(D012_INSTALLS_ON_ANDROID)
// Room for Android's copy of the app, its libraries unpacked, and the game's
// code the app may compile again.
constexpr std::uint64_t kSpaceBesidesProgram = 1024ull * 1024u * 1024u;
// The apps' names: rules::android_file_name.
constexpr std::string_view kProgramPrefix = "Dissidia012-";
constexpr std::string_view kProgramSuffix = "-Android-arm64.apk";
#else
// Room for the AppImage's files unpacked (about 550 MB) and the game's code it
// may compile.
constexpr std::uint64_t kSpaceBesidesProgram = 1500ull * 1024u * 1024u;
// The AppImages' names: rules::linux_file_name.
constexpr std::string_view kProgramPrefix = "Dissidia012-Setup-";
constexpr std::string_view kProgramSuffix = "-x86_64.AppImage";
// In update-download/: what the AppImage unpacks to, and its output.
constexpr const char *kUnpacked = "squashfs-root";
constexpr const char *kUnpackLog = "unpack.log";
// Beside the program: the files an update replaced, until the new version
// has started.
constexpr const char *kOldFiles = "update-old";
#endif

std::mutex g_mutex;
Status g_status;
fs::path g_program;  // Ready: the checked download
std::atomic<bool> g_busy{false};
std::atomic<bool> g_cancel{false};

struct Cancelled {};
struct Problem {
    std::string text;
};

#if defined(D012_INSTALLS_ON_ANDROID)
fs::path folder() { return portablekit::install::user_data_directory() / "update-download"; }
#else
// The program's folder as it was at the start (PortableKit remembers it), so
// an update that moved the program aside does not move this.
fs::path folder() { return portablekit::executable_directory() / "update-download"; }
#endif

std::optional<rules::UpdateFiles> update_files(const rules::Release &release) {
#if defined(D012_INSTALLS_ON_WINDOWS)
    return rules::windows_update(release);
#elif defined(D012_INSTALLS_ON_ANDROID)
    return rules::android_update(release);
#else
    return rules::linux_update(release);
#endif
}

void set_step(Step step) {
    std::lock_guard lock(g_mutex);
    g_status.step = step;
}

// --- Downloading -----------------------------------------------------------

#if defined(D012_INSTALLS_ON_WINDOWS)

// Asked every frame the System page shows an update, and neither can change.
bool running_under_wine() {
    static const bool wine = [] {
        const HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
        return ntdll != nullptr && GetProcAddress(ntdll, "wine_get_version") != nullptr;
    }();
    return wine;
}

bool installed_by_the_setup() {
    static const bool installed = [] {
        std::error_code ec;
        return fs::is_regular_file(portablekit::executable_directory() / "unins000.exe", ec);
    }();
    return installed;
}

std::wstring wide(const std::string &text) { return std::wstring(text.begin(), text.end()); }  // ASCII addresses

// `url` into `part`, continuing what is there, with `size` bytes expected.
void download(const std::string &url, const fs::path &part, std::uint64_t size) {
    wchar_t host[256] = {};
    wchar_t path[2048] = {};
    URL_COMPONENTS parts{};
    parts.dwStructSize = sizeof parts;
    parts.lpszHostName = host;
    parts.dwHostNameLength = static_cast<DWORD>(std::size(host));
    parts.lpszUrlPath = path;
    parts.dwUrlPathLength = static_cast<DWORD>(std::size(path));
    const std::wstring address = wide(url);
    if (!WinHttpCrackUrl(address.c_str(), 0, 0, &parts) || parts.nScheme != INTERNET_SCHEME_HTTPS)
        throw Problem{"the download's address cannot be used"};

    std::error_code ec;
    std::uint64_t have = fs::is_regular_file(part, ec) ? fs::file_size(part, ec) : 0u;
    if (ec || have > size) {
        fs::remove(part, ec);
        have = 0;
    }

    const std::wstring agent = L"Dissidia012-pc/" + wide(kVersion);
    HINTERNET session = WinHttpOpen(agent.c_str(), WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME,
                                    WINHTTP_NO_PROXY_BYPASS, 0);
    HINTERNET connection = nullptr;
    HINTERNET request = nullptr;
    const auto close = [&] {
        if (request != nullptr) WinHttpCloseHandle(request);
        if (connection != nullptr) WinHttpCloseHandle(connection);
        if (session != nullptr) WinHttpCloseHandle(session);
    };
    const auto lost = [&](const char *step) {
        std::cout << "[update] " << step << " failed: WinHTTP error " << GetLastError() << std::endl;
        close();
        throw Problem{"the download was interrupted. Try again to continue it"};
    };
    if (session == nullptr) lost("WinHttpOpen");
    WinHttpSetTimeouts(session, 10000, 15000, 15000, 30000);
    connection = WinHttpConnect(session, host, parts.nPort, 0);
    if (connection == nullptr) lost("WinHttpConnect");
    request = WinHttpOpenRequest(connection, L"GET", path, nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
                                 WINHTTP_FLAG_SECURE);
    if (request == nullptr) lost("WinHttpOpenRequest");
    const std::wstring range = have > 0 ? L"Range: bytes=" + std::to_wstring(have) + L"-\r\n" : std::wstring();
    if (!WinHttpSendRequest(request, have > 0 ? range.c_str() : WINHTTP_NO_ADDITIONAL_HEADERS,
                            have > 0 ? static_cast<DWORD>(-1) : 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
        !WinHttpReceiveResponse(request, nullptr))
        lost("the request");

    DWORD status = 0;
    DWORD length = sizeof status;
    WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
                        &status, &length, WINHTTP_NO_HEADER_INDEX);
    if (have > 0 && (status == 416 || status == 200)) {
        // GitHub will not continue this one: from the start.
        close();
        fs::remove(part, ec);
        download(url, part, size);
        return;
    }
    if (status != 200 && status != 206) {
        close();
        throw Problem{"GitHub answered with error " + std::to_string(status)};
    }
    if (have > 0) std::cout << "[update] continuing the download from byte " << have << std::endl;

    std::ofstream out(part, std::ios::binary | std::ios::app);
    if (!out) {
        close();
        throw Problem{"the download cannot be written beside the game"};
    }
    std::vector<char> buffer(256u * 1024u);
    for (;;) {
        if (g_cancel.load()) {
            close();
            throw Cancelled{};
        }
        DWORD read = 0;
        if (!WinHttpReadData(request, buffer.data(), static_cast<DWORD>(buffer.size()), &read)) lost("reading");
        if (read == 0) break;
        have += read;
        if (have > size) {
            close();
            out.close();
            fs::remove(part, ec);
            throw Problem{"the download is larger than the release says"};
        }
        out.write(buffer.data(), read);
        if (!out) {
            close();
            throw Problem{"the download cannot be written beside the game: check the free space"};
        }
        std::lock_guard lock(g_mutex);
        g_status.done = have;
    }
    close();
    out.close();
    if (have != size) throw Problem{"the download was interrupted. Try again to continue it"};
}

#elif defined(D012_INSTALLS_ON_ANDROID)

// `url` into `part` through Java's HttpURLConnection, continuing what is
// there, with `size` bytes expected. It follows GitHub's redirect to its
// download server by itself (https to https).
void download(const std::string &url, const fs::path &part, std::uint64_t size) {
    if (url.rfind("https://", 0) != 0) throw Problem{"the download's address cannot be used"};
    std::error_code ec;
    std::uint64_t have = fs::is_regular_file(part, ec) ? fs::file_size(part, ec) : 0u;
    if (ec || have > size) {
        fs::remove(part, ec);
        have = 0;
    }

    auto *jni = static_cast<JNIEnv *>(SDL_GetAndroidJNIEnv());
    if (jni == nullptr || jni->PushLocalFrame(32) != JNI_OK) {
        std::cout << "[update] no Java environment: " << SDL_GetError() << std::endl;
        throw Problem{"the download could not start"};
    }
    struct Frame {
        JNIEnv *jni;
        ~Frame() { jni->PopLocalFrame(nullptr); }
    } frame{jni};
    // A Java exception ends the attempt; its text goes to the log.
    const auto thrown = [&](const char *step) {
        if (!jni->ExceptionCheck()) return false;
        jthrowable error = jni->ExceptionOccurred();
        jni->ExceptionClear();
        std::string text = "?";
        if (jclass object = jni->FindClass("java/lang/Object"); object != nullptr) {
            const jmethodID to_string = jni->GetMethodID(object, "toString", "()Ljava/lang/String;");
            if (auto *s = static_cast<jstring>(jni->CallObjectMethod(error, to_string)); s != nullptr) {
                if (const char *chars = jni->GetStringUTFChars(s, nullptr); chars != nullptr) {
                    text = chars;
                    jni->ReleaseStringUTFChars(s, chars);
                }
            }
        }
        jni->ExceptionClear();
        std::cout << "[update] " << step << ": " << text << std::endl;
        return true;
    };
    const auto lost = [](const char *) -> Problem { return Problem{"the download was interrupted. Try again to continue it"}; };

    jclass url_class = jni->FindClass("java/net/URL");
    jclass http = jni->FindClass("java/net/HttpURLConnection");
    jclass stream_class = jni->FindClass("java/io/InputStream");
    if (thrown("classes") || url_class == nullptr || http == nullptr || stream_class == nullptr)
        throw Problem{"the download could not start"};
    jobject address = jni->NewObject(url_class, jni->GetMethodID(url_class, "<init>", "(Ljava/lang/String;)V"),
                                     jni->NewStringUTF(url.c_str()));
    if (thrown("URL")) throw Problem{"the download's address cannot be used"};
    jobject connection =
        jni->CallObjectMethod(address, jni->GetMethodID(url_class, "openConnection", "()Ljava/net/URLConnection;"));
    if (thrown("openConnection") || connection == nullptr || !jni->IsInstanceOf(connection, http))
        throw lost("openConnection");
    const jmethodID disconnect_method = jni->GetMethodID(http, "disconnect", "()V");
    const auto disconnect = [&] {
        jni->CallVoidMethod(connection, disconnect_method);
        (void)thrown("disconnect");
    };
    const jmethodID property = jni->GetMethodID(http, "setRequestProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
    const std::string agent = std::string("Dissidia012-pc/") + kVersion;
    jni->CallVoidMethod(connection, property, jni->NewStringUTF("User-Agent"), jni->NewStringUTF(agent.c_str()));
    if (have > 0) {
        const std::string range = "bytes=" + std::to_string(have) + "-";
        jni->CallVoidMethod(connection, property, jni->NewStringUTF("Range"), jni->NewStringUTF(range.c_str()));
    }
    jni->CallVoidMethod(connection, jni->GetMethodID(http, "setConnectTimeout", "(I)V"), 15000);
    jni->CallVoidMethod(connection, jni->GetMethodID(http, "setReadTimeout", "(I)V"), 30000);
    const jint status = jni->CallIntMethod(connection, jni->GetMethodID(http, "getResponseCode", "()I"));
    if (thrown("getResponseCode")) {
        disconnect();
        throw lost("getResponseCode");
    }
    if (have > 0 && (status == 416 || status == 200)) {
        // GitHub will not continue this one: from the start.
        disconnect();
        fs::remove(part, ec);
        download(url, part, size);
        return;
    }
    if (status != 200 && status != 206) {
        disconnect();
        throw Problem{"GitHub answered with error " + std::to_string(status)};
    }
    if (have > 0) std::cout << "[update] continuing the download from byte " << have << std::endl;

    jobject stream = jni->CallObjectMethod(connection, jni->GetMethodID(http, "getInputStream", "()Ljava/io/InputStream;"));
    if (thrown("getInputStream") || stream == nullptr) {
        disconnect();
        throw lost("getInputStream");
    }
    const jmethodID read_method = jni->GetMethodID(stream_class, "read", "([B)I");
    const jmethodID close_method = jni->GetMethodID(stream_class, "close", "()V");
    const auto close = [&] {
        jni->CallVoidMethod(stream, close_method);
        (void)thrown("close");
        disconnect();
    };
    std::ofstream out(part, std::ios::binary | std::ios::app);
    if (!out) {
        close();
        throw Problem{"the download cannot be written to the app's storage"};
    }
    constexpr jsize kChunk = 256 * 1024;
    jbyteArray buffer = jni->NewByteArray(kChunk);
    std::vector<char> bytes(static_cast<std::size_t>(kChunk));
    for (;;) {
        if (g_cancel.load()) {
            close();
            throw Cancelled{};
        }
        const jint got = jni->CallIntMethod(stream, read_method, buffer);
        if (thrown("reading")) {
            close();
            throw lost("reading");
        }
        if (got < 0) break;
        if (got == 0) continue;
        jni->GetByteArrayRegion(buffer, 0, got, reinterpret_cast<jbyte *>(bytes.data()));
        have += static_cast<std::uint64_t>(got);
        if (have > size) {
            close();
            out.close();
            fs::remove(part, ec);
            throw Problem{"the download is larger than the release says"};
        }
        out.write(bytes.data(), got);
        if (!out) {
            close();
            throw Problem{"the download cannot be written to the app's storage: check the free space"};
        }
        std::lock_guard lock(g_mutex);
        g_status.done = have;
    }
    close();
    out.close();
    if (have != size) throw lost("the end");
}

#else  // Linux

// A copy the Linux setup installed: it writes setup.ini beside the program
// (tools/port/linux_setup/AppRun), and the folder must take the new files.
bool installed_by_the_setup() {
    static const bool installed = [] {
        std::error_code ec;
        const fs::path dir = portablekit::executable_directory();
        return !dir.empty() && fs::is_regular_file(dir / "setup.ini", ec) && access(dir.c_str(), W_OK) == 0;
    }();
    return installed;
}

// What the download's callback keeps.
struct Transfer {
    std::FILE *out = nullptr;
    void *handle = nullptr;
    const curl::Api *c = nullptr;
    std::uint64_t have = 0;
    std::uint64_t size = 0;
    bool answered = false;   // the first bytes have come, and the answer's status is known
    bool cancelled = false;
    bool too_large = false;
    bool unwritable = false;
};

std::size_t take(char *data, std::size_t size, std::size_t count, void *to) {
    Transfer &t = *static_cast<Transfer *>(to);
    const std::size_t bytes = size * count;
    if (g_cancel.load()) {
        t.cancelled = true;
        return 0;
    }
    if (!t.answered) {
        t.answered = true;
        long status = 0;
        t.c->easy_getinfo(t.handle, curl::kInfoResponseCode, &status);
        if (t.have > 0 && status == 200) {
            // GitHub sent the whole file, not the rest: from the start.
            std::fflush(t.out);
            if (ftruncate(fileno(t.out), 0) != 0) {
                t.unwritable = true;
                return 0;
            }
            t.have = 0;
        }
    }
    if (t.have + bytes > t.size) {
        t.too_large = true;
        return 0;
    }
    if (std::fwrite(data, 1, bytes, t.out) != bytes) {
        t.unwritable = true;
        return 0;
    }
    t.have += bytes;
    std::lock_guard lock(g_mutex);
    g_status.done = t.have;
    return bytes;
}

// `url` into `part` through the system's libcurl, continuing what is there,
// with `size` bytes expected. It follows GitHub's redirect to its download
// server, and only to https.
void download(const std::string &url, const fs::path &part, std::uint64_t size) {
    const curl::Api *c = curl::api();
    if (c == nullptr) throw Problem{"this system has no libcurl, which the download needs"};
    if (url.rfind("https://", 0) != 0) throw Problem{"the download's address cannot be used"};
    std::error_code ec;
    std::uint64_t have = fs::is_regular_file(part, ec) ? fs::file_size(part, ec) : 0u;
    if (ec || have > size) {
        fs::remove(part, ec);
        have = 0;
    }
    if (have > 0) std::cout << "[update] continuing the download from byte " << have << std::endl;

    Transfer t;
    t.out = std::fopen(part.c_str(), "ab");
    if (t.out == nullptr) throw Problem{"the download cannot be written beside the game"};
    t.handle = c->easy_init();
    if (t.handle == nullptr) {
        std::fclose(t.out);
        throw Problem{"the download could not start"};
    }
    t.c = c;
    t.have = have;
    t.size = size;
    const std::string agent = std::string("Dissidia012-pc/") + kVersion;
    void *h = t.handle;
    c->easy_setopt(h, curl::kOptUrl, url.c_str());
    c->easy_setopt(h, curl::kOptUserAgent, agent.c_str());
    c->easy_setopt(h, curl::kOptWriteFunction, &take);
    c->easy_setopt(h, curl::kOptWriteData, static_cast<void *>(&t));
    c->easy_setopt(h, curl::kOptFollowLocation, 1L);
    c->easy_setopt(h, curl::kOptProtocols, curl::kProtocolHttps);
    c->easy_setopt(h, curl::kOptRedirectProtocols, curl::kProtocolHttps);
    c->easy_setopt(h, curl::kOptFailOnError, 1L);
    c->easy_setopt(h, curl::kOptConnectTimeout, 15L);
    // No time limit on the whole: given up only when nothing comes for 30 s.
    c->easy_setopt(h, curl::kOptLowSpeedLimit, 1L);
    c->easy_setopt(h, curl::kOptLowSpeedTime, 30L);
    c->easy_setopt(h, curl::kOptNoSignal, 1L);
    if (have > 0) c->easy_setopt(h, curl::kOptResumeFromLarge, static_cast<long long>(have));
    const int code = c->easy_perform(h);
    long status = 0;
    c->easy_getinfo(h, curl::kInfoResponseCode, &status);
    if (code != 0) std::cout << "[update] libcurl: " << c->easy_strerror(code) << " (HTTP " << status << ")" << std::endl;
    c->easy_cleanup(h);
    const bool closed = std::fclose(t.out) == 0;
    if (t.cancelled) throw Cancelled{};
    if (t.too_large) {
        fs::remove(part, ec);
        throw Problem{"the download is larger than the release says"};
    }
    if (t.unwritable || !closed) throw Problem{"the download cannot be written beside the game: check the free space"};
    if (code != 0 && have > 0 && status == 416) {
        // GitHub will not continue this one: from the start.
        fs::remove(part, ec);
        download(url, part, size);
        return;
    }
    if (code != 0 && status >= 400) throw Problem{"GitHub answered with error " + std::to_string(status)};
    if (code != 0 || t.have != size) throw Problem{"the download was interrupted. Try again to continue it"};
}

// The AppImage's files, unpacked by the AppImage itself into
// update-download/squashfs-root/: payload/ holds what the setup installs, and
// VERSION must name the release.
void unpack(const fs::path &appimage, const std::string &version) {
    set_step(Step::Unpacking);
    std::error_code ec;
    const fs::path dir = appimage.parent_path();
    fs::remove_all(dir / kUnpacked, ec);
    fs::permissions(appimage, fs::perms::owner_exec, fs::perm_options::add, ec);
    const int log = open((dir / kUnpackLog).c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
    const int null = open("/dev/null", O_RDWR | O_CLOEXEC);
    const pid_t child = fork();
    if (child == 0) {
        // Only what is safe between fork and exec in a program with threads.
        if (null >= 0) {
            dup2(null, STDIN_FILENO);
            dup2(null, STDOUT_FILENO);  // a line for every file
        }
        if (log >= 0) dup2(log, STDERR_FILENO);
        if (chdir(dir.c_str()) != 0) _exit(126);
        const char *argv[] = {appimage.c_str(), "--appimage-extract", nullptr};
        execv(appimage.c_str(), const_cast<char *const *>(argv));
        _exit(127);
    }
    if (log >= 0) close(log);
    if (null >= 0) close(null);
    if (child < 0) throw Problem{"the update could not be unpacked"};
    int status = 0;
    while (waitpid(child, &status, 0) < 0 && errno == EINTR) {
    }
    const fs::path unpacked = dir / kUnpacked;
    std::ifstream in(unpacked / "VERSION");
    std::string found;
    std::getline(in, found);
    while (!found.empty() && (found.back() == '\r' || found.back() == ' ')) found.pop_back();
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0 || !fs::is_regular_file(unpacked / "payload" / "D012Native", ec)) {
        std::cout << "[update] the AppImage did not unpack (status " << status << "); see " << (dir / kUnpackLog).string()
                  << std::endl;
        throw Problem{"the update could not be unpacked: check the free space"};
    }
    if (found != version) {
        std::cout << "[update] the AppImage says it is version " << found << ", not " << version << std::endl;
        throw Problem{"the download is not version " + version};
    }
}

#endif

// D012_UPDATE_ASSETS's copy of a file of the release, when it has one.
std::optional<fs::path> test_copy(const rules::Asset &asset) {
    const char *dir = std::getenv("D012_UPDATE_ASSETS");
    if (dir == nullptr || *dir == '\0') return std::nullopt;
    std::error_code ec;
    fs::path file = fs::path(dir) / asset.name;
    if (!fs::is_regular_file(file, ec)) return std::nullopt;
    return file;
}

// One of the two small files.
std::string fetch_small(const rules::Asset &asset) {
    if (const std::optional<fs::path> file = test_copy(asset)) {
        std::ifstream in(*file, std::ios::binary);
        return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    }
    const updates::Fetched answer = updates::get(asset.url);
    if (!answer.problem.empty()) throw Problem{answer.problem};
    if (answer.status != 200) throw Problem{"GitHub answered with error " + std::to_string(answer.status)};
    return answer.body;
}

// --- The steps -------------------------------------------------------------

// The checked download, ready to install.
void ready(const fs::path &program, [[maybe_unused]] const std::string &version) {
#if defined(D012_INSTALLS_ON_LINUX)
    unpack(program, version);
#endif
    std::lock_guard lock(g_mutex);
    g_program = program;
    g_status.step = Step::Ready;
}

void work(const rules::Release &release) {
    const std::optional<rules::UpdateFiles> update = update_files(release);
    if (!update) throw Problem{"this release cannot be installed from here"};

    const std::string sums = fetch_small(update->sums);
    const std::string signature = fetch_small(update->signature);
    if (!rules::sums_signed(sums, signature, kUpdateSigningKey))
        throw Problem{"the release's signature is not valid, so nothing was installed"};
    const std::string expected = rules::listed_hash(sums, update->program.name);
    if (expected.empty()) throw Problem{"the release's signed list does not name this download"};

    std::error_code ec;
    const fs::path dir = folder();
    fs::create_directories(dir, ec);
    if (ec) throw Problem{"the folder for the download cannot be made"};
    const fs::path program = dir / update->program.name;
    const fs::path part = fs::path(program).concat(".part");
    // Downloads of other versions go, and what was unpacked from any.
    for (const fs::directory_entry &entry : fs::directory_iterator(dir, ec))
        if (entry.path() != program && entry.path() != part) fs::remove_all(entry.path(), ec);

    const auto is_good = [&](const fs::path &file) { return psprecomp::sha256_file(file) == expected; };
    if (fs::is_regular_file(program, ec)) {
        set_step(Step::Checking);
        if (is_good(program)) {
            ready(program, release.version);
            return;
        }
        fs::remove(program, ec);
    }

    const fs::space_info space = fs::space(dir, ec);
    if (!ec && space.available < update->program.size + kSpaceBesidesProgram)
        throw Problem{"there is not enough free space on this drive"};

    {
        std::lock_guard lock(g_mutex);
        g_status.step = Step::Downloading;
        g_status.total = update->program.size;
        g_status.done = fs::is_regular_file(part, ec) ? fs::file_size(part, ec) : 0u;
    }
    if (const std::optional<fs::path> file = test_copy(update->program)) {
        fs::copy_file(*file, part, fs::copy_options::overwrite_existing, ec);
        if (ec) throw Problem{"the test folder's copy cannot be copied"};
    } else {
        download(update->program.url, part, update->program.size);
    }

    set_step(Step::Checking);
    if (!is_good(part)) {
        fs::remove(part, ec);
        throw Problem{"the download is damaged, so nothing was installed. Try again"};
    }
    fs::rename(part, program, ec);
    if (ec) throw Problem{"the download cannot be kept"};
    ready(program, release.version);
}

// Downloads of this version or older have served their turn, and so has what
// was unpacked from them.
void clear_finished_downloads() {
    std::error_code ec;
    const fs::path dir = folder();
    if (!fs::is_directory(dir, ec)) return;
#if defined(D012_INSTALLS_ON_LINUX)
    fs::remove_all(dir / kUnpacked, ec);
    fs::remove(dir / kUnpackLog, ec);
#endif
    for (const fs::directory_entry &entry : fs::directory_iterator(dir, ec)) {
        std::string name = entry.path().filename().string();
        if (name.size() > 5 && name.ends_with(".part")) name.resize(name.size() - 5);
        if (name.size() <= kProgramPrefix.size() + kProgramSuffix.size() || !name.starts_with(kProgramPrefix) ||
            !name.ends_with(kProgramSuffix))
            continue;
        const std::string version =
            name.substr(kProgramPrefix.size(), name.size() - kProgramPrefix.size() - kProgramSuffix.size());
        if (rules::is_version(version) && rules::compare_versions(version, kVersion) <= 0)
            fs::remove(entry.path(), ec);
    }
    fs::remove(dir, ec);  // only when empty
}

// --- Installing ------------------------------------------------------------

#if defined(D012_INSTALLS_ON_WINDOWS)

// The environment without the port's own switches, which are this run's only.
std::vector<wchar_t> clean_environment() {
    std::vector<wchar_t> out;
    wchar_t *block = GetEnvironmentStringsW();
    if (block == nullptr) return out;
    for (const wchar_t *at = block; *at != L'\0';) {
        const std::size_t size = wcslen(at);
        if (_wcsnicmp(at, L"D012_", 5) != 0) out.insert(out.end(), at, at + size + 1);
        at += size + 1;
    }
    FreeEnvironmentStringsW(block);
    out.push_back(L'\0');
    return out;
}

// The setup, as a process that outlives this one. `silent`: it asks nothing
// and starts the game when it is done; otherwise the player sees its wizard.
bool start_setup(const fs::path &setup, bool silent) {
    const fs::path program_dir = portablekit::executable_directory();
    std::wstring command = L"\"" + setup.wstring() + L"\" /WAITPID=" + std::to_wstring(GetCurrentProcessId()) +
                           L" /DIR=\"" + program_dir.wstring() + L"\" /LOG=\"" +
                           (program_dir / "setup-update.log").wstring() + L"\"";
    if (silent)
        command += L" /UPDATE=1 /SILENT /SUPPRESSMSGBOXES /NORESTART /CLOSEAPPLICATIONS /NORESTARTAPPLICATIONS";
    std::vector<wchar_t> mutable_command(command.begin(), command.end());
    mutable_command.push_back(L'\0');
    std::vector<wchar_t> environment = clean_environment();
    STARTUPINFOW startup{};
    startup.cb = sizeof startup;
    PROCESS_INFORMATION process{};
    const std::wstring directory = program_dir.wstring();
    const auto launch = [&](DWORD flags) {
        return CreateProcessW(setup.c_str(), mutable_command.data(), nullptr, nullptr, FALSE,
                              flags | CREATE_UNICODE_ENVIRONMENT, environment.empty() ? nullptr : environment.data(),
                              directory.c_str(), &startup, &process) != FALSE;
    };
    // Out of a job a launcher may hold the game in, which would end the setup
    // with the game; where that is not allowed, as it is.
    if (!launch(CREATE_BREAKAWAY_FROM_JOB) && !launch(0)) {
        std::cout << "[update] the setup could not be started: error " << GetLastError() << std::endl;
        return false;
    }
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return true;
}

void write_pending(const std::string &version) {
    std::ofstream out(folder() / kPendingFile, std::ios::binary | std::ios::trunc);
    out << version << "\n";
}

std::string read_pending() {
    std::ifstream in(folder() / kPendingFile, std::ios::binary);
    std::string version;
    std::getline(in, version);
    while (!version.empty() && (version.back() == '\r' || version.back() == ' ')) version.pop_back();
    return version;
}

#elif defined(D012_INSTALLS_ON_ANDROID)

// Set once Android's installer has the app, so that its answer about an
// earlier attempt is never taken for this one's.
std::atomic<bool> g_asked{false};
std::atomic<bool> g_handing{false};

// Android's refusal, for the player (PackageInstaller's STATUS_FAILURE_*).
std::string refusal(int status) {
    switch (status) {
    case 2: return "Android blocked the install";
    case 4: return "Android says the download is not a valid app";
    case 5: return "Android refused it: this copy of the app was signed with another key. Install the update from its "
                   "page";
    case 6: return "there is not enough free space on this device";
    case 7: return "Android says this update does not suit this device";
    default: return "Android could not install it";
    }
}

#else  // Linux

// The new program's files in place of this one's: each of payload/'s files
// and folders moves the old one into update-old/ and itself into the program's
// folder. Renames on one drive, so all or none: what was moved goes back when
// one fails.
void swap_in(const std::string &version) {
    std::error_code ec;
    const fs::path dir = portablekit::executable_directory();
    const fs::path payload = folder() / kUnpacked / "payload";
    const fs::path old = dir / kOldFiles;
    fs::remove_all(old, ec);
    fs::create_directory(old, ec);
    if (ec) throw Problem{"the folder for the old files cannot be made"};
    std::vector<std::string> names;
    for (const fs::directory_entry &entry : fs::directory_iterator(payload, ec)) names.push_back(entry.path().filename().string());
    if (ec || std::find(names.begin(), names.end(), "D012Native") == names.end())
        throw Problem{"the unpacked update is incomplete"};

    std::vector<std::string> set_aside;
    std::vector<std::string> put_in;
    const auto undo = [&] {
        std::error_code ignored;
        for (auto name = put_in.rbegin(); name != put_in.rend(); ++name) fs::rename(dir / *name, payload / *name, ignored);
        for (auto name = set_aside.rbegin(); name != set_aside.rend(); ++name)
            fs::rename(old / *name, dir / *name, ignored);
    };
    for (const std::string &name : names) {
        if (!fs::exists(fs::symlink_status(dir / name, ec))) continue;
        fs::rename(dir / name, old / name, ec);
        if (ec) {
            std::cout << "[update] cannot move " << name << " aside: " << ec.message() << std::endl;
            undo();
            throw Problem{"the old files could not be moved aside, so nothing was installed"};
        }
        set_aside.push_back(name);
    }
    for (const std::string &name : names) {
        fs::rename(payload / name, dir / name, ec);
        if (ec) {
            std::cout << "[update] cannot put " << name << " in place: " << ec.message() << std::endl;
            undo();
            throw Problem{"the new files could not be put in place, so nothing was installed"};
        }
        put_in.push_back(name);
    }

    // setup.ini's version, as the setup writes it (iso= and copy_iso= stay).
    std::ifstream in(dir / "setup.ini");
    std::string text;
    for (std::string line; std::getline(in, line);)
        if (!line.starts_with("version=")) text += line + "\n";
    in.close();
    std::ofstream out(dir / "setup.ini", std::ios::trunc);
    out << text << "version=" << version << "\n";
}

#endif

#endif  // D012_INSTALLS_UPDATES

} // namespace

#if defined(D012_INSTALLS_UPDATES)

bool can_install(const rules::Release &release) {
#if defined(D012_INSTALLS_ON_WINDOWS)
    return !kFromSource && *kUpdateSigningKey != '\0' && installed_by_the_setup() && !running_under_wine() &&
           update_files(release).has_value();
#elif defined(D012_INSTALLS_ON_ANDROID)
    return !kFromSource && *kUpdateSigningKey != '\0' && update_files(release).has_value();
#else
    return !kFromSource && *kUpdateSigningKey != '\0' && installed_by_the_setup() && update_files(release).has_value();
#endif
}

Status status() {
    std::lock_guard lock(g_mutex);
#if defined(D012_INSTALLS_ON_ANDROID)
    if (g_status.step == Step::Installing && g_asked.load()) {
        const portablekit::android::InstallAnswer answer = portablekit::android::install_answer();
        using State = portablekit::android::InstallAnswer::State;
        if (answer.state == State::Cancelled) {
            std::cout << "[update] the player did not install it" << std::endl;
            g_asked.store(false);
            g_status.step = Step::Ready;
            g_status.problem = "The update was not installed";
        } else if (answer.state == State::Failed) {
            std::cout << "[update] Android refused it: status " << answer.status << ", " << answer.message << std::endl;
            g_asked.store(false);
            g_status.step = Step::Failed;
            g_status.problem = refusal(answer.status);
        }
    }
#endif
    return g_status;
}

void start(const rules::Release &release) {
    if (g_busy.exchange(true)) return;
    g_cancel.store(false);
    {
        std::lock_guard lock(g_mutex);
        g_status = Status{};
        g_status.step = Step::Preparing;
        g_status.version = release.version;
        g_program.clear();
    }
    std::cout << "[update] getting version " << release.version << std::endl;
    const auto failed = [](const std::string &text) {
        std::cout << "[update] " << text << std::endl;
        std::lock_guard lock(g_mutex);
        g_status.step = Step::Failed;
        g_status.problem = text;
    };
    try {
        std::thread([release, failed] {
            try {
                work(release);
                std::cout << "[update] version " << release.version << " is ready to install" << std::endl;
            } catch (const Cancelled &) {
                std::cout << "[update] cancelled" << std::endl;
                set_step(Step::Idle);
            } catch (const Problem &problem) {
                failed(problem.text);
            } catch (const std::exception &e) {
                std::cout << "[update] " << e.what() << std::endl;
                failed("the update could not be prepared");
            }
            g_busy.store(false);
        }).detach();
    } catch (const std::exception &e) {
        std::cout << "[update] " << e.what() << std::endl;
        failed("the update could not be prepared");
        g_busy.store(false);
    }
}

void cancel() { g_cancel.store(true); }

#if defined(D012_INSTALLS_ON_WINDOWS)

bool install_now() {
    // Once: the game may draw another frame before it closes.
    static std::atomic<bool> started{false};
    fs::path setup;
    std::string version;
    {
        std::lock_guard lock(g_mutex);
        if (g_status.step != Step::Ready) return false;
        setup = g_program;
        version = g_status.version;
    }
    if (started.exchange(true)) return true;
    // The setup reads the disc image the game plays from. Where that has gone,
    // its wizard asks for it.
    std::error_code ec;
    const bool disc_found = fs::is_regular_file(portablekit::disc_image_path(), ec);
    write_pending(version);
    if (!start_setup(setup, disc_found)) {
        started.store(false);
        fs::remove(folder() / kPendingFile, ec);
        std::lock_guard lock(g_mutex);
        g_status.step = Step::Failed;
        g_status.problem = "the setup could not be started";
        return false;
    }
    std::cout << "[update] the setup is running; closing the game" << std::endl;
#if defined(PORTABLEKIT_HAS_RENDERER)
    portablekit::ui::request_quit();
#endif
    return true;
}

bool finish_at_start() {
    if (kFromSource) return false;
    std::error_code ec;
    const fs::path pending = folder() / kPendingFile;
    if (!fs::is_regular_file(pending, ec)) {
        clear_finished_downloads();
        return false;
    }
    const std::string version = read_pending();
    if (version != kVersion) {
        // The setup never replaced this program.
        fs::remove(pending, ec);
        clear_finished_downloads();
        return false;
    }
    const fs::path setup = folder() / rules::setup_file_name(version);
    const std::wstring title = L"Dissidia 012 (PC port)";
    if (!fs::is_regular_file(setup, ec)) {
        fs::remove(pending, ec);
        MessageBoxW(nullptr,
                    L"The last update did not finish. If the game does not start, run the newest setup from the "
                    L"project's releases page and install into this folder. Your saves, settings and mods stay.",
                    title.c_str(), MB_OK | MB_ICONWARNING);
        return false;
    }
    const int answer = MessageBoxW(nullptr, L"The last update did not finish. Finish it now?", title.c_str(),
                                   MB_YESNO | MB_ICONQUESTION);
    if (answer != IDYES) return false;
    return start_setup(setup, true);
}

#elif defined(D012_INSTALLS_ON_ANDROID)

bool install_now() {
    fs::path app;
    {
        std::lock_guard lock(g_mutex);
        if (g_status.step != Step::Ready && g_status.step != Step::Installing) return false;
        app = g_program;
        g_status.step = Step::Installing;
        g_status.problem.clear();
    }
    // One hand-over at a time; pressed again, Android is asked again.
    if (g_handing.exchange(true)) return true;
    g_asked.store(false);
    std::cout << "[update] handing " << app.filename().string() << " to Android's installer" << std::endl;
    try {
        std::thread([app] {
            if (portablekit::android::install_package(app.string())) {
                std::cout << "[update] Android asks the player to install it" << std::endl;
                g_asked.store(true);
            } else {
                const portablekit::android::InstallAnswer answer = portablekit::android::install_answer();
                std::cout << "[update] Android's installer did not take it: " << answer.message << std::endl;
                std::lock_guard lock(g_mutex);
                g_status.step = Step::Failed;
                g_status.problem = "Android's installer could not take the update";
            }
            g_handing.store(false);
        }).detach();
    } catch (const std::exception &e) {
        std::cout << "[update] " << e.what() << std::endl;
        g_handing.store(false);
        std::lock_guard lock(g_mutex);
        g_status.step = Step::Failed;
        g_status.problem = "Android's installer could not take the update";
        return false;
    }
    return true;
}

bool finish_at_start() {
    // Android installed the app, or the player chose not to: either way a
    // download of this version or older has served its turn.
    clear_finished_downloads();
    return false;
}

#else  // Linux

bool install_now() {
    // Once: the game may draw another frame before it quits.
    static std::atomic<bool> started{false};
    std::string version;
    {
        std::lock_guard lock(g_mutex);
        if (g_status.step != Step::Ready) return false;
        version = g_status.version;
    }
    if (started.exchange(true)) return true;
    try {
        swap_in(version);
    } catch (const Problem &problem) {
        started.store(false);
        std::cout << "[update] " << problem.text << std::endl;
        std::lock_guard lock(g_mutex);
        g_status.step = Step::Failed;
        g_status.problem = problem.text;
        return false;
    }
    std::cout << "[update] version " << version << " is in place; the game starts again" << std::endl;
    portablekit::install::request_restart_on_exit();
#if defined(PORTABLEKIT_HAS_RENDERER)
    portablekit::ui::request_quit();
#endif
    return true;
}

bool finish_at_start() {
    // After an update, prepare_after_update clears up once the game's code is
    // ready; otherwise what a download left goes now.
    std::error_code ec;
    if (!kFromSource && !fs::is_directory(portablekit::executable_directory() / kOldFiles, ec))
        clear_finished_downloads();
    return false;
}

#endif

#else

bool can_install(const rules::Release &) { return false; }
Status status() { return {}; }
void start(const rules::Release &) {}
void cancel() {}
bool install_now() { return false; }
bool finish_at_start() { return false; }

#endif

#if defined(D012_INSTALLS_ON_LINUX)

bool prepare_after_update(portablekit::install::InstallerUi *given, const fs::path &, const fs::path &) {
    std::error_code ec;
    const fs::path old = portablekit::executable_directory() / kOldFiles;
    if (!fs::is_directory(old, ec)) return true;
    std::cout << "[update] this copy was just updated: checking the game's code" << std::endl;
    if (compile_game::run_here(true, {}) != 0) {
        // The setup's screens, as the Android app's compile uses them
        // (d012_device_compile.cpp).
        std::unique_ptr<portablekit::install::InstallerUi> own;
        if (given == nullptr) own = portablekit::install::make_installer_ui();
        portablekit::install::InstallerUi *ui = given != nullptr ? given : own.get();
        std::string failure;
        int code = 1;
#if defined(PORTABLEKIT_HAS_RENDERER)
        // The screen stays on: the work can take many minutes.
        SDL_DisableScreenSaver();
#endif
        const auto work = [&] {
            // Called from the compile's worker threads too: it must not throw,
            // so the screen's Cancel is not honoured while it compiles.
            std::mutex lock;
            code = compile_game::run_here(false, [&](const std::string &message, unsigned percent) {
                const std::lock_guard<std::mutex> guard(lock);
                failure = message;
                if (!ui) return;
                try {
                    ui->progress(message, percent, 100u);
                } catch (...) {
                }
            });
        };
        try {
            if (ui) ui->run_task("Making the game's code for version " + std::string(kVersion), work);
            else work();
        } catch (const portablekit::install::InstallCancelled &) {
#if defined(PORTABLEKIT_HAS_RENDERER)
            SDL_EnableScreenSaver();
#endif
            return false;
        }
#if defined(PORTABLEKIT_HAS_RENDERER)
        SDL_EnableScreenSaver();
#endif
        if (code != 0) {
            portablekit::install::report_problem(
                "The game could not be made",
                "Making the game's code for the new version did not succeed:\n\n" + failure +
                    "\n\nStart the game again to try again, or run the newest setup from the project's releases page "
                    "and install into this folder. The log is game-code-build.log in the game's folder.",
                false);
            return false;
        }
    }
    fs::remove_all(old, ec);
    clear_finished_downloads();
    std::cout << "[update] version " << kVersion << " is ready" << std::endl;
    return true;
}

#else

bool prepare_after_update(portablekit::install::InstallerUi *, const std::filesystem::path &,
                          const std::filesystem::path &) {
    return true;
}

#endif

} // namespace d012::update_install
