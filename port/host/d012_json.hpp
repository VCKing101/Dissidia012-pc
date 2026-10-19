#pragma once

// A small JSON reader for the port's own files (mod manifests). Values keep
// where they were in the text, so a manifest's errors can name a line.
// MIT licence; see port/README.md.

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace d012::json {

class Value {
public:
    enum class Kind { Null, Bool, Number, String, Array, Object };

    Kind kind = Kind::Null;
    bool boolean{};
    double number{};
    std::string text;                        // String
    std::vector<Value> items;                // Array
    std::vector<std::pair<std::string, Value>> members;  // Object, in the order written
    int line{};                              // where the value starts, from 1

    [[nodiscard]] bool is(Kind k) const noexcept { return kind == k; }
    // The member `name` of an object; null when absent or not an object.
    [[nodiscard]] const Value *find(std::string_view name) const;
    [[nodiscard]] static const char *kind_name(Kind kind);
};

// Parses `text`. On failure returns nothing and sets `error` to
// "line L, column C: what".
[[nodiscard]] std::optional<Value> parse(std::string_view text, std::string &error);

} // namespace d012::json
