// The update check's reading of GitHub's answer and its version order
// (d012_update_rules.hpp). MIT licence; see port/README.md.

#include "d012_update_rules.hpp"

#include "d012_json.hpp"

#include <array>
#include <cstdint>

namespace d012::update_rules {
namespace {

struct Parts {
    std::array<std::uint64_t, 3> numbers{};
    std::string_view suffix;  // after the "-", empty for a release
};

std::optional<Parts> parts(std::string_view text) {
    Parts out;
    std::size_t at = 0;
    for (int i = 0; i < 3; ++i) {
        if (i > 0) {
            if (at >= text.size() || text[at] != '.') return std::nullopt;
            ++at;
        }
        const std::size_t start = at;
        std::uint64_t number = 0;
        while (at < text.size() && text[at] >= '0' && text[at] <= '9' && at - start < 9)
            number = number * 10u + static_cast<std::uint64_t>(text[at++] - '0');
        if (at == start) return std::nullopt;
        out.numbers[static_cast<std::size_t>(i)] = number;
    }
    if (at == text.size()) return out;
    if (text[at] != '-' || at + 1 == text.size()) return std::nullopt;
    out.suffix = text.substr(at + 1);
    for (const char c : out.suffix)
        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.')) return std::nullopt;
    return out;
}

} // namespace

bool is_version(std::string_view text) { return parts(text).has_value(); }

int compare_versions(std::string_view a, std::string_view b) {
    const std::optional<Parts> x = parts(a);
    const std::optional<Parts> y = parts(b);
    if (!x || !y) return a.compare(b);
    for (std::size_t i = 0; i < 3; ++i)
        if (x->numbers[i] != y->numbers[i]) return x->numbers[i] < y->numbers[i] ? -1 : 1;
    if (x->suffix.empty() || y->suffix.empty()) return x->suffix.empty() == y->suffix.empty() ? 0 : x->suffix.empty() ? 1 : -1;
    const int order = x->suffix.compare(y->suffix);
    return order < 0 ? -1 : order > 0 ? 1 : 0;
}

std::optional<Release> parse_release(std::string_view text, std::string &error) {
    const std::optional<json::Value> value = json::parse(text, error);
    if (!value) return std::nullopt;
    const auto flag = [&](const char *name) {
        const json::Value *member = value->find(name);
        return member != nullptr && member->is(json::Value::Kind::Bool) && member->boolean;
    };
    const json::Value *tag = value->find("tag_name");
    const json::Value *page = value->find("html_url");
    if (tag == nullptr || !tag->is(json::Value::Kind::String) || page == nullptr ||
        !page->is(json::Value::Kind::String)) {
        error = "the answer names no release";
        return std::nullopt;
    }
    if (flag("draft") || flag("prerelease")) {
        error = "the answer is a draft or a pre-release";
        return std::nullopt;
    }
    std::string_view version = tag->text;
    if (!version.empty() && version.front() == 'v') version.remove_prefix(1);
    if (!is_version(version)) {
        error = "the release's tag is not a version: " + tag->text.substr(0, 40);
        return std::nullopt;
    }
    if (page->text.rfind(kReleasePages, 0) != 0) {
        error = "the release's page is not on this project's GitHub";
        return std::nullopt;
    }
    return Release{std::string(version), page->text};
}

} // namespace d012::update_rules
