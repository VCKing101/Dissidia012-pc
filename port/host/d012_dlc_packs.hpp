#pragma once

// DLC packs (d012_dlc_packs.cpp): <data dir>/dlc/<any folder>/ holds one pack's
// .edat files, as DissDlcToolkit writes them. The game finds them as if they
// were on the memory stick, and packs built for the same slots do not replace
// each other. Off with D012_NO_DLC_PACKS=1; D012_DLC_PACKS_DIR names another
// folder. The guide is docs/port/dlc-packs.md.

#include <cstdint>
#include <optional>
#include <vector>

namespace portablekit {
class HleRegistrar;
}

namespace d012::dlc_packs {

void install(portablekit::HleRegistrar &hle);

// The bytes a pack serves the game at the DLC name `crc`, as the game reads
// them (a rewritten table's new bytes); nothing when no pack serves that name.
[[nodiscard]] std::optional<std::vector<std::uint8_t>> served_file(std::uint32_t crc);

} // namespace d012::dlc_packs
