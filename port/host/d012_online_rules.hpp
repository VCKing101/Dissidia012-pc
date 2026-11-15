#pragma once

// Play online's address file (d012_online.hpp), read without PortableKit, so
// the port's tests check it (d012_tests.cpp). MIT licence; see port/README.md.

#include <string>

namespace d012::online_rules {

// What a player is told when the file names no server on purpose.
inline constexpr const char *kNoServerYet = "There is no public server yet.";

// The server's address in the file's text ({"address": "host" or
// "host:port"}), or empty with `note` saying why for the player: no server on
// purpose (kNoServerYet), or a file that cannot be read as one.
[[nodiscard]] std::string address_in(const std::string &text, std::string &note);

} // namespace d012::online_rules
