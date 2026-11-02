// One input record per game frame (d012_input.hpp).
//
// The file (D012_INPUT_RECORD, D012_INPUT_REPLAY): "D012INPT", u32 version
// (2), then one 48-byte record per pad read: u64 read number, u8 the message
// dialog's answer (0 none, 1 OK or Yes, 2 No, 3 Back), u8 the save-data
// dialog's release (below), 6 spare, then per
// player u8 connected, u8 lx, ly, rx, ry, 3 spare, u32 buttons, u32 extra.
// Little endian.
//
// The message dialog (sceUtilityMsgDialog*) takes its answer from the player
// through PortableKit's interface, on whichever frame they click: input that
// never passes the pad read. While the record is in use the port runs the
// dialog itself, with PortableKit's life cycle (utility_dialog.hpp) and its
// window: an answer waits for the next pad read, goes into that frame's
// record, and is handed to the game at the dialog's next poll. A replay hands
// it over on the same frame without drawing the dialog.
//
// The save-data dialogs that ask (a list of saves to load, save or delete;
// saving or deleting one save) are PortableKit's and keep their state to
// themselves, so the port gates them instead: once one is VISIBLE, its polls
// report VISIBLE without reaching PortableKit until a frame's record
// releases it. Recording, the frame after the player's answer closed the
// window releases it, and the choice the game got (a save's name, or
// "cancel") goes to FILE.saves, a line each. A replay hands those choices
// to PortableKit as D012_SAVEDATA_ANSWERS and releases each dialog on its
// recorded frame.
//
// In netplay (D012_NET, d012_net.hpp) each frame's record is put together from
// both copies' input: player 1's pad and dialog answers from player 1's copy,
// player 2's pad from the other.
//
// MIT licence; see port/README.md.

#include "d012_input.hpp"
#include "d012_net.hpp"
#include "d012_sync.hpp"

#include "hle/hle_common.hpp"
#include "hle/utility_dialog.hpp"
#include "portablekit/hle_extension.hpp"

#include "psprecomp/common.hpp"
#include "psprecomp/runtime.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "ui/message_dialog.hpp"
#include "ui/save_list_dialog.hpp"
#endif

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace d012::input {
namespace {

namespace ext = portablekit::hle_extension;
namespace status = portablekit::dialog_status;
namespace common = portablekit::dialog_common;

constexpr std::uint32_t kSceCtrlReadBufferPositive = 0x1F803938u;
constexpr std::uint32_t kMsgDialogInitStart = 0x2AD8E239u;
constexpr std::uint32_t kMsgDialogShutdownStart = 0x67AF3428u;
constexpr std::uint32_t kMsgDialogUpdate = 0x95FC253Bu;
constexpr std::uint32_t kMsgDialogGetStatus = 0x9A1C91D7u;
constexpr std::uint32_t kMsgDialogAbort = 0x4928BD96u;
constexpr std::uint32_t kSavedataInitStart = 0x50C4CD57u;
constexpr std::uint32_t kSavedataShutdownStart = 0x9790B33Cu;
constexpr std::uint32_t kSavedataUpdate = 0xD4B95FFBu;
constexpr std::uint32_t kSavedataGetStatus = 0x8874DBE0u;
// SceUtilitySavedataParam, as PortableKit reads it (hle_savedata.cpp).
constexpr std::uint32_t kSaveMode = 0x30u;
constexpr std::uint32_t kSaveName = 0x4Cu;  // char[20]
constexpr std::uint32_t kSaveCancelled = 1u;
constexpr std::uint32_t kVersion = 2u;
constexpr std::size_t kRecordSize = 48u;

// SceUtilityMsgDialogParams, as PortableKit reads it (hle_utility.cpp).
constexpr std::uint32_t kMsgMode = 0x34u;  // 0: error code, 1: text
constexpr std::uint32_t kMsgErrorValue = 0x38u;
constexpr std::uint32_t kMsgText = 0x3Cu;
constexpr std::uint32_t kMsgOptions = 0x23Cu;
constexpr std::uint32_t kMsgButtonPressed = 0x240u;
constexpr std::uint32_t kMsgMinimumSize = 0x244u;
constexpr std::uint32_t kMsgOptionYesNo = 0x10u;
constexpr std::uint32_t kMsgOptionNoCancel = 0x80u;
constexpr std::uint32_t kMsgOptionDefaultNo = 0x100u;

struct Dialog {
    std::uint32_t params = 0;
    std::uint32_t status = status::kNone;
    unsigned serial = 0;
    std::uint8_t clicked = 0;  // the player's answer, waiting for the next pad read
    std::uint8_t answer = 0;   // the answer a frame's record brought, for the next poll
};

// The save-data dialog that asks, while the port gates it.
struct SaveAsk {
    bool gated = false;   // its polls stop at VISIBLE
    bool visible = false; // its first poll (INIT to VISIBLE) has passed
    std::uint32_t params = 0;
};

struct State {
    Frame frame;
    std::uint64_t reads = 0;
    std::FILE *record = nullptr;
    std::vector<Frame> replay;
    bool replaying = false;
    Dialog dialog;
    SaveAsk save;
    std::FILE *choices = nullptr;  // FILE.saves while recording
    bool second_pad = false;       // D012_NET_PAD=2: this copy's player plays on the second gamepad
    std::uint64_t ram_hash = 0;    // the netplay check's hash so far (below)
};

State &state() {
    static State s;
    return s;
}

bool in_use() {
    const State &s = state();
    return s.record != nullptr || s.replaying || net::active();
}

// Player 2's copy in netplay draws no message dialog, as a replay draws none:
// player 1 answers it, and the answer comes in the frame's record.
bool draws_dialogs() { return !state().replaying && (!net::active() || net::player1()); }

void put_pad(std::uint8_t *out, const Pad &p) {
    out[0] = p.connected ? 1u : 0u;
    out[1] = p.lx;
    out[2] = p.ly;
    out[3] = p.rx;
    out[4] = p.ry;
    std::memcpy(out + 8, &p.buttons, 4u);
    std::memcpy(out + 12, &p.extra, 4u);
}

Pad get_pad(const std::uint8_t *in) {
    Pad p;
    p.connected = in[0] != 0u;
    p.lx = in[1];
    p.ly = in[2];
    p.rx = in[3];
    p.ry = in[4];
    std::memcpy(&p.buttons, in + 8, 4u);
    std::memcpy(&p.extra, in + 12, 4u);
    return p;
}

void write_record(std::FILE *file, const Frame &f) {
    std::uint8_t out[kRecordSize]{};
    std::memcpy(out, &f.read, 8u);
    out[8] = f.dialog;
    out[9] = f.save;
    put_pad(out + 16, f.player1);
    put_pad(out + 32, f.player2);
    std::fwrite(out, 1u, kRecordSize, file);
}

std::vector<Frame> read_records(const char *path) {
    std::vector<Frame> frames;
    std::FILE *file = std::fopen(path, "rb");
    if (file == nullptr) {
        std::cerr << "[input] cannot read " << path << std::endl;
        return frames;
    }
    char magic[8]{};
    std::uint32_t version = 0;
    if (std::fread(magic, 1u, 8u, file) != 8u || std::memcmp(magic, "D012INPT", 8u) != 0 ||
        std::fread(&version, 4u, 1u, file) != 1u || version != kVersion) {
        std::cerr << "[input] " << path << " is not an input record of version " << kVersion << std::endl;
        std::fclose(file);
        return frames;
    }
    std::uint8_t in[kRecordSize];
    while (std::fread(in, 1u, kRecordSize, file) == kRecordSize) {
        Frame f;
        std::memcpy(&f.read, in, 8u);
        f.dialog = in[8];
        f.save = in[9];
        f.player1 = get_pad(in + 16);
        f.player2 = get_pad(in + 32);
        frames.push_back(f);
    }
    std::fclose(file);
    return frames;
}

// Player 1's pad as the read got it: its last sample, as the camera takes it.
Pad live_player1(psprecomp::GuestMemory &memory, std::uint32_t buffer, std::uint32_t count) {
    Pad p;
    p.connected = ext::first_pad_connected();
    p.extra = ext::extra_buttons();
    if (buffer == 0u) return p;
    const std::uint32_t last = buffer + (count - 1u) * 16u;
    p.buttons = memory.load32(last + 4u);
    p.lx = memory.load8(last + 8u);
    p.ly = memory.load8(last + 9u);
    p.rx = memory.load8(last + 10u);
    p.ry = memory.load8(last + 11u);
    return p;
}

Pad live_player2() {
    const ext::SecondPad pad = ext::second_pad();
    Pad p;
    p.connected = pad.connected;
    p.buttons = pad.buttons;
    p.extra = pad.extra;
    p.lx = pad.analog_x;
    p.ly = pad.analog_y;
    p.rx = pad.right_x;
    p.ry = pad.right_y;
    return p;
}

void close_window() {
#if defined(PORTABLEKIT_HAS_RENDERER)
    if (portablekit::ui::game_message_open()) portablekit::ui::close_game_message();
#endif
}

// One poll (Update or GetStatus): the status to report, then the next step,
// as PortableKit's own dialog does.
std::uint32_t poll(psprecomp::GuestMemory &memory) {
    Dialog &d = state().dialog;
    const std::uint32_t reported = d.status;
    switch (d.status) {
    case status::kInit: d.status = status::kVisible; break;
    case status::kVisible:
        if (d.answer != 0u) {
            if (memory.load32(d.params + common::kSizeOffset) >= kMsgMinimumSize)
                memory.store32(d.params + kMsgButtonPressed, d.answer);
            memory.store32(d.params + common::kResultOffset, 0u);
            std::cout << "[input] message dialog #" << d.serial << " answered " << static_cast<unsigned>(d.answer)
                      << " at read " << state().frame.read << std::endl;
            d.answer = 0u;
            close_window();
            d.status = status::kQuit;
        }
        break;
    case status::kFinished:
        d.status = status::kNone;
        d.params = 0u;
        break;
    default: break;
    }
    return reported;
}

void install_message_dialog(portablekit::HleRegistrar &hle) {
    using portablekit::kernel;
    if (!hle.bound_function("sceUtility", kMsgDialogInitStart)) {
        std::cerr << "[input] sceUtilityMsgDialogInitStart is not bound; dialogs answer outside the record\n";
        return;
    }
    hle.bind("sceUtility", kMsgDialogInitStart, [](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        psprecomp::GuestMemory &memory = rt.memory();
        Dialog &d = state().dialog;
        const std::uint32_t params = portablekit::arg(ctx, 0);
        if (d.status != status::kNone || params == 0u) {
            kernel().finish(ctx, portablekit::kErrorUtilityInvalidStatus);
            return;
        }
        const std::uint32_t size = memory.load32(params + common::kSizeOffset);
        const std::uint32_t options = size >= kMsgMinimumSize ? memory.load32(params + kMsgOptions) : 0u;
        std::string text = memory.load32(params + kMsgMode) == 0u
                               ? "An error has occurred.\n(" + psprecomp::hex32(memory.load32(params + kMsgErrorValue)) + ")"
                               : portablekit::read_cstring(memory, params + kMsgText, 512u);
        d = Dialog{params, status::kInit, d.serial + 1u};
        memory.store32(params + common::kResultOffset, 0u);
        std::cout << "[input] message dialog #" << d.serial << " opens at read " << state().reads << std::endl;
        if (!draws_dialogs()) {
            kernel().finish(ctx, 0u);
            return;
        }
#if defined(PORTABLEKIT_HAS_RENDERER)
        portablekit::ui::MessageRequest request;
        request.text = std::move(text);
        request.question = (options & kMsgOptionYesNo) != 0u;
        request.default_no = (options & kMsgOptionDefaultNo) != 0u;
        request.allow_back = (options & kMsgOptionNoCancel) == 0u;
        const unsigned serial = d.serial;
        portablekit::ui::open_game_message(std::move(request), [serial](portablekit::ui::MessageAnswer answer) {
            Dialog &now = state().dialog;
            if (now.serial != serial || now.status == status::kNone) return;
            now.clicked = answer == portablekit::ui::MessageAnswer::Yes  ? 1u
                          : answer == portablekit::ui::MessageAnswer::No ? 2u
                                                                         : 3u;
        });
#else
        (void)options;
        std::cerr << "[input] no window for message dialog #" << d.serial << "; it waits for an answer\n";
#endif
        kernel().finish(ctx, 0u);
    });
    hle.bind("sceUtility", kMsgDialogUpdate, [](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        (void)poll(rt.memory());
        portablekit::kernel().finish(ctx, 0u);
    });
    hle.bind("sceUtility", kMsgDialogGetStatus, [](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        portablekit::kernel().finish(ctx, poll(rt.memory()));
    });
    hle.bind("sceUtility", kMsgDialogShutdownStart, [](psprecomp::Runtime &, psprecomp::AllegrexContext &ctx) {
        Dialog &d = state().dialog;
        if (d.status == status::kNone) {
            portablekit::kernel().finish(ctx, portablekit::kErrorUtilityInvalidStatus);
            return;
        }
        close_window();
        d.clicked = d.answer = 0u;
        d.status = status::kFinished;
        portablekit::kernel().finish(ctx, 0u);
    });
    hle.bind("sceUtility", kMsgDialogAbort, [](psprecomp::Runtime &, psprecomp::AllegrexContext &ctx) {
        Dialog &d = state().dialog;
        if (d.status != status::kVisible && d.status != status::kInit) {
            portablekit::kernel().finish(ctx, portablekit::kErrorUtilityInvalidStatus);
            return;
        }
        close_window();
        d.clicked = d.answer = 0u;
        d.status = status::kQuit;
        portablekit::kernel().finish(ctx, 0u);
    });
}

bool asks(std::uint32_t mode) { return mode >= 3u && mode <= 7u; }  // SAVE, the three lists, DELETE

void install_save_dialog(portablekit::HleRegistrar &hle) {
    const portablekit::HleFunction init = hle.bound_function("sceUtility", kSavedataInitStart);
    const portablekit::HleFunction update = hle.bound_function("sceUtility", kSavedataUpdate);
    const portablekit::HleFunction get_status = hle.bound_function("sceUtility", kSavedataGetStatus);
    const portablekit::HleFunction shutdown = hle.bound_function("sceUtility", kSavedataShutdownStart);
    if (!init || !update || !get_status || !shutdown) {
        std::cerr << "[input] sceUtilitySavedata* is not bound; save-data dialogs answer outside the record\n";
        return;
    }
    hle.bind("sceUtility", kSavedataInitStart, [init](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        const std::uint32_t params = portablekit::arg(ctx, 0);
        const std::uint32_t mode = params != 0u ? rt.memory().load32(params + kSaveMode) : 0u;
        init(rt, ctx);
        SaveAsk &a = state().save;
        a = SaveAsk{};
        if (params != 0u && asks(mode)) {
            a = SaveAsk{true, false, params};
            std::cout << "[input] save-data dialog (mode " << mode << ") opens at read " << state().reads << std::endl;
        }
    });
    const auto gated = [](const SaveAsk &a) { return a.gated && a.visible; };
    hle.bind("sceUtility", kSavedataUpdate, [update, gated](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        SaveAsk &a = state().save;
        if (gated(a)) {
            portablekit::kernel().finish(ctx, 0u);
            return;
        }
        update(rt, ctx);
        if (a.gated) a.visible = true;
    });
    hle.bind("sceUtility", kSavedataGetStatus,
             [get_status, gated](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
                 SaveAsk &a = state().save;
                 if (gated(a)) {
                     portablekit::kernel().finish(ctx, status::kVisible);
                     return;
                 }
                 get_status(rt, ctx);
                 if (a.gated) a.visible = true;
             });
    hle.bind("sceUtility", kSavedataShutdownStart, [shutdown](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        State &s = state();
        if (s.save.params != 0u && s.choices != nullptr) {
            psprecomp::GuestMemory &memory = rt.memory();
            const bool cancelled = memory.load32(s.save.params + common::kResultOffset) == kSaveCancelled;
            const std::string name =
                cancelled ? "cancel" : portablekit::read_cstring(memory, s.save.params + kSaveName, 20u);
            std::fprintf(s.choices, "%s\n", name.c_str());
            std::fflush(s.choices);
        }
        s.save = SaveAsk{};
        shutdown(rt, ctx);
    });
}

// FILE.saves: the recorded choices, in order, as D012_SAVEDATA_ANSWERS.
void answer_saves_from(const std::string &path) {
    std::FILE *file = std::fopen(path.c_str(), "r");
    if (file == nullptr) return;
    std::string answers;
    char line[64];
    while (std::fgets(line, sizeof line, file) != nullptr) {
        std::string item(line);
        while (!item.empty() && (item.back() == '\n' || item.back() == '\r')) item.pop_back();
        if (item.empty()) continue;
        if (!answers.empty()) answers += ",";
        answers += item;
    }
    std::fclose(file);
    if (answers.empty()) return;
#if defined(_WIN32)
    _putenv_s("D012_SAVEDATA_ANSWERS", answers.c_str());
#else
    setenv("D012_SAVEDATA_ANSWERS", answers.c_str(), 1);
#endif
    std::cout << "[input] save-data dialogs answered " << answers << " (from " << path << ")" << std::endl;
}

} // namespace

const Frame &current() { return state().frame; }

void install(portablekit::HleRegistrar &hle) {
    static bool installed = false;
    if (installed) return;
    installed = true;
    State &s = state();
    if (net::start()) {
        const char *pad = std::getenv("D012_NET_PAD");
        s.second_pad = pad != nullptr && std::strcmp(pad, "2") == 0;
    }
    if (const char *path = std::getenv("D012_INPUT_REPLAY"); path != nullptr && *path != '\0') {
        s.replay = read_records(path);
        s.replaying = !s.replay.empty();
        if (s.replaying) {
            std::cout << "[input] replaying " << s.replay.size() << " frames of input from " << path << std::endl;
            answer_saves_from(std::string(path) + ".saves");
        }
    }
    if (const char *path = std::getenv("D012_INPUT_RECORD"); path != nullptr && *path != '\0') {
        s.record = std::fopen(path, "wb");
        if (s.record == nullptr) {
            std::cerr << "[input] cannot write " << path << std::endl;
        } else {
            std::fwrite("D012INPT", 1u, 8u, s.record);
            std::fwrite(&kVersion, 4u, 1u, s.record);
            std::cout << "[input] recording every frame's input to " << path << std::endl;
            s.choices = std::fopen((std::string(path) + ".saves").c_str(), "w");
        }
    }
    const portablekit::HleFunction previous = hle.bound_function("sceCtrl", kSceCtrlReadBufferPositive);
    if (!previous) {
        std::cerr << "[input] sceCtrlReadBufferPositive is not bound; no input record\n";
        return;
    }
    if (in_use()) {
        install_message_dialog(hle);
        install_save_dialog(hle);
    }
    hle.bind("sceCtrl", kSceCtrlReadBufferPositive, [previous](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        const std::uint32_t buffer = ctx.gpr[4];
        const std::uint32_t count = std::clamp<std::uint32_t>(ctx.gpr[5], 1u, 64u);
        previous(rt, ctx);
        State &s = state();
        psprecomp::GuestMemory &memory = rt.memory();
        Frame f;
        f.read = s.reads++;
        if (net::active()) {
            net::Local mine;
            mine.pad = s.second_pad ? live_player2() : live_player1(memory, buffer, count);
            if (s.dialog.status == status::kVisible) mine.dialog = s.dialog.clicked;
            s.dialog.clicked = 0u;
#if defined(PORTABLEKIT_HAS_RENDERER)
            if (s.save.gated && s.save.visible && !portablekit::ui::game_save_list_open()) mine.save = 1u;
#endif
            f = net::exchange(f.read, mine);
        } else if (s.replaying && f.read < s.replay.size()) {
            f = s.replay[f.read];
        } else {
            f.player1 = live_player1(memory, buffer, count);
            f.player2 = live_player2();
            if (s.dialog.status == status::kVisible) f.dialog = s.dialog.clicked;
            s.dialog.clicked = 0u;
#if defined(PORTABLEKIT_HAS_RENDERER)
            // The player answered: the window closed.
            if (s.save.gated && s.save.visible && !portablekit::ui::game_save_list_open()) f.save = 1u;
#endif
        }
        s.frame = f;
        if (!in_use()) return;
        if (f.dialog != 0u && s.dialog.status == status::kVisible) s.dialog.answer = f.dialog;
        if (f.save != 0u && s.save.gated) {
            s.save.gated = false;
            std::cout << "[input] save-data dialog released at read " << f.read << std::endl;
        }
        if (s.record != nullptr) {
            write_record(s.record, f);
            if (f.read % 64u == 0u) std::fflush(s.record);
        }
        // The game sees exactly the record: every sample of the read.
        for (std::uint32_t i = 0; buffer != 0u && i < count; ++i) {
            const std::uint32_t sample = buffer + i * 16u;
            memory.store32(sample + 4u, f.player1.buttons);
            memory.store8(sample + 8u, f.player1.lx);
            memory.store8(sample + 9u, f.player1.ly);
            memory.store8(sample + 10u, f.player1.rx);
            memory.store8(sample + 11u, f.player1.ry);
        }
        // Netplay's check that the copies agree: an eighth of a MiB of guest
        // RAM a read, all of it every 64 reads, compared with the other copy.
        if (net::active()) {
            const std::uint32_t pages = sync::page_count(memory);
            const std::uint32_t step = (pages + 63u) / 64u;
            const std::uint32_t first = static_cast<std::uint32_t>(f.read % 64u) * step;
            if (first < pages) {
                const std::uint64_t h = sync::hash_pages(memory, first, std::min(step, pages - first));
                s.ram_hash = (s.ram_hash ^ h) * 0x9E3779B97F4A7C15ull;
            }
            if (f.read % 64u == 63u) {
                net::check(f.read, s.ram_hash);
                s.ram_hash = 0u;
            }
        }
    });
}

} // namespace d012::input
