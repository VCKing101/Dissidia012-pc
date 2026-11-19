// Installing a newer version from the menu (d012_update_install.hpp).
//
// Only in a copy the Windows setup installed (unins000.exe beside the program)
// and only when this build carries the release key's public half
// (d012::kUpdateSigningKey). Elsewhere the System page keeps "Open the download
// page": RecompLauncher updates its own copies, and Linux and Android have no
// install from the menu yet.
//
// The steps, on a thread of their own while the game goes on:
//  1. SHA256SUMS and SHA256SUMS.sig are fetched, and the list is refused unless
//     the signature is the release key's. The list names the setup with its
//     version, so an older release's list cannot pass for this one.
//  2. The setup is downloaded to update-download/ beside the program, as
//     <name>.part, which a later attempt continues.
//  3. Its SHA-256 must be the one the signed list gives. Only then does it get
//     its own name, and only a file with its own name is ever started.
//
// When the player chooses to install, update-download/pending.txt is written
// with the version, the setup starts, and the game closes. The setup
// (tools/port/installer/dissidia012.iss, /UPDATE=1) waits for the game to end,
// replaces the program, compiles the game's code when it must, deletes
// pending.txt and starts the game. A run that finds pending.txt naming its own
// version knows the setup stopped between replacing the program and finishing
// the game's code, and offers to finish; naming another version, the program
// was never replaced and nothing needs doing.
//
// Tests: D012_UPDATE_ASSETS=<folder> takes those of the three files that are in
// the folder from there instead of GitHub; every check is made all the same.
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
#include <thread>
#include <vector>

namespace d012::update_install {
namespace {

namespace fs = std::filesystem;
namespace rules = update_rules;

#if defined(_WIN32)

// Room for the setup, what it unpacks and the game's code it may compile.
constexpr std::uint64_t kSpaceBesidesSetup = 1500ull * 1024u * 1024u;
constexpr const char *kPendingFile = "pending.txt";

std::mutex g_mutex;
Status g_status;
fs::path g_setup;  // Ready: the checked setup
std::atomic<bool> g_busy{false};
std::atomic<bool> g_cancel{false};

struct Cancelled {};
struct Problem {
    std::string text;
};

fs::path folder() { return portablekit::executable_directory() / "update-download"; }

void set_step(Step step) {
    std::lock_guard lock(g_mutex);
    g_status.step = step;
}

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

// --- Downloading -----------------------------------------------------------

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
    const std::optional<rules::WindowsUpdate> update = rules::windows_update(release);
    if (!update) throw Problem{"this release cannot be installed from here"};

    const std::string sums = fetch_small(update->sums);
    const std::string signature = fetch_small(update->signature);
    if (!rules::sums_signed(sums, signature, kUpdateSigningKey))
        throw Problem{"the release's signature is not valid, so nothing was installed"};
    const std::string expected = rules::listed_hash(sums, update->setup.name);
    if (expected.empty()) throw Problem{"the release's signed list does not name its setup"};

    std::error_code ec;
    const fs::path dir = folder();
    fs::create_directories(dir, ec);
    if (ec) throw Problem{"the folder for the download cannot be made beside the game"};
    const fs::path setup = dir / update->setup.name;
    const fs::path part = fs::path(setup).concat(".part");
    // Downloads of other versions go.
    for (const fs::directory_entry &entry : fs::directory_iterator(dir, ec))
        if (entry.path() != setup && entry.path() != part) fs::remove(entry.path(), ec);

    const auto is_good = [&](const fs::path &file) { return psprecomp::sha256_file(file) == expected; };
    if (fs::is_regular_file(setup, ec)) {
        set_step(Step::Checking);
        if (is_good(setup)) {
            std::lock_guard lock(g_mutex);
            g_setup = setup;
            g_status.step = Step::Ready;
            return;
        }
        fs::remove(setup, ec);
    }

    const fs::space_info space = fs::space(dir, ec);
    if (!ec && space.available < update->setup.size + kSpaceBesidesSetup)
        throw Problem{"there is not enough free space on this drive"};

    {
        std::lock_guard lock(g_mutex);
        g_status.step = Step::Downloading;
        g_status.total = update->setup.size;
        g_status.done = fs::is_regular_file(part, ec) ? fs::file_size(part, ec) : 0u;
    }
    if (const std::optional<fs::path> file = test_copy(update->setup)) {
        fs::copy_file(*file, part, fs::copy_options::overwrite_existing, ec);
        if (ec) throw Problem{"the test folder's setup cannot be copied"};
    } else {
        download(update->setup.url, part, update->setup.size);
    }

    set_step(Step::Checking);
    if (!is_good(part)) {
        fs::remove(part, ec);
        throw Problem{"the download is damaged, so nothing was installed. Try again"};
    }
    fs::rename(part, setup, ec);
    if (ec) throw Problem{"the download cannot be kept beside the game"};
    std::lock_guard lock(g_mutex);
    g_setup = setup;
    g_status.step = Step::Ready;
}

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

// Downloads of this version or older have served their turn.
void clear_finished_downloads() {
    std::error_code ec;
    const fs::path dir = folder();
    if (!fs::is_directory(dir, ec)) return;
    const std::string prefix = "Dissidia012-PC-Setup-";
    for (const fs::directory_entry &entry : fs::directory_iterator(dir, ec)) {
        std::string name = entry.path().filename().string();
        if (name.size() > 5 && name.ends_with(".part")) name.resize(name.size() - 5);
        if (!name.starts_with(prefix) || !name.ends_with(".exe")) continue;
        const std::string version = name.substr(prefix.size(), name.size() - prefix.size() - 4);
        if (rules::is_version(version) && rules::compare_versions(version, kVersion) <= 0)
            fs::remove(entry.path(), ec);
    }
    fs::remove(dir, ec);  // only when empty
}

#endif  // _WIN32

} // namespace

#if defined(_WIN32)

bool can_install(const rules::Release &release) {
    return !kFromSource && *kUpdateSigningKey != '\0' && installed_by_the_setup() && !running_under_wine() &&
           rules::windows_update(release).has_value();
}

Status status() {
    std::lock_guard lock(g_mutex);
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
        g_setup.clear();
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

bool install_now() {
    // Once: the game may draw another frame before it closes.
    static std::atomic<bool> started{false};
    fs::path setup;
    std::string version;
    {
        std::lock_guard lock(g_mutex);
        if (g_status.step != Step::Ready) return false;
        setup = g_setup;
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

#else

bool can_install(const rules::Release &) { return false; }
Status status() { return {}; }
void start(const rules::Release &) {}
void cancel() {}
bool install_now() { return false; }
bool finish_at_start() { return false; }

#endif

} // namespace d012::update_install
