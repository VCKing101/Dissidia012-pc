#pragma once

// What the port's analog walk (d012_decomp_pad.cpp) tells the camera: which
// walker the stick steers. See docs/port/analog-movement.md. MIT licence;
// see port/README.md.

#include "../decomp/pad_types.h"

#include <cstdint>

namespace d012::decomp {

// False with D012_NO_SMOOTH_STICK=1.
[[nodiscard]] bool smooth_stick_on();

// Whether the last walk input of `walker` (battle 0x08B38A0C) took the
// port's stick: its pad walks on the stick and it is not a battle fighter
// (unless D012_SMOOTH_STICK_BATTLE=1). `stick` gets that stick, magnitude 0
// when it was inside the dead zone.
[[nodiscard]] bool analog_walker(std::uint32_t walker, PadAnalog *stick);

} // namespace d012::decomp
