#pragma once

// What the update check (d012_updates.cpp) reads from GitHub, and how it
// compares versions: no network here, so d012_tests checks it.
// MIT licence; see port/README.md.

#include <optional>
#include <string>
#include <string_view>

namespace d012::update_rules {

// Where the newest release is described: GitHub's "latest release", which is
// the newest one not marked as a pre-release, so a pre-release (a test build)
// is never offered.
inline constexpr const char *kLatestRelease = "https://api.github.com/repos/VCKing101/Dissidia012-pc/releases/latest";
// Every release page starts with this; another address in the answer is not
// opened.
inline constexpr const char *kReleasePages = "https://github.com/VCKing101/Dissidia012-pc/releases/";

struct Release {
    std::string version;  // "0.5.5", from the tag "v0.5.5"
    std::string page;     // its page on GitHub
};

// The release in GitHub's answer for kLatestRelease (JSON). Nothing, with
// `error` set, when the answer is not one: not JSON, a draft or a
// pre-release, a tag that is not a version, or a page elsewhere.
[[nodiscard]] std::optional<Release> parse_release(std::string_view json, std::string &error);

// Whether `text` is a version as tools/port/release-version.txt writes one:
// "1.2.3", or "1.2.3-rc.1" for a test build.
[[nodiscard]] bool is_version(std::string_view text);

// Negative, zero or positive as version `a` is older than, the same as or
// newer than `b`. A test build is older than the release of its number
// ("0.6.0-rc.1" < "0.6.0"); test builds compare by their suffix's text.
[[nodiscard]] int compare_versions(std::string_view a, std::string_view b);

} // namespace d012::update_rules
