#pragma once

// What the update check (d012_updates.cpp) reads from GitHub, and how it
// compares versions: no network here, so d012_tests checks it.
// MIT licence; see port/README.md.

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace d012::update_rules {

// Where the newest release is described: GitHub's "latest release", which is
// the newest one not marked as a pre-release, so a pre-release (a test build)
// is never offered.
inline constexpr const char *kLatestRelease = "https://api.github.com/repos/VCKing101/Dissidia012-pc/releases/latest";
// Every release page starts with this; another address in the answer is not
// opened.
inline constexpr const char *kReleasePages = "https://github.com/VCKing101/Dissidia012-pc/releases/";
// Every file of a release is downloaded from under this address; one anywhere
// else is left out of the release as it is read.
inline constexpr const char *kReleaseDownloads = "https://github.com/VCKing101/Dissidia012-pc/releases/download/";

// A release's list of its files' SHA-256 hashes, and that list's signature
// (tools/port/release_signing.sh).
inline constexpr const char *kSumsFile = "SHA256SUMS";
inline constexpr const char *kSignatureFile = "SHA256SUMS.sig";

struct Asset {
    std::string name;
    std::string url;
    std::uint64_t size = 0;
};

struct Release {
    std::string version;  // "0.5.5", from the tag "v0.5.5"
    std::string page;     // its page on GitHub
    std::vector<Asset> assets;
};

// The release in GitHub's answer for kLatestRelease (JSON). Nothing, with
// `error` set, when the answer is not one: not JSON, a draft or a
// pre-release, a tag that is not a version, or a page elsewhere.
[[nodiscard]] std::optional<Release> parse_release(std::string_view json, std::string &error);

// "Dissidia012-PC-Setup-0.5.5.exe"
[[nodiscard]] std::string setup_file_name(std::string_view version);
// "Dissidia012-0.5.5-Android-arm64.apk"
[[nodiscard]] std::string android_file_name(std::string_view version);

// The three files that install a release: the program for one platform (the
// Windows setup, the Android app), the list of hashes and its signature.
struct UpdateFiles {
    Asset program;
    Asset sums;
    Asset signature;
};

// Nothing when the release lacks one of them, or one has a size it cannot
// have: an older release, made before releases were signed, is installed from
// its page.
[[nodiscard]] std::optional<UpdateFiles> windows_update(const Release &release);
[[nodiscard]] std::optional<UpdateFiles> android_update(const Release &release);

// Whether `signature` (64 bytes) is the signature of `sums`, by the key written
// as 64 hexadecimal digits in `public_key`.
[[nodiscard]] bool sums_signed(std::string_view sums, std::string_view signature, std::string_view public_key);

// The SHA-256 (64 lowercase hexadecimal digits) that `sums` lists for the file
// `name`; empty when it lists none. `sums` is in sha256sum's format.
[[nodiscard]] std::string listed_hash(std::string_view sums, std::string_view name);

// Whether `text` is a version as tools/port/release-version.txt writes one:
// "1.2.3", or "1.2.3-rc.1" for a test build.
[[nodiscard]] bool is_version(std::string_view text);

// Negative, zero or positive as version `a` is older than, the same as or
// newer than `b`. A test build is older than the release of its number
// ("0.6.0-rc.1" < "0.6.0"); test builds compare by their suffix's text.
[[nodiscard]] int compare_versions(std::string_view a, std::string_view b);

} // namespace d012::update_rules
