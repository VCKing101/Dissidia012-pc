#pragma once

// The cheats the port offers on its menu's Cheats page (GameProfile::cheats).

#include "profile.hpp"

#include <cstdint>
#include <span>

namespace d012 {

[[nodiscard]] std::span<const portablekit::BuiltinCheat> builtin_cheats();

// The AP row's amount, or 0 when it is off (or Use cheats is). Read by the
// battle's rewards (d012_ap_cheat.cpp).
[[nodiscard]] std::uint32_t ap_after_battle();

// Whether the 10 Skills row is on (and Use cheats). Read by the Skill pages
// (d012_skill_pages.cpp).
[[nodiscard]] bool skill_pages_on();

} // namespace d012
