#pragma once

// Downloadable content (d012_dlc.cpp): a line in the log at start saying what
// the game will find in ms0:/PSP/GAME/<disc id> (ULUS10566, or ULES01505 on
// the European disc), and a warning for DLC in a folder the game never reads.

#include <filesystem>

namespace portablekit {
class HleRegistrar;
}

namespace d012::dlc {

// The folder the game reads DLC from, ms0:/PSP/GAME/<disc id>; empty when
// there is no memory stick folder.
[[nodiscard]] std::filesystem::path folder();

void install(portablekit::HleRegistrar &hle);

} // namespace d012::dlc
