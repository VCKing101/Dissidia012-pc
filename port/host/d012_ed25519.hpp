#pragma once

// Checking an Ed25519 signature (RFC 8032), for the update's signed list of
// files (d012_update_rules.hpp). Only the check: the signing is done with
// OpenSSL when a release is made (tools/port/release_signing.sh).
// MIT licence; see port/README.md.

#include <array>
#include <cstdint>
#include <span>

namespace d012::ed25519 {

using PublicKey = std::array<std::uint8_t, 32>;
using Signature = std::array<std::uint8_t, 64>;

// Whether `signature` is `key`'s signature of `message`.
[[nodiscard]] bool verify(const PublicKey &key, std::span<const std::uint8_t> message, const Signature &signature);

} // namespace d012::ed25519
