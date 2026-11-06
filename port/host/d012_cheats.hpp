#pragma once

// The cheats the port offers on its menu's Cheats page (GameProfile::cheats).

#include "profile.hpp"

#include <span>

namespace d012 {

[[nodiscard]] std::span<const portablekit::BuiltinCheat> builtin_cheats();

} // namespace d012
