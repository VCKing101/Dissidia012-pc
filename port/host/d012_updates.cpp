// Checking for a newer version (d012_updates.hpp).
//
// The check asks GitHub for the project's latest release
// (update_rules::kLatestRelease) and compares its tag with d012::kVersion,
// tools/port/release-version.txt's number compiled in. It runs on a thread of
// its own, so the game never waits for it: through WinHTTP on Windows, which
// is part of Windows, through Java's HttpURLConnection on Android (no app can
// load the system's libcurl there), and through the system's libcurl
// elsewhere, opened when the check runs (SteamOS and nearly every desktop
// Linux have it; without it the menu says the check cannot run). Nothing is sent but the request: the
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
// The check itself downloads and runs nothing. "Open the download page" opens
// the release's page in the browser, and only a page of this project's releases
// (update_rules::kReleasePages). "Download and install", in a copy the Windows
// or Linux setup installed and in the player's Android app, is
// d012_update_install.cpp.
//
// MIT licence; see port/README.md.

#include "d012_updates.hpp"

#include "d012_input.hpp"
#include "d012_update_install.hpp"
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
#elif defined(__ANDROID__)
#include <SDL3/SDL_system.h>
#include <jni.h>
#else
#include "d012_curl.hpp"
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
#elif defined(__ANDROID__)
// java.net.HttpURLConnection, called through JNI on the check's own thread
// (SDL_GetAndroidJNIEnv attaches it to the Java VM). It follows redirects
// within https by itself.
Answer fetch(const std::string &url) {
    Answer out;
    if (url.rfind("https://", 0) != 0) {
        out.problem = "the address is not one the check can use";
        return out;
    }
    auto *jni = static_cast<JNIEnv *>(SDL_GetAndroidJNIEnv());
    if (jni == nullptr || jni->PushLocalFrame(32) != JNI_OK) {
        std::cout << "[updates] no Java environment: " << SDL_GetError() << std::endl;
        out.problem = "the check could not start";
        return out;
    }
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
        std::cout << "[updates] " << step << ": " << text << std::endl;
        return true;
    };
    jobject connection = nullptr;
    jclass http = nullptr;
    [&] {
        jclass url_class = jni->FindClass("java/net/URL");
        http = jni->FindClass("java/net/HttpURLConnection");
        jclass stream_class = jni->FindClass("java/io/InputStream");
        if (thrown("classes") || url_class == nullptr || http == nullptr || stream_class == nullptr) {
            out.problem = "the check could not start";
            return;
        }
        jobject address = jni->NewObject(url_class, jni->GetMethodID(url_class, "<init>", "(Ljava/lang/String;)V"),
                                         jni->NewStringUTF(url.c_str()));
        if (thrown("URL")) {
            out.problem = "the address is not one the check can use";
            return;
        }
        connection = jni->CallObjectMethod(
            address, jni->GetMethodID(url_class, "openConnection", "()Ljava/net/URLConnection;"));
        if (thrown("openConnection") || connection == nullptr || !jni->IsInstanceOf(connection, http)) {
            connection = nullptr;
            out.problem = "GitHub could not be reached";
            return;
        }
        const jmethodID property =
            jni->GetMethodID(http, "setRequestProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
        const std::string agent = std::string("Dissidia012-pc/") + kVersion;
        jni->CallVoidMethod(connection, property, jni->NewStringUTF("User-Agent"), jni->NewStringUTF(agent.c_str()));
        jni->CallVoidMethod(connection, property, jni->NewStringUTF("Accept"),
                            jni->NewStringUTF("application/vnd.github+json"));
        jni->CallVoidMethod(connection, jni->GetMethodID(http, "setConnectTimeout", "(I)V"), 10000);
        jni->CallVoidMethod(connection, jni->GetMethodID(http, "setReadTimeout", "(I)V"), 20000);
        const jint status = jni->CallIntMethod(connection, jni->GetMethodID(http, "getResponseCode", "()I"));
        if (thrown("getResponseCode")) {
            out.problem = "GitHub could not be reached";
            return;
        }
        out.status = status;
        if (status != 200) return;  // only the status matters then
        jobject stream =
            jni->CallObjectMethod(connection, jni->GetMethodID(http, "getInputStream", "()Ljava/io/InputStream;"));
        if (thrown("getInputStream") || stream == nullptr) {
            out.status = 0;
            out.problem = "GitHub could not be reached";
            return;
        }
        const jmethodID read = jni->GetMethodID(stream_class, "read", "([B)I");
        jbyteArray buffer = jni->NewByteArray(16384);
        for (;;) {
            const jint got = jni->CallIntMethod(stream, read, buffer);
            if (thrown("read")) {
                out.status = 0;
                out.problem = "GitHub could not be reached";
                break;
            }
            if (got < 0) break;
            if (out.body.size() + static_cast<std::size_t>(got) > kMaxAnswer) {
                out.status = 0;
                out.problem = "GitHub's answer was too long";
                break;
            }
            const std::size_t at = out.body.size();
            out.body.resize(at + static_cast<std::size_t>(got));
            jni->GetByteArrayRegion(buffer, 0, got, reinterpret_cast<jbyte *>(out.body.data() + at));
        }
        jni->CallVoidMethod(stream, jni->GetMethodID(stream_class, "close", "()V"));
        (void)thrown("close");
    }();
    if (connection != nullptr) {
        jni->CallVoidMethod(connection, jni->GetMethodID(http, "disconnect", "()V"));
        (void)thrown("disconnect");
    }
    jni->PopLocalFrame(nullptr);
    return out;
}
#else
// The system's libcurl (d012_curl.hpp).
using namespace d012::curl;

std::size_t keep(char *data, std::size_t size, std::size_t count, void *to) {
    Answer &out = *static_cast<Answer *>(to);
    const std::size_t bytes = size * count;
    if (out.body.size() + bytes > kMaxAnswer) return 0;  // stops the transfer
    out.body.append(data, bytes);
    return bytes;
}

Answer fetch(const std::string &url) {
    Answer out;
    const Api *c = api();
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

} // namespace

Fetched get(const std::string &url) {
    const Answer answer = fetch(url);
    return Fetched{answer.status, answer.body, answer.problem};
}

namespace {

// --- The check -------------------------------------------------------------

enum class Stage { Idle, Checking, Newest, Available, Failed };

struct Result {
    Stage stage = Stage::Idle;
    std::string latest;   // Newest, Available: the release's version; empty when there is none
    std::string page;     // Available
    std::optional<rules::Release> release;  // Available
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
        r.release = release;
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
#if defined(__ANDROID__)
    const char *hint = "Menu (or Back): System, Updates";
#else
    const char *hint = "Esc (or L3 + R3): System, Updates";
#endif
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

// Downloading and installing `release`, in a copy that can.
void install_rows(const rules::Release &release) {
    namespace ui = portablekit::ui;
    namespace install = update_install;
    const install::Status s = install::status();
    // A row that replaces the one the player pressed takes its focus, so a
    // controller is never left with nothing selected.
    static install::Step drawn = install::Step::Idle;
    const bool arrived = s.step != drawn;
    drawn = s.step;
    const auto take_focus = [&] {
        if (arrived) ui::focus_next_row();
    };
    const auto megabytes = [](std::uint64_t bytes) { return std::to_string((bytes + 512u * 1024u) / (1024u * 1024u)); };
    const auto download_row = [&](const char *label) {
        ui::RowOptions row;
#if defined(__ANDROID__)
        row.description = "Downloads version " + release.version + ". Android asks before it installs it.";
#elif defined(__linux__)
        row.description = "Downloads version " + release.version + ". The game asks before it restarts to install it.";
#else
        row.description = "Downloads version " + release.version + ". The game asks before it closes to install it.";
#endif
        take_focus();
        if (ui::button_row(label, row)) install::start(release);
    };
#if defined(__ANDROID__)
    const auto install_row = [&](const char *description) {
        ui::RowOptions row;
        row.description = description;
        take_focus();
        if (ui::button_row("Install the update", row)) (void)install::install_now();
    };
#endif
    switch (s.step) {
    case install::Step::Idle:
        download_row("Download and install");
        break;
    case install::Step::Preparing:
        ui::info_row("Update##updates", "Getting ready…");
        break;
    case install::Step::Downloading: {
        const float fraction = s.total > 0 ? static_cast<float>(static_cast<double>(s.done) / static_cast<double>(s.total)) : 0.0f;
        ui::progress_bar(fraction, "Downloading: " + megabytes(s.done) + " of " + megabytes(s.total) + " MB");
        ui::RowOptions row;
        row.description = "Stops the download. Choosing Download and install again continues it.";
        take_focus();
        if (ui::button_row("Cancel the download", row)) install::cancel();
        break;
    }
    case install::Step::Checking:
        ui::info_row("Update##updates", "Checking the download…");
        break;
    case install::Step::Unpacking:
        ui::info_row("Update##updates", "Unpacking the update…");
        break;
    case install::Step::Ready: {
#if defined(__ANDROID__)
        if (!s.problem.empty()) ui::info_row("Update##updates", s.problem);
        const std::string description =
            "Android asks you to confirm, then closes the game and installs version " + s.version +
            ". Open the game again when it has finished. The first time, Android asks you to allow this app to "
            "install apps. Your saves, settings and mods stay. Progress since your last save is lost.";
        install_row(description.c_str());
#elif defined(__linux__)
        ui::RowOptions row;
        row.description = "Installs version " + s.version +
                          ", closes the game and starts it again. If its game code differs, the game makes it first, "
                          "which takes a few minutes. Your saves, settings and mods stay. Progress since your last save "
                          "is lost.";
        take_focus();
        if (ui::button_row("Install and restart", row)) (void)install::install_now();
#else
        ui::RowOptions row;
        row.description = "Closes the game, installs version " + s.version +
                          " and starts it again. Your saves, settings and mods stay. Progress since your last save "
                          "is lost.";
        take_focus();
        if (ui::button_row("Close the game and install", row)) (void)install::install_now();
#endif
        break;
    }
    case install::Step::Installing:
        ui::info_row("Update##updates", "Waiting for you to confirm in Android's window");
#if defined(__ANDROID__)
        install_row("Asks Android again, if its window has gone.");
#endif
        break;
    case install::Step::Failed:
        ui::info_row("Update##updates", "Could not update: " + s.problem);
        download_row("Try again");
        break;
    }
}

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
        ui::info_row("Status##updates", "Checking…");
        break;
    case Stage::Newest:
        ui::info_row("Status##updates", std::string("This is the newest version (") + kVersion + ")");
        break;
    case Stage::Available: {
        ui::info_row("Status##updates", "Version " + r.latest + " is available");
        if (r.release && update_install::can_install(*r.release)) install_rows(*r.release);
        ui::RowOptions open;
#if defined(__ANDROID__)
        open.description = "Opens the release's page in your browser, with the Android app (the .apk). Install it "
                           "over this one: your saves, settings and mods stay.";
#else
        open.description = "Opens the release's page in your browser, with the setups for Windows and Linux. "
                           "Install it into the same folder: your saves, settings and mods stay.";
#endif
        if (ui::button_row("Open the download page", open) && !SDL_OpenURL(r.page.c_str()))
            std::cout << "[updates] cannot open " << r.page << ": " << SDL_GetError() << std::endl;
        ui::info_row("Page##updates", r.page);
        break;
    }
    case Stage::Failed:
        ui::info_row("Status##updates", "Could not check: " + r.problem);
        break;
    }
}
#endif

} // namespace

void install(portablekit::HleRegistrar &) {
    static bool installed = false;
    if (installed) return;
    installed = true;
#if defined(__ANDROID__)
    // Android installs an update itself (Windows' setup does this at the
    // program's start, d012_compile_game.cpp): what is left is the download.
    (void)update_install::finish_at_start();
#endif
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
