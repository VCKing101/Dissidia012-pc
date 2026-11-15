// Play online's address file (d012_online_rules.hpp).
//
// MIT licence; see port/README.md.

#include "d012_online_rules.hpp"

#include "d012_json.hpp"

#include <optional>

namespace d012::online_rules {
namespace {

constexpr std::size_t kLongestAddress = 100u;
constexpr const char *kUnreadable = "The server's address could not be read.";

// A host name, an IPv4 address or a bracketed IPv6 one, with a port or not.
bool address_ok(const std::string &address) {
    if (address.empty() || address.size() > kLongestAddress) return false;
    for (const char c : address)
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '.' || c == '-' ||
              c == ':' || c == '[' || c == ']'))
            return false;
    return true;
}

} // namespace

std::string address_in(const std::string &text, std::string &note) {
    std::string error;
    const std::optional<json::Value> value = json::parse(text, error);
    const json::Value *address = value ? value->find("address") : nullptr;
    if (address == nullptr || !address->is(json::Value::Kind::String)) {
        note = kUnreadable;
        return {};
    }
    if (address->text.empty()) {
        note = kNoServerYet;
        return {};
    }
    if (!address_ok(address->text)) {
        note = kUnreadable;
        return {};
    }
    note.clear();
    return address->text;
}

} // namespace d012::online_rules
