// The Discord status (d012_discord.hpp).
//
// The Discord app listens on the PC for programs that show a status: on
// Windows the named pipe \\?\pipe\discord-ipc-0 (to -9), elsewhere the socket
// discord-ipc-0 (to -9) in $XDG_RUNTIME_DIR, the temporary folders, or where
// the Flatpak, Snap and Vesktop Discord apps keep it. The port connects with
// the project's application ID (discord_rules::kApplicationId) and sends
// SET_ACTIVITY each time the status changes, at most once every 5 seconds.
// No sign-in is needed, and nothing leaves the PC from here: Discord's app
// shows the status to the player's friends. Closing the connection (leaving
// the game, or the switch) clears it. It runs on a thread of its own, so the
// game never waits for Discord; without Discord it tries again every 15
// seconds.
//
// What it says (discord_rules::describe), from the game's memory at every
// 15th pad read, on the game's thread:
// - the battle overlay resident (the first two words of battle 0x08B2F2E4,
//   as d012_versus.cpp tells it) with a FIELD object (*(0x089D6A10)): the
//   object's mode at +0x4C (0 Story Mode's world map, 4 Quick Battle, 5
//   Communication Mode, 9 a replay; main 0x0888B544 takes 0 to 14) and each
//   side's character ID (battle
//   setup 0x09AFB8A0 + side * 0x1060, +0x1058; docs/research/
//   roster-and-save.md 1.7);
// - otherwise the menus.
// Each new mode goes to the log ("[discord] FIELD mode N"), to name more.
//
// When: unless the player turns it off (settings.ini discord.show_activity,
// the System page's "Show on Discord"), in every run but a scripted test
// (D012_INPUT_SCRIPT), whose output a status would not change but whose
// runs should not reach the player's Discord. D012_DISCORD=0 or 1 decides
// over the setting; D012_DISCORD_IPC=<path> connects there only, for tests
// with a stand-in Discord. Without an application ID
// (kApplicationId, settings.ini discord.application_id or
// D012_DISCORD_APP_ID) nothing runs.
//
// MIT licence; see port/README.md.

#include "d012_discord.hpp"

#include "d012_discord_rules.hpp"

#include "hle/hle_common.hpp"
#include "install/user_data.hpp"
#include "psprecomp/runtime.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "ui/ui.hpp"
#include "ui/widgets.hpp"
#endif

#if defined(_WIN32)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <cerrno>
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

namespace d012::discord {
namespace {

namespace rules = discord_rules;
using Clock = std::chrono::steady_clock;
using namespace std::chrono_literals;

constexpr const char *kShowKey = "discord.show_activity";
constexpr const char *kApplicationKey = "discord.application_id";
constexpr auto kRetry = 15s;            // without Discord
constexpr auto kRetryRefused = 60s;     // after Discord refused the handshake
constexpr auto kBetweenUpdates = 5s;    // Discord takes a few a minute
constexpr auto kHandshakeWait = 5000ms;
constexpr std::uint32_t kSampleEvery = 15u;  // pad reads, a half second

constexpr std::uint32_t kSceCtrlReadBufferPositive = 0x1F803938u;
constexpr std::uint32_t kSetAiMode = 0x08B2F2E4u;  // battle overlay code
constexpr std::uint32_t kBattleManager = 0x089D6A10u;  // -> FIELD
constexpr std::uint32_t kFieldMode = 0x4Cu;
constexpr std::uint32_t kSideSetups = 0x09AFB8A0u;
constexpr std::uint32_t kSideSetupSize = 0x1060u;
constexpr std::uint32_t kSideCharacter = 0x1058u;

const char *env(const char *name) {
    const char *text = std::getenv(name);
    return text != nullptr && *text != '\0' ? text : nullptr;
}

bool user_ram(std::uint32_t address) {
    const std::uint32_t a = address & 0x1FFFFFFFu;
    return a >= 0x08800000u && a < 0x0A000000u - 0x1000u;
}

// --- Settings --------------------------------------------------------------

std::string read_setting(const char *key) {
    try {
        const std::filesystem::path dir = portablekit::install::user_data_directory();
        if (!std::filesystem::exists(dir / portablekit::install::kSettingsFile)) return {};
        const portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
        if (const auto found = entries.find(key); found != entries.end()) return found->second;
    } catch (const std::exception &e) {
        std::cerr << "[discord] settings.ini not read for " << key << ": " << e.what() << "\n";
    }
    return {};
}

// What the connection's thread reads is made once and never destroyed: the
// thread runs until the process ends, after the game's statics are gone.
const std::string &application_id() {
    static const std::string *const id = new std::string([] {
        if (const char *forced = env("D012_DISCORD_APP_ID"); forced != nullptr) return std::string(forced);
        if (std::string set = read_setting(kApplicationKey); !set.empty()) return set;
        return std::string(rules::kApplicationId);
    }());
    return *id;
}

std::atomic<bool> &show_setting() {
    static std::atomic<bool> value{[] {
        const std::string text = read_setting(kShowKey);
        return !(text == "0" || text == "off" || text == "false");
    }()};
    return value;
}

std::atomic<bool> g_retry_now{false};

void save_show(bool on) {
    show_setting().store(on);
    g_retry_now.store(true);
    try {
        const std::filesystem::path dir = portablekit::install::user_data_directory();
        portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
        entries[kShowKey] = on ? "1" : "0";
        portablekit::install::write_settings_file(dir, entries);
    } catch (const std::exception &e) {
        std::cerr << "[discord] settings.ini not written for " << kShowKey << ": " << e.what() << "\n";
    }
}

// Whether this run shows a status now.
bool showing() {
    if (application_id().empty()) return false;
    if (const char *forced = env("D012_DISCORD"); forced != nullptr) return std::strcmp(forced, "0") != 0;
    if (env("D012_INPUT_SCRIPT") != nullptr) return false;
    return show_setting().load();
}

// --- What the menu shows ---------------------------------------------------

enum class Status { Off, Waiting, Showing, Refused };

// Between the game's thread (the scene), the connection's thread (the status)
// and the menu. Never destroyed, as application_id().
struct Shared {
    std::mutex mutex;
    rules::Scene scene;
    Status status = Status::Off;
    std::string refusal;
};

Shared &shared() {
    static Shared *const s = new Shared;
    return *s;
}

void set_status(Status status, std::string refusal = {}) {
    Shared &s = shared();
    std::lock_guard lock(s.mutex);
    s.status = status;
    s.refusal = std::move(refusal);
}

rules::Scene scene() {
    Shared &s = shared();
    std::lock_guard lock(s.mutex);
    return s.scene;
}

// --- The connection --------------------------------------------------------

class Connection {
public:
    Connection() = default;
    Connection(const Connection &) = delete;
    Connection &operator=(const Connection &) = delete;
    ~Connection() { close(); }

#if defined(_WIN32)
    bool open(const std::string &path) {
        close();
        const std::wstring wide(path.begin(), path.end());  // the paths are ASCII
        handle_ = CreateFileW(wide.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
        return handle_ != INVALID_HANDLE_VALUE;
    }
    void close() {
        if (handle_ != INVALID_HANDLE_VALUE) CloseHandle(handle_);
        handle_ = INVALID_HANDLE_VALUE;
    }
    [[nodiscard]] bool is_open() const { return handle_ != INVALID_HANDLE_VALUE; }
    bool write_all(const std::string &bytes) {
        std::size_t at = 0;
        while (at < bytes.size()) {
            DWORD wrote = 0;
            if (!WriteFile(handle_, bytes.data() + at, static_cast<DWORD>(bytes.size() - at), &wrote, nullptr) ||
                wrote == 0)
                return false;
            at += wrote;
        }
        return true;
    }
    // Adds what has come in to `into`, waiting up to `wait` for the first
    // bytes. False when the other end has gone.
    bool read_some(std::string &into, std::chrono::milliseconds wait) {
        const Clock::time_point until = Clock::now() + wait;
        bool came = false;
        for (;;) {
            DWORD available = 0;
            if (!PeekNamedPipe(handle_, nullptr, 0, nullptr, &available, nullptr)) return false;
            if (available > 0) {
                char chunk[4096];
                DWORD got = 0;
                if (!ReadFile(handle_, chunk, std::min<DWORD>(available, sizeof chunk), &got, nullptr)) return false;
                into.append(chunk, got);
                came = true;
                continue;  // and whatever else is waiting
            }
            if (came || Clock::now() >= until) return true;
            Sleep(20);
        }
    }

private:
    HANDLE handle_ = INVALID_HANDLE_VALUE;
#else
    bool open(const std::string &path) {
        close();
        sockaddr_un address{};
        if (path.size() >= sizeof address.sun_path) return false;
        address.sun_family = AF_UNIX;
        std::memcpy(address.sun_path, path.c_str(), path.size() + 1);
        fd_ = socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd_ < 0) return false;
        if (connect(fd_, reinterpret_cast<const sockaddr *>(&address), sizeof address) != 0) {
            close();
            return false;
        }
        return true;
    }
    void close() {
        if (fd_ >= 0) ::close(fd_);
        fd_ = -1;
    }
    [[nodiscard]] bool is_open() const { return fd_ >= 0; }
    bool write_all(const std::string &bytes) {
        std::size_t at = 0;
        while (at < bytes.size()) {
            const ssize_t wrote = send(fd_, bytes.data() + at, bytes.size() - at, MSG_NOSIGNAL);
            if (wrote < 0 && errno == EINTR) continue;
            if (wrote <= 0) return false;
            at += static_cast<std::size_t>(wrote);
        }
        return true;
    }
    bool read_some(std::string &into, std::chrono::milliseconds wait) {
        pollfd p{fd_, POLLIN, 0};
        const int ready = poll(&p, 1, static_cast<int>(wait.count()));
        if (ready < 0) return errno == EINTR;
        if (ready == 0) return true;
        for (;;) {
            char chunk[4096];
            const ssize_t got = recv(fd_, chunk, sizeof chunk, MSG_DONTWAIT);
            if (got > 0) {
                into.append(chunk, static_cast<std::size_t>(got));
                continue;
            }
            if (got == 0) return false;  // closed
            return errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR;
        }
    }

private:
    int fd_ = -1;
#endif
};

// Where Discord may be listening, in the order Discord's own clients try.
std::vector<std::string> places() {
    if (const char *forced = env("D012_DISCORD_IPC"); forced != nullptr) return {forced};
    std::vector<std::string> out;
#if defined(_WIN32)
    for (int n = 0; n < 10; ++n) out.push_back("\\\\?\\pipe\\discord-ipc-" + std::to_string(n));
#else
    std::vector<std::string> folders;
    for (const char *name : {"XDG_RUNTIME_DIR", "TMPDIR", "TMP", "TEMP"})
        if (const char *dir = env(name); dir != nullptr) folders.emplace_back(dir);
    folders.emplace_back("/tmp");
    for (const std::string &folder : folders)
        for (const char *sub : {"", "/app/com.discordapp.Discord", "/snap.discord", "/.flatpak/dev.vencord.Vesktop/xdg-run"})
            for (int n = 0; n < 10; ++n) {
                std::string path = folder + sub + "/discord-ipc-" + std::to_string(n);
                if (std::find(out.begin(), out.end(), path) == out.end()) out.push_back(std::move(path));
            }
#endif
    return out;
}

std::uint32_t process_id() {
#if defined(_WIN32)
    return static_cast<std::uint32_t>(GetCurrentProcessId());
#else
    return static_cast<std::uint32_t>(getpid());
#endif
}

class Link {
public:
    // The thread's loop: never returns.
    void run() {
        for (bool first = true;; first = false) {
            if (!first) std::this_thread::sleep_for(1s);
            if (!showing()) {
                if (connection_.is_open()) {
                    std::cout << "[discord] status off" << std::endl;
                    drop();
                }
                set_status(Status::Off);
                continue;
            }
            const Clock::time_point now = Clock::now();
            if (!connection_.is_open()) {
                if (g_retry_now.exchange(false)) next_try_ = now;
                if (now < next_try_) continue;
                next_try_ = now + kRetry;
                if (!connect()) continue;
            }
            if (!receive()) continue;
            const rules::Activity wanted = rules::describe(scene());
            if (sent_ && *sent_ == wanted) continue;
            if (sent_ && now - last_sent_ < kBetweenUpdates) continue;
            send(wanted);
        }
    }

private:
    bool connect() {
        bool any = false;
        for (const std::string &place : places()) {
            if (!connection_.open(place)) continue;
            any = true;
            inbox_.clear();
            if (!connection_.write_all(rules::encode(rules::Op::Handshake, rules::handshake(application_id())))) {
                connection_.close();
                continue;
            }
            // Discord answers READY, or closes with its reason.
            const Clock::time_point until = Clock::now() + kHandshakeWait;
            std::optional<rules::Message> answer;
            while (!answer && Clock::now() < until) {
                const auto left = std::chrono::duration_cast<std::chrono::milliseconds>(until - Clock::now());
                if (!connection_.read_some(inbox_, std::max(left, 1ms))) break;
                answer = rules::take_message(inbox_);
                if (rules::broken(inbox_)) break;
            }
            const rules::Answer said = answer ? rules::read_answer(answer->json) : rules::Answer{};
            if (answer && answer->op == rules::Op::Frame && said.event == "READY") {
                std::cout << "[discord] connected (" << place << ")" << std::endl;
                said_not_open_ = false;
                accepted_ = false;
                sent_.reset();
                set_status(Status::Waiting);
                return true;
            }
            connection_.close();
            if (answer && answer->op == rules::Op::Close) {
                std::cout << "[discord] Discord refused the connection: "
                          << (said.error.empty() ? answer->json : said.error) << std::endl;
                next_try_ = Clock::now() + kRetryRefused;
                set_status(Status::Refused, said.error);
                return false;
            }
            std::cout << "[discord] no answer at " << place << std::endl;
        }
        if (!any && !said_not_open_) {
            std::cout << "[discord] Discord is not open; trying again every "
                      << std::chrono::duration_cast<std::chrono::seconds>(kRetry).count() << " seconds" << std::endl;
            said_not_open_ = true;
        }
        set_status(Status::Waiting);
        return false;
    }

    // Takes Discord's messages; false (and closed) when the connection went.
    bool receive() {
        if (!connection_.read_some(inbox_, 0ms)) return lost("Discord closed the connection");
        while (std::optional<rules::Message> message = rules::take_message(inbox_)) {
            switch (message->op) {
            case rules::Op::Ping:
                if (!connection_.write_all(rules::encode(rules::Op::Pong, message->json)))
                    return lost("the connection broke");
                break;
            case rules::Op::Close:
                return lost("Discord closed the connection: " + rules::read_answer(message->json).error);
            case rules::Op::Frame: {
                const rules::Answer said = rules::read_answer(message->json);
                if (said.event == "ERROR") {
                    std::cout << "[discord] Discord answered: " << said.error << std::endl;
                } else if (said.command == "SET_ACTIVITY" && !accepted_) {
                    std::cout << "[discord] Discord took the status" << std::endl;
                    accepted_ = true;
                }
                break;
            }
            default:
                break;
            }
        }
        if (rules::broken(inbox_)) return lost("Discord sent what is not its format");
        return true;
    }

    void send(const rules::Activity &activity) {
        const std::string json = rules::set_activity(&activity, process_id(), started_, ++nonce_);
        if (!connection_.write_all(rules::encode(rules::Op::Frame, json))) {
            lost("the connection broke");
            return;
        }
        sent_ = activity;
        last_sent_ = Clock::now();
        std::cout << "[discord] status: " << activity.details << (activity.state.empty() ? "" : " / ")
                  << activity.state << std::endl;
        set_status(Status::Showing);
    }

    bool lost(const std::string &why) {
        std::cout << "[discord] " << why << std::endl;
        drop();
        next_try_ = Clock::now() + kRetry;
        set_status(Status::Waiting);
        return false;
    }

    void drop() {
        connection_.close();
        inbox_.clear();
        sent_.reset();
    }

    Connection connection_;
    std::string inbox_;
    std::optional<rules::Activity> sent_;
    Clock::time_point last_sent_{};
    Clock::time_point next_try_{};
    std::uint64_t nonce_ = 0;
    bool said_not_open_ = false;
    bool accepted_ = false;  // Discord answered a SET_ACTIVITY on this connection
    const std::int64_t started_ = static_cast<std::int64_t>(std::time(nullptr));
};

// --- The game's state ------------------------------------------------------

bool battle_resident(psprecomp::GuestMemory &memory) {
    return memory.load32(kSetAiMode) == 0x27BDFFF0u && memory.load32(kSetAiMode + 4u) == 0x2CA60006u;
}

void sample(psprecomp::GuestMemory &memory) {
    rules::Scene s;
    s.where = rules::Scene::Where::Menus;
    if (battle_resident(memory)) {
        const std::uint32_t field = memory.load32(kBattleManager);
        if (user_ram(field)) {
            s.where = rules::Scene::Where::Field;
            s.mode = memory.load32(field + kFieldMode);
            for (std::uint32_t side = 0; side < 2u; ++side)
                s.fighters[side] =
                    static_cast<std::int8_t>(memory.load8(kSideSetups + side * kSideSetupSize + kSideCharacter));
        }
    }
    static std::optional<rules::Scene> last;
    if (!last || last->where != s.where || last->mode != s.mode) {
        if (s.where == rules::Scene::Where::Field)
            std::cout << "[discord] FIELD mode " << s.mode << ", characters " << s.fighters[0] << " and "
                      << s.fighters[1] << std::endl;
    }
    last = s;
    Shared &shared_state = shared();
    std::lock_guard lock(shared_state.mutex);
    shared_state.scene = s;
}

#if defined(PORTABLEKIT_HAS_RENDERER)
// --- The System page -------------------------------------------------------

void system_rows() {
    namespace ui = portablekit::ui;
    ui::section("Discord");
    ui::RowOptions show;
    show.description = "Shows what you are playing on your Discord profile while Discord is open on this PC.";
    bool on = show_setting().load();
    if (application_id().empty()) {
        show.disabled = true;
        show.note = "Not in this build";
        on = false;
    } else if (const char *forced = env("D012_DISCORD"); forced != nullptr) {
        show.disabled = true;
        show.note = "Set by D012_DISCORD";
        on = std::strcmp(forced, "0") != 0;
    }
    if (ui::toggle_row("Show on Discord", on, show)) save_show(!on);
    if (!on || application_id().empty()) return;
    Status status;
    std::string refusal;
    {
        Shared &s = shared();
        std::lock_guard lock(s.mutex);
        status = s.status;
        refusal = s.refusal;
    }
    switch (status) {
    case Status::Off:
    case Status::Waiting:
        ui::info_row("Status", "Discord is not open");
        break;
    case Status::Showing:
        ui::info_row("Status", "Showing");
        break;
    case Status::Refused:
        ui::info_row("Status", refusal.empty() ? std::string("Discord refused it") : "Discord refused it: " + refusal);
        break;
    }
}
#endif

} // namespace

void install(portablekit::HleRegistrar &hle) {
    static bool installed = false;
    if (installed) return;
    installed = true;
#if defined(PORTABLEKIT_HAS_RENDERER)
    portablekit::ui::add_system_rows(&system_rows);
#endif
    if (application_id().empty()) return;
    const portablekit::HleFunction previous = hle.bound_function("sceCtrl", kSceCtrlReadBufferPositive);
    if (!previous) {
        std::cerr << "[discord] sceCtrlReadBufferPositive is not bound; no Discord status\n";
        return;
    }
    hle.bind("sceCtrl", kSceCtrlReadBufferPositive, [previous](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        previous(rt, ctx);
        static std::uint32_t reads = 0;
        if (++reads % kSampleEvery == 0u) sample(rt.memory());
    });
    try {
        // Never destroyed (see application_id()): the process's end closes
        // the connection, which clears the status.
        std::thread([] { (new Link)->run(); }).detach();
    } catch (const std::exception &e) {
        std::cout << "[discord] the status could not start: " << e.what() << std::endl;
    }
}

} // namespace d012::discord
