// Checking for a newer version (d012_updates.hpp).
//
// The check asks GitHub for the project's latest release
// (update_rules::kLatestRelease) and compares its tag with d012::kVersion,
// tools/port/release-version.txt's number compiled in. It runs on a thread of
// its own, so the game never waits for it: through WinHTTP on Windows, which
// is part of Windows, and through the system's libcurl elsewhere, opened when
// the check runs (SteamOS and nearly every desktop Linux have it; without it
// the menu says the check cannot run). Nothing is sent but the request: the
// address, and a User-Agent naming the port and its version, which GitHub's
// API asks for.
//
// When it runs:
// - from the System page's "Check for updates";
// - once when the game starts, with settings.ini's updates.check_at_start on
//   (it is unless the player turns it off), in a copy made by the setup with
//   its interface on. A newer version then shows a notice over the logos and
//   the title screen until the player presses a button, so never in a
//   battle. A copy built from source (setup.ps1) does not
//   check by itself, and neither does a scripted test run
//   (D012_INPUT_SCRIPT), whose pictures a notice would change.
// D012_UPDATE_CHECK=0 or 1 decides the check at start over settings.ini; 1
// checks even with no interface (D012_NO_RENDER), as soon as the game starts,
// for tests of the check itself.
// D012_UPDATE_URL=<file> reads the answer from a file instead of GitHub, for
// tests (it also checks at start in a scripted run).
//
// It never downloads or runs anything: "Open the download page" opens the
// release's page in the browser, and only a page of this project's releases
// (update_rules::kReleasePages).
//
// MIT licence; see port/README.md.

#include "d012_updates.hpp"

#include "d012_input.hpp"
#include "d012_update_rules.hpp"
#include "d012_version.hpp"

#include "install/user_data.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "ui/ui.hpp"
#include "ui/widgets.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>
#endif

#if defined(_WIN32)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <windows.h>
#include <winhttp.h>
#else
#include <dlfcn.h>
#endif

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <mutex>
#include <string>
#include <thread>
#include <type_traits>

namespace d012::updates {
namespace {

namespace rules = update_rules;
using Clock = std::chrono::steady_clock;

constexpr const char *kCheckAtStartKey = "updates.check_at_start";
// The notice: over the logos and the title screen until the player presses a
// button (reaching a battle takes many), at most this long after the game
// starts, and only when the answer came in the game's first minute. A press
// counts once the notice has been on screen a moment, so the one answering
// the game's message at start (its autosave notice, drawn instead of the
// notice) does not hide it.
constexpr auto kNoticeWithin = std::chrono::seconds(60);
constexpr auto kNoticeAtMost = std::chrono::seconds(120);
constexpr auto kNoticeBeforePress = std::chrono::seconds(2);
// More than any answer of GitHub's for one release.
constexpr std::size_t kMaxAnswer = 1u << 20;

const char *env(const char *name) {
    const char *text = std::getenv(name);
    return text != nullptr && *text != '\0' ? text : nullptr;
}

// --- Fetching --------------------------------------------------------------

struct Answer {
    long status = 0;      // HTTP status, 0 when none came
    std::string body;
    std::string problem;  // for the player, when it could not be fetched
};

#if defined(_WIN32)
Answer fetch(const std::string &url) {
    Answer out;
    const std::wstring wide(url.begin(), url.end());  // the addresses are ASCII
    wchar_t host[256] = {};
    wchar_t path[2048] = {};
    URL_COMPONENTS parts{};
    parts.dwStructSize = sizeof parts;
    parts.lpszHostName = host;
    parts.dwHostNameLength = static_cast<DWORD>(std::size(host));
    parts.lpszUrlPath = path;
    parts.dwUrlPathLength = static_cast<DWORD>(std::size(path));
    if (!WinHttpCrackUrl(wide.c_str(), 0, 0, &parts) || parts.nScheme != INTERNET_SCHEME_HTTPS) {
        out.problem = "the address is not one the check can use";
        return out;
    }
    const std::wstring agent = std::wstring(L"Dissidia012-pc/") +
                               std::wstring(kVersion, kVersion + std::strlen(kVersion));
    HINTERNET session = WinHttpOpen(agent.c_str(), WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME,
                                    WINHTTP_NO_PROXY_BYPASS, 0);
    HINTERNET connection = nullptr;
    HINTERNET request = nullptr;
    const auto failed = [&](const char *step) {
        const DWORD code = GetLastError();
        std::cout << "[updates] " << step << " failed: WinHTTP error " << code << std::endl;
        out.problem = "GitHub could not be reached";
    };
    if (session == nullptr) {
        failed("WinHttpOpen");
    } else if (WinHttpSetTimeouts(session, 10000, 10000, 10000, 10000),
               (connection = WinHttpConnect(session, host, parts.nPort, 0)) == nullptr) {
        failed("WinHttpConnect");
    } else if ((request = WinHttpOpenRequest(connection, L"GET", path, nullptr, WINHTTP_NO_REFERER,
                                             WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE)) == nullptr) {
        failed("WinHttpOpenRequest");
    } else if (!WinHttpSendRequest(request, L"Accept: application/vnd.github+json\r\n", static_cast<DWORD>(-1),
                                   WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
               !WinHttpReceiveResponse(request, nullptr)) {
        failed("the request");
    } else {
        DWORD status = 0;
        DWORD size = sizeof status;
        WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                            WINHTTP_HEADER_NAME_BY_INDEX, &status, &size, WINHTTP_NO_HEADER_INDEX);
        out.status = static_cast<long>(status);
        for (;;) {
            DWORD available = 0;
            if (!WinHttpQueryDataAvailable(request, &available)) {
                failed("reading the answer");
                break;
            }
            if (available == 0) break;
            if (out.body.size() + available > kMaxAnswer) {
                out.problem = "GitHub's answer was too long";
                break;
            }
            const std::size_t at = out.body.size();
            out.body.resize(at + available);
            DWORD read = 0;
            if (!WinHttpReadData(request, out.body.data() + at, available, &read)) {
                failed("reading the answer");
                break;
            }
            out.body.resize(at + read);
        }
    }
    if (request != nullptr) WinHttpCloseHandle(request);
    if (connection != nullptr) WinHttpCloseHandle(connection);
    if (session != nullptr) WinHttpCloseHandle(session);
    return out;
}
#else
// The few libcurl calls the check makes, opened from the system's libcurl.
// The option and information numbers are libcurl's stable ABI (curl.h).
struct Curl {
    void *(*easy_init)() = nullptr;
    int (*easy_setopt)(void *, int, ...) = nullptr;
    int (*easy_perform)(void *) = nullptr;
    int (*easy_getinfo)(void *, int, ...) = nullptr;
    void (*easy_cleanup)(void *) = nullptr;
    const char *(*easy_strerror)(int) = nullptr;
    void *(*slist_append)(void *, const char *) = nullptr;
    void (*slist_free_all)(void *) = nullptr;
};
constexpr int kOptWriteData = 10001;
constexpr int kOptUrl = 10002;
constexpr int kOptUserAgent = 10018;
constexpr int kOptHttpHeader = 10023;
constexpr int kOptWriteFunction = 20011;
constexpr int kOptTimeout = 13;
constexpr int kOptFollowLocation = 52;
constexpr int kOptConnectTimeout = 78;
constexpr int kOptNoSignal = 99;
constexpr int kInfoResponseCode = 0x200002;

const Curl *curl() {
    static const Curl loaded = [] {
        Curl c;
        void *lib = nullptr;
        for (const char *name : {"libcurl.so.4", "libcurl-gnutls.so.4", "libcurl.4.dylib"})
            if ((lib = dlopen(name, RTLD_NOW | RTLD_LOCAL)) != nullptr) break;
        if (lib == nullptr) {
            std::cout << "[updates] no libcurl on this system: " << dlerror() << std::endl;
            return c;
        }
        const auto get = [&](auto &slot, const char *name) {
            slot = reinterpret_cast<std::remove_reference_t<decltype(slot)>>(dlsym(lib, name));
        };
        get(c.easy_init, "curl_easy_init");
        get(c.easy_setopt, "curl_easy_setopt");
        get(c.easy_perform, "curl_easy_perform");
        get(c.easy_getinfo, "curl_easy_getinfo");
        get(c.easy_cleanup, "curl_easy_cleanup");
        get(c.easy_strerror, "curl_easy_strerror");
        get(c.slist_append, "curl_slist_append");
        get(c.slist_free_all, "curl_slist_free_all");
        return c;
    }();
    const Curl &c = loaded;
    const bool complete = c.easy_init && c.easy_setopt && c.easy_perform && c.easy_getinfo && c.easy_cleanup &&
                          c.easy_strerror && c.slist_append && c.slist_free_all;
    return complete ? &c : nullptr;
}

std::size_t keep(char *data, std::size_t size, std::size_t count, void *to) {
    Answer &out = *static_cast<Answer *>(to);
    const std::size_t bytes = size * count;
    if (out.body.size() + bytes > kMaxAnswer) return 0;  // stops the transfer
    out.body.append(data, bytes);
    return bytes;
}

Answer fetch(const std::string &url) {
    Answer out;
    const Curl *c = curl();
    if (c == nullptr) {
        out.problem = "this system has no libcurl, which the check needs";
        return out;
    }
    if (url.rfind("https://", 0) != 0) {
        out.problem = "the address is not one the check can use";
        return out;
    }
    void *handle = c->easy_init();
    if (handle == nullptr) {
        out.problem = "the check could not start";
        return out;
    }
    const std::string agent = std::string("Dissidia012-pc/") + kVersion;
    void *headers = c->slist_append(nullptr, "Accept: application/vnd.github+json");
    c->easy_setopt(handle, kOptUrl, url.c_str());
    c->easy_setopt(handle, kOptUserAgent, agent.c_str());
    c->easy_setopt(handle, kOptHttpHeader, headers);
    c->easy_setopt(handle, kOptWriteFunction, &keep);
    c->easy_setopt(handle, kOptWriteData, static_cast<void *>(&out));
    c->easy_setopt(handle, kOptFollowLocation, 1L);
    c->easy_setopt(handle, kOptConnectTimeout, 10L);
    c->easy_setopt(handle, kOptTimeout, 20L);
    c->easy_setopt(handle, kOptNoSignal, 1L);
    if (const int code = c->easy_perform(handle); code != 0) {
        std::cout << "[updates] libcurl: " << c->easy_strerror(code) << std::endl;
        out.problem = out.body.size() >= kMaxAnswer ? "GitHub's answer was too long" : "GitHub could not be reached";
    } else {
        long status = 0;
        c->easy_getinfo(handle, kInfoResponseCode, &status);
        out.status = status;
    }
    c->easy_cleanup(handle);
    c->slist_free_all(headers);
    return out;
}
#endif

// D012_UPDATE_URL's file, as if GitHub had answered it.
Answer read_file(const std::string &path) {
    Answer out;
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        out.problem = "D012_UPDATE_URL's file cannot be read";
        return out;
    }
    out.body.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    out.status = 200;
    return out;
}

// --- The check -------------------------------------------------------------

enum class Stage { Idle, Checking, Newest, Available, Failed };

struct Result {
    Stage stage = Stage::Idle;
    std::string latest;   // Newest, Available: the release's version; empty when there is none
    std::string page;     // Available
    std::string problem;  // Failed
    Clock::time_point when{};
};

std::mutex g_mutex;
Result g_result;
std::atomic<bool> g_busy{false};
const Clock::time_point g_start = Clock::now();

Result result() {
    std::lock_guard lock(g_mutex);
    return g_result;
}

Result check() {
    Result r;
    r.when = Clock::now();
    const char *source = env("D012_UPDATE_URL");
    const Answer answer = source == nullptr                           ? fetch(rules::kLatestRelease)
                          : std::strncmp(source, "https://", 8) == 0 ? fetch(source)
                                                                      : read_file(source);
    if (!answer.problem.empty()) {
        r.stage = Stage::Failed;
        r.problem = answer.problem;
        return r;
    }
    if (answer.status == 404) {
        // No release yet that is not a pre-release.
        r.stage = Stage::Newest;
        return r;
    }
    if (answer.status == 403 || answer.status == 429) {
        r.stage = Stage::Failed;
        r.problem = "GitHub asks to wait before checking again";
        return r;
    }
    if (answer.status != 200) {
        r.stage = Stage::Failed;
        r.problem = "GitHub answered with error " + std::to_string(answer.status);
        return r;
    }
    std::string error;
    const std::optional<rules::Release> release = rules::parse_release(answer.body, error);
    if (!release) {
        std::cout << "[updates] GitHub's answer not read: " << error << std::endl;
        r.stage = Stage::Failed;
        r.problem = "GitHub's answer could not be read";
        return r;
    }
    r.latest = release->version;
    if (rules::compare_versions(release->version, kVersion) > 0) {
        r.stage = Stage::Available;
        r.page = release->page;
    } else {
        r.stage = Stage::Newest;
    }
    return r;
}

// Starts a check unless one is running.
void start_check(const char *why) {
    if (g_busy.exchange(true)) return;
    {
        std::lock_guard lock(g_mutex);
        g_result.stage = Stage::Checking;
    }
    std::cout << "[updates] checking (" << why << "), this version " << kVersion << std::endl;
    try {
        std::thread([] {
            const Result r = check();
            if (r.stage == Stage::Failed)
                std::cout << "[updates] could not check: " << r.problem << std::endl;
            else if (r.stage == Stage::Available)
                std::cout << "[updates] version " << r.latest << " is available: " << r.page << std::endl;
            else
                std::cout << "[updates] this is the newest version"
                          << (r.latest.empty() ? " (no release found)" : " (latest " + r.latest + ")") << std::endl;
            {
                std::lock_guard lock(g_mutex);
                g_result = r;
            }
            g_busy.store(false);
        }).detach();
    } catch (const std::exception &e) {
        std::cout << "[updates] the check could not start: " << e.what() << std::endl;
        std::lock_guard lock(g_mutex);
        g_result.stage = Stage::Failed;
        g_result.problem = "the check could not start";
        g_busy.store(false);
    }
}

// --- The setting -----------------------------------------------------------

std::atomic<bool> &check_at_start() {
    static std::atomic<bool> value{[] {
        try {
            const std::filesystem::path dir = portablekit::install::user_data_directory();
            if (!std::filesystem::exists(dir / portablekit::install::kSettingsFile)) return true;
            const portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
            if (const auto found = entries.find(kCheckAtStartKey); found != entries.end()) {
                const std::string &text = found->second;
                return !(text == "0" || text == "off" || text == "false");
            }
        } catch (const std::exception &e) {
            std::cerr << "[updates] settings.ini not read for " << kCheckAtStartKey << ": " << e.what() << "\n";
        }
        return true;
    }()};
    return value;
}

void save_check_at_start(bool on) {
    check_at_start().store(on);
    try {
        const std::filesystem::path dir = portablekit::install::user_data_directory();
        portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
        entries[kCheckAtStartKey] = on ? "1" : "0";
        portablekit::install::write_settings_file(dir, entries);
    } catch (const std::exception &e) {
        std::cerr << "[updates] settings.ini not written for " << kCheckAtStartKey << ": " << e.what() << "\n";
    }
}

// Whether this run checks by itself once its interface is up.
bool checks_at_start() {
    if (const char *forced = env("D012_UPDATE_CHECK"); forced != nullptr) return std::strcmp(forced, "0") != 0;
    if (env("D012_UPDATE_URL") != nullptr) return check_at_start().load();
    if (kFromSource || env("D012_INPUT_SCRIPT") != nullptr) return false;
    return check_at_start().load();
}

#if defined(PORTABLEKIT_HAS_RENDERER)
// --- The notice ------------------------------------------------------------

std::atomic<bool> g_started{false};
Clock::duration g_notice_shown{};  // how long the notice has been on screen
Clock::time_point g_notice_drawn{};
std::uint32_t g_notice_buttons = 0;  // the buttons held at its latest draw
bool g_notice_done = false;

// Called every frame the interface draws the game: the check at start begins
// here, so a run with no interface never checks.
bool notice_visible() {
    if (!g_started.exchange(true) && checks_at_start()) start_check("at start");
    const Result r = result();
    return r.stage == Stage::Available && r.when - g_start <= kNoticeWithin && !g_notice_done &&
           Clock::now() - g_start < kNoticeAtMost;
}

void notice_draw() {
    // A frame's time on screen, at most a tenth of a second: a pause between
    // two draws (the game's message, the menu) does not count.
    const Clock::time_point now = Clock::now();
    const std::uint32_t buttons = input::current().player1.buttons;
    if (g_notice_drawn != Clock::time_point{}) {
        g_notice_shown += std::min<Clock::duration>(now - g_notice_drawn, std::chrono::milliseconds(100));
        if ((buttons & ~g_notice_buttons) != 0u && g_notice_shown >= kNoticeBeforePress) g_notice_done = true;
    } else {
        std::cout << "[updates] notice shown" << std::endl;
    }
    g_notice_drawn = now;
    g_notice_buttons = buttons;
    const Result r = result();
    const portablekit::ui::GamePicture picture = portablekit::ui::game_picture();
    const float sx = picture.width / 480.0f;
    const float sy = picture.height / 272.0f;
    const auto at = [&](float x, float y) { return ImVec2(picture.left + x * sx, picture.top + y * sy); };
    ImDrawList *draw = ImGui::GetForegroundDrawList();
    ImFont *font = portablekit::ui::game_text_font();
    const std::string title = "Version " + r.latest + " is available";
    const char *hint = "Esc (or L3 + R3): System, Updates";
    // A band at the top right, clear of the title screen's choices.
    const float left = 292.0f;
    const float right = 472.0f;
    const float top = 8.0f;
    draw->AddRectFilled(at(left, top), at(right, top + 34.0f), IM_COL32(10, 14, 40, 220), 4.0f * sy);
    draw->AddRect(at(left, top), at(right, top + 34.0f), IM_COL32(255, 196, 64, 255), 4.0f * sy, 0, 1.2f * sy);
    const auto text = [&](float size, float y, ImU32 colour, const char *line) {
        const ImVec2 extent = font->CalcTextSizeA(size * sy, 1.0e9f, 0.0f, line);
        const ImVec2 middle = at((left + right) * 0.5f, y);
        draw->AddText(font, size * sy, ImVec2(middle.x - extent.x * 0.5f, middle.y - extent.y * 0.5f), colour, line);
    };
    text(10.0f, top + 11.0f, IM_COL32(255, 255, 255, 255), title.c_str());
    text(7.5f, top + 25.0f, IM_COL32(255, 220, 140, 255), hint);
}

// --- The System page -------------------------------------------------------

void system_rows() {
    namespace ui = portablekit::ui;
    ui::section("Updates");
    if (kFromSource) ui::info_row("Built from source", "To update: git pull, then run setup.ps1 again");

    ui::RowOptions at_start;
    at_start.description = "Each time the game starts, asks GitHub whether a newer version is out, and says so "
                           "over the title screen.";
    bool on = check_at_start().load();
    if (const char *forced = env("D012_UPDATE_CHECK"); forced != nullptr) {
        at_start.disabled = true;
        at_start.note = "Set by D012_UPDATE_CHECK";
        on = std::strcmp(forced, "0") != 0;
    } else if (kFromSource) {
        at_start.disabled = true;
        at_start.note = "Not in a copy built from source";
        on = false;
    }
    if (ui::toggle_row("Check when the game starts", on, at_start)) save_check_at_start(!on);

    const Result r = result();
    ui::RowOptions button;
    button.description = "Asks GitHub whether a newer version is out.";
    button.disabled = r.stage == Stage::Checking;
    if (ui::button_row("Check for updates", button)) start_check("from the menu");

    switch (r.stage) {
    case Stage::Idle:
        break;
    case Stage::Checking:
        ui::info_row("Status", "Checking…");
        break;
    case Stage::Newest:
        ui::info_row("Status", std::string("This is the newest version (") + kVersion + ")");
        break;
    case Stage::Available: {
        ui::info_row("Status", "Version " + r.latest + " is available");
        ui::RowOptions open;
        open.description = "Opens the release's page in your browser, with the setups for Windows and Linux. "
                           "Install it into the same folder: your saves, settings and mods stay.";
        if (ui::button_row("Open the download page", open) && !SDL_OpenURL(r.page.c_str()))
            std::cout << "[updates] cannot open " << r.page << ": " << SDL_GetError() << std::endl;
        ui::info_row("Page", r.page);
        break;
    }
    case Stage::Failed:
        ui::info_row("Status", "Could not check: " + r.problem);
        break;
    }
}
#endif

} // namespace

void install(portablekit::HleRegistrar &) {
    static bool installed = false;
    if (installed) return;
    installed = true;
    if (const char *forced = env("D012_UPDATE_CHECK"); forced != nullptr && std::strcmp(forced, "1") == 0) {
#if defined(PORTABLEKIT_HAS_RENDERER)
        g_started.store(true);
#endif
        start_check("D012_UPDATE_CHECK=1");
    }
#if defined(PORTABLEKIT_HAS_RENDERER)
    portablekit::ui::add_system_rows(&system_rows);
    portablekit::ui::add_game_overlay(&notice_visible, &notice_draw);
#endif
}

} // namespace d012::updates
