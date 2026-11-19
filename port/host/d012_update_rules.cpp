// The update check's reading of GitHub's answer and its version order
// (d012_update_rules.hpp). MIT licence; see port/README.md.

#include "d012_update_rules.hpp"

#include "d012_ed25519.hpp"
#include "d012_json.hpp"

#include <array>
#include <cstdint>
#include <span>

namespace d012::update_rules {
namespace {

// The largest sizes the three files of an update can have.
constexpr std::uint64_t kMaxProgramBytes = 1024ull * 1024u * 1024u;
constexpr std::uint64_t kMaxSumsBytes = 64u * 1024u;
constexpr std::uint64_t kSignatureBytes = 64u;

int hex_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

const Asset *find_asset(const Release &release, std::string_view name) {
    for (const Asset &asset : release.assets)
        if (asset.name == name) return &asset;
    return nullptr;
}

// The files of a release as GitHub lists them. One that is not whole, or is
// not under this project's downloads, is left out.
std::vector<Asset> read_assets(const json::Value &answer) {
    std::vector<Asset> out;
    const json::Value *assets = answer.find("assets");
    if (assets == nullptr || !assets->is(json::Value::Kind::Array)) return out;
    for (const json::Value &item : assets->items) {
        const json::Value *name = item.find("name");
        const json::Value *url = item.find("browser_download_url");
        const json::Value *size = item.find("size");
        if (name == nullptr || !name->is(json::Value::Kind::String) || url == nullptr ||
            !url->is(json::Value::Kind::String) || size == nullptr || !size->is(json::Value::Kind::Number) ||
            size->number < 0.0 || size->number > 9.0e15)
            continue;
        if (url->text.rfind(kReleaseDownloads, 0) != 0) continue;
        out.push_back(Asset{name->text, url->text, static_cast<std::uint64_t>(size->number)});
    }
    return out;
}

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
    return Release{std::string(version), page->text, read_assets(*value)};
}

std::string setup_file_name(std::string_view version) {
    return "Dissidia012-PC-Setup-" + std::string(version) + ".exe";
}

std::string android_file_name(std::string_view version) {
    return "Dissidia012-" + std::string(version) + "-Android-arm64.apk";
}

std::string linux_file_name(std::string_view version) {
    return "Dissidia012-Setup-" + std::string(version) + "-x86_64.AppImage";
}

namespace {

std::optional<UpdateFiles> update_files(const Release &release, const std::string &program_name) {
    const Asset *program = find_asset(release, program_name);
    const Asset *sums = find_asset(release, kSumsFile);
    const Asset *signature = find_asset(release, kSignatureFile);
    if (program == nullptr || sums == nullptr || signature == nullptr) return std::nullopt;
    if (program->size == 0 || program->size > kMaxProgramBytes || sums->size == 0 || sums->size > kMaxSumsBytes ||
        signature->size != kSignatureBytes)
        return std::nullopt;
    return UpdateFiles{*program, *sums, *signature};
}

} // namespace

std::optional<UpdateFiles> windows_update(const Release &release) {
    return update_files(release, setup_file_name(release.version));
}

std::optional<UpdateFiles> android_update(const Release &release) {
    return update_files(release, android_file_name(release.version));
}

std::optional<UpdateFiles> linux_update(const Release &release) {
    return update_files(release, linux_file_name(release.version));
}

bool sums_signed(std::string_view sums, std::string_view signature, std::string_view public_key) {
    ed25519::PublicKey key{};
    ed25519::Signature bytes{};
    if (public_key.size() != key.size() * 2 || signature.size() != bytes.size()) return false;
    for (std::size_t i = 0; i < key.size(); ++i) {
        const int high = hex_digit(public_key[i * 2]);
        const int low = hex_digit(public_key[i * 2 + 1]);
        if (high < 0 || low < 0) return false;
        key[i] = static_cast<std::uint8_t>(high * 16 + low);
    }
    for (std::size_t i = 0; i < bytes.size(); ++i) bytes[i] = static_cast<std::uint8_t>(signature[i]);
    const std::span<const std::uint8_t> message(reinterpret_cast<const std::uint8_t *>(sums.data()), sums.size());
    return ed25519::verify(key, message, bytes);
}

std::string listed_hash(std::string_view sums, std::string_view name) {
    while (!sums.empty()) {
        const std::size_t end = sums.find('\n');
        std::string_view line = sums.substr(0, end);
        sums = end == std::string_view::npos ? std::string_view{} : sums.substr(end + 1);
        if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
        // <64 digits>, a space, then a space (text) or * (binary), then the name.
        if (line.size() != 66 + name.size() || line[64] != ' ' || (line[65] != ' ' && line[65] != '*') ||
            line.substr(66) != name)
            continue;
        std::string hash(line.substr(0, 64));
        bool digits = true;
        for (char &c : hash) {
            digits = digits && hex_digit(c) >= 0;
            if (c >= 'A' && c <= 'F') c = static_cast<char>(c - 'A' + 'a');
        }
        if (digits) return hash;
    }
    return {};
}

} // namespace d012::update_rules
