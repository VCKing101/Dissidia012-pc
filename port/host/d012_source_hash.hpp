#pragma once

// A SHA-256 of the port's own source, line endings aside (64 hex digits),
// made by the build (port/cmake/source_hash.cmake). Two copies with the same
// hash run the same program: online play compares it (d012_comm_battle.cpp).
// MIT licence; see port/README.md.

namespace d012 {

[[nodiscard]] const char *source_hash();

} // namespace d012
