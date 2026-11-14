#pragma once

// The portable layout (d012_portable.cpp).
// MIT licence; see port/README.md.

#include <filesystem>

namespace d012::portable {

// The file what the program prints goes to, when the layout chose one
// (<data>/logs/D012Native.log, or D012_LOG_FILE); empty when the output goes
// where it was going.
[[nodiscard]] std::filesystem::path log_file();

} // namespace d012::portable
