#pragma once

// One input record per game frame (docs/port/online-play.md, section 10): all
// the input the game and the port's own features act on, taken once at the
// game's pad read. The camera, local versus and the BGM selector read it
// instead of the live pads, so that what a frame does depends only on its
// record: the seam for netplay, and for replaying a run.
// MIT licence; see port/README.md.

#include <cstdint>

namespace portablekit {
class HleRegistrar;
}

namespace d012::input {

struct Pad {
    bool connected = false;
    std::uint32_t buttons = 0;  // SceCtrlButtons bits
    std::uint32_t extra = 0;    // hle_extension::kExtra* (L2, R2, L3, R3)
    std::uint8_t lx = 0x80u;
    std::uint8_t ly = 0x80u;
    std::uint8_t rx = 0x80u;
    std::uint8_t ry = 0x80u;
    // A linked battle's only (d012_camera.hpp, send_mouse): the mouse's turn
    // of the free camera this frame, in 1/100 degree, and its vertical motion
    // in 1/4 degree.
    std::int16_t mouse_yaw = 0;
    std::int8_t mouse_pitch = 0;
};

struct Frame {
    std::uint64_t read = 0;  // the game's pad reads before this one
    std::uint8_t dialog = 0; // the message dialog's answer this frame: 0 none, 1 OK or Yes, 2 No, 3 Back
    std::uint8_t save = 0;   // 1: the save-data dialog's answer reaches the game this frame
    Pad player1;             // the game's own pad: buttons and sticks as its read got them
    Pad player2;             // the second gamepad (hle_extension::second_pad), for local versus
};

// The record of the game's latest pad read.
[[nodiscard]] const Frame &current();

// Wraps sceCtrlReadBufferPositive innermost, so it must be installed before
// any other wrapper of it. Off unless one of these is set:
// - D012_INPUT_RECORD=FILE: appends every frame's record to FILE;
// - D012_INPUT_REPLAY=FILE: each frame's record comes from FILE, read by read,
//   instead of the pads (live again after its last record);
// - D012_NET (d012_net.hpp): each frame's record comes from both linked copies,
//   this copy's player on its first pad, or on its second gamepad with
//   D012_NET_PAD=2 (two copies on one machine, a controller each).
// With either, every sample of the read is rewritten from the record, so the
// game sees exactly what the record holds. Without them the pads go to the
// game as before and the record only mirrors them.
void install(portablekit::HleRegistrar &hle);

// Adds the input record's place (the read count, the frame, the dialogs'
// answers) to save states (portablekit snapshot).
void add_to_snapshots();

} // namespace d012::input
