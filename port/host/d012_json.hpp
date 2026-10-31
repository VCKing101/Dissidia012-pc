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
    bool operator==(const Value &) const = default;
    // The member `name` of an object; null when absent or not an object.
    [[nodiscard]] const Value *find(std::string_view name) const;
    [[nodiscard]] static const char *kind_name(Kind kind);
};

// Parses `text`. On failure returns nothing and sets `error` to
// "line L, column C: what".
[[nodiscard]] std::optional<Value> parse(std::string_view text, std::string &error);

// `value` as text in the layout of Python's json.dumps(value, indent=2,
// ensure_ascii=False), which the Battle BGM Maker's Python tool writes, so the
// two tools write a manifest byte for byte the same. A number with no fraction
// is written as a whole number. No new line at the end.
[[nodiscard]] std::string write(const Value &value);

// Values made for writing.
[[nodiscard]] Value string_value(std::string text);
[[nodiscard]] Value number_value(double number);
[[nodiscard]] Value object_value();
[[nodiscard]] Value array_value();

} // namespace d012::json
