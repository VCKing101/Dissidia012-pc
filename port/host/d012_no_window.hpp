#pragma once

// MIT licence; see port/README.md.

#include <string>

namespace d012 {

// PortableKit's GameProfile::renderer_unavailable: the game's window cannot be
// made, for the reason `why` (d012_no_window.cpp).
void renderer_unavailable(const std::string &why);

} // namespace d012
