// Installing a newer version from the menu (d012_update_install.hpp).
//
// Only when this build carries the release key's public half
// (d012::kUpdateSigningKey), and only in a copy the Windows setup installed
// (unins000.exe beside the program) or in the player's Android app (the
// installer build, D012_INSTALLER_BUILD). Elsewhere the System page keeps
// "Open the download page": RecompLauncher updates its own copies, and Linux
// has no install from the menu yet.
//
// The steps, on a thread of their own while the game goes on:
//  1. SHA256SUMS and SHA256SUMS.sig are fetched, and the list is refused unless
//     the signature is the release key's. The list names the program with its
//     version, so an older release's list cannot pass for this one.
//  2. The program (the Windows setup, the Android app) is downloaded to
//     update-download/, as <name>.part, which a later attempt continues: beside
//     the program on Windows, in the app's own storage on Android.
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
#else
// Room for Android's copy of the app, its libraries unpacked, and the game's
// code the app may compile again.
constexpr std::uint64_t kSpaceBesidesProgram = 1024ull * 1024u * 1024u;
// The apps' names: rules::android_file_name.
constexpr std::string_view kProgramPrefix = "Dissidia012-";
constexpr std::string_view kProgramSuffix = "-Android-arm64.apk";
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

#if defined(D012_INSTALLS_ON_WINDOWS)
fs::path folder() { return portablekit::executable_directory() / "update-download"; }
#else
fs::path folder() { return portablekit::install::user_data_directory() / "update-download"; }
#endif

std::optional<rules::UpdateFiles> update_files(const rules::Release &release) {
#if defined(D012_INSTALLS_ON_WINDOWS)
    return rules::windows_update(release);
#else
    return rules::android_update(release);
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

#else  // Android

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
    // Downloads of other versions go.
    for (const fs::directory_entry &entry : fs::directory_iterator(dir, ec))
        if (entry.path() != program && entry.path() != part) fs::remove(entry.path(), ec);

    const auto is_good = [&](const fs::path &file) { return psprecomp::sha256_file(file) == expected; };
    if (fs::is_regular_file(program, ec)) {
        set_step(Step::Checking);
        if (is_good(program)) {
            std::lock_guard lock(g_mutex);
            g_program = program;
            g_status.step = Step::Ready;
            return;
        }
        fs::remove(program, ec);
    }

    const fs::space_info space = fs::space(dir, ec);
    if (!ec && space.available < update->program.size + kSpaceBesidesProgram)
        throw Problem{"there is not enough free space on this device"};

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
    std::lock_guard lock(g_mutex);
    g_program = program;
    g_status.step = Step::Ready;
}

// Downloads of this version or older have served their turn.
void clear_finished_downloads() {
    std::error_code ec;
    const fs::path dir = folder();
    if (!fs::is_directory(dir, ec)) return;
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

#else  // Android

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

#endif

#endif  // D012_INSTALLS_UPDATES

} // namespace

#if defined(D012_INSTALLS_UPDATES)

bool can_install(const rules::Release &release) {
#if defined(D012_INSTALLS_ON_WINDOWS)
    return !kFromSource && *kUpdateSigningKey != '\0' && installed_by_the_setup() && !running_under_wine() &&
           update_files(release).has_value();
#else
    return !kFromSource && *kUpdateSigningKey != '\0' && update_files(release).has_value();
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

#else  // Android

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

#endif

#else

bool can_install(const rules::Release &) { return false; }
Status status() { return {}; }
void start(const rules::Release &) {}
void cancel() {}
bool install_now() { return false; }
bool finish_at_start() { return false; }

#endif

} // namespace d012::update_install
