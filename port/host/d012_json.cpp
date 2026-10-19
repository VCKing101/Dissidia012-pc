// A small JSON reader (RFC 8259): objects, arrays, strings with escapes
// (\uXXXX and surrogate pairs to UTF-8), numbers, true, false, null. MIT
// licence; see port/README.md.

#include "d012_json.hpp"

#include <charconv>
#include <cstdlib>

namespace d012::json {
namespace {

class Parser {
public:
    explicit Parser(std::string_view text) : text_(text) {}

    std::optional<Value> document(std::string &error) {
        skip_space();
        // A UTF-8 byte order mark, which some editors write.
        if (text_.substr(at_).starts_with("\xEF\xBB\xBF")) at_ += 3u;
        skip_space();
        Value value;
        bool parsed = parse_value(value, 0);
        if (parsed) {
            skip_space();
            if (at_ != text_.size()) parsed = fail("text after the end of the document");
        }
        if (!parsed) {
            error = "line " + std::to_string(line_) + ", column " + std::to_string(column()) + ": " + error_;
            return std::nullopt;
        }
        return value;
    }

private:
    static constexpr int kMaxDepth = 64;

    bool fail(std::string what) {
        if (error_.empty()) error_ = std::move(what);
        return false;
    }
    [[nodiscard]] int column() const {
        std::size_t start = text_.rfind('\n', at_ == 0 ? 0 : at_ - 1u);
        start = start == std::string_view::npos ? 0u : start + 1u;
        return static_cast<int>(at_ - start) + 1;
    }
    void skip_space() {
        while (at_ < text_.size()) {
            const char c = text_[at_];
            if (c == '\n') ++line_;
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') break;
            ++at_;
        }
    }
    bool literal(std::string_view word) {
        if (text_.substr(at_, word.size()) != word) return fail("unexpected character");
        at_ += word.size();
        return true;
    }

    bool parse_value(Value &value, int depth) {
        if (depth > kMaxDepth) return fail("nested too deeply");
        skip_space();
        value.line = line_;
        if (at_ >= text_.size()) return fail("unexpected end of the document");
        const char c = text_[at_];
        if (c == '{') return parse_object(value, depth);
        if (c == '[') return parse_array(value, depth);
        if (c == '"') {
            value.kind = Value::Kind::String;
            return parse_string(value.text);
        }
        if (c == 't') {
            value.kind = Value::Kind::Bool;
            value.boolean = true;
            return literal("true");
        }
        if (c == 'f') {
            value.kind = Value::Kind::Bool;
            return literal("false");
        }
        if (c == 'n') return literal("null");
        if (c == '-' || (c >= '0' && c <= '9')) return parse_number(value);
        return fail(std::string("unexpected character '") + c + "'");
    }

    bool parse_object(Value &value, int depth) {
        value.kind = Value::Kind::Object;
        ++at_;
        skip_space();
        if (at_ < text_.size() && text_[at_] == '}') {
            ++at_;
            return true;
        }
        while (true) {
            skip_space();
            if (at_ >= text_.size() || text_[at_] != '"') return fail("expected a member name in quotes");
            std::string name;
            if (!parse_string(name)) return false;
            for (const auto &member : value.members)
                if (member.first == name) return fail("member \"" + name + "\" appears twice");
            skip_space();
            if (at_ >= text_.size() || text_[at_] != ':') return fail("expected ':' after a member name");
            ++at_;
            Value member;
            if (!parse_value(member, depth + 1)) return false;
            value.members.emplace_back(std::move(name), std::move(member));
            skip_space();
            if (at_ < text_.size() && text_[at_] == ',') {
                ++at_;
                continue;
            }
            if (at_ < text_.size() && text_[at_] == '}') {
                ++at_;
                return true;
            }
            return fail("expected ',' or '}' in an object");
        }
    }

    bool parse_array(Value &value, int depth) {
        value.kind = Value::Kind::Array;
        ++at_;
        skip_space();
        if (at_ < text_.size() && text_[at_] == ']') {
            ++at_;
            return true;
        }
        while (true) {
            Value item;
            if (!parse_value(item, depth + 1)) return false;
            value.items.push_back(std::move(item));
            skip_space();
            if (at_ < text_.size() && text_[at_] == ',') {
                ++at_;
                continue;
            }
            if (at_ < text_.size() && text_[at_] == ']') {
                ++at_;
                return true;
            }
            return fail("expected ',' or ']' in an array");
        }
    }

    static void put_utf8(std::string &out, std::uint32_t code) {
        if (code < 0x80u) {
            out.push_back(static_cast<char>(code));
        } else if (code < 0x800u) {
            out.push_back(static_cast<char>(0xC0u | (code >> 6u)));
            out.push_back(static_cast<char>(0x80u | (code & 0x3Fu)));
        } else if (code < 0x10000u) {
            out.push_back(static_cast<char>(0xE0u | (code >> 12u)));
            out.push_back(static_cast<char>(0x80u | ((code >> 6u) & 0x3Fu)));
            out.push_back(static_cast<char>(0x80u | (code & 0x3Fu)));
        } else {
            out.push_back(static_cast<char>(0xF0u | (code >> 18u)));
            out.push_back(static_cast<char>(0x80u | ((code >> 12u) & 0x3Fu)));
            out.push_back(static_cast<char>(0x80u | ((code >> 6u) & 0x3Fu)));
            out.push_back(static_cast<char>(0x80u | (code & 0x3Fu)));
        }
    }

    bool hex4(std::uint32_t &code) {
        if (at_ + 4u > text_.size()) return fail("a \\u escape needs four hex digits");
        const auto [end, ec] = std::from_chars(text_.data() + at_, text_.data() + at_ + 4u, code, 16);
        if (ec != std::errc{} || end != text_.data() + at_ + 4u) return fail("a \\u escape needs four hex digits");
        at_ += 4u;
        return true;
    }

    bool parse_string(std::string &out) {
        ++at_;  // the opening quote
        while (true) {
            if (at_ >= text_.size()) return fail("a string is not closed");
            const char c = text_[at_++];
            if (c == '"') return true;
            if (c == '\n') return fail("a string runs past the end of its line");
            if (static_cast<unsigned char>(c) < 0x20u) return fail("a control character in a string");
            if (c != '\\') {
                out.push_back(c);
                continue;
            }
            if (at_ >= text_.size()) return fail("a string is not closed");
            const char e = text_[at_++];
            switch (e) {
            case '"': out.push_back('"'); break;
            case '\\': out.push_back('\\'); break;
            case '/': out.push_back('/'); break;
            case 'b': out.push_back('\b'); break;
            case 'f': out.push_back('\f'); break;
            case 'n': out.push_back('\n'); break;
            case 'r': out.push_back('\r'); break;
            case 't': out.push_back('\t'); break;
            case 'u': {
                std::uint32_t code{};
                if (!hex4(code)) return false;
                if (code >= 0xD800u && code < 0xDC00u) {
                    std::uint32_t low{};
                    if (text_.substr(at_, 2u) != "\\u") return fail("a lone surrogate in a \\u escape");
                    at_ += 2u;
                    if (!hex4(low)) return false;
                    if (low < 0xDC00u || low >= 0xE000u) return fail("a lone surrogate in a \\u escape");
                    code = 0x10000u + ((code - 0xD800u) << 10u) + (low - 0xDC00u);
                } else if (code >= 0xDC00u && code < 0xE000u) {
                    return fail("a lone surrogate in a \\u escape");
                }
                put_utf8(out, code);
                break;
            }
            default:
                return fail(std::string("unknown escape \\") + e);
            }
        }
    }

    bool parse_number(Value &value) {
        const std::size_t start = at_;
        if (text_[at_] == '-') ++at_;
        const auto digits = [&] {
            const std::size_t from = at_;
            while (at_ < text_.size() && text_[at_] >= '0' && text_[at_] <= '9') ++at_;
            return at_ > from;
        };
        if (at_ < text_.size() && text_[at_] == '0') ++at_;
        else if (!digits()) return fail("a number needs digits");
        if (at_ < text_.size() && text_[at_] == '.') {
            ++at_;
            if (!digits()) return fail("a number needs digits after '.'");
        }
        if (at_ < text_.size() && (text_[at_] == 'e' || text_[at_] == 'E')) {
            ++at_;
            if (at_ < text_.size() && (text_[at_] == '+' || text_[at_] == '-')) ++at_;
            if (!digits()) return fail("a number needs digits in its exponent");
        }
        const std::string number(text_.substr(start, at_ - start));
        value.kind = Value::Kind::Number;
        value.number = std::strtod(number.c_str(), nullptr);
        return true;
    }

    std::string_view text_;
    std::size_t at_{};
    int line_{1};
    std::string error_;
};

} // namespace

const Value *Value::find(std::string_view name) const {
    if (kind != Kind::Object) return nullptr;
    for (const auto &member : members)
        if (member.first == name) return &member.second;
    return nullptr;
}

const char *Value::kind_name(Kind kind) {
    switch (kind) {
    case Kind::Null: return "null";
    case Kind::Bool: return "true or false";
    case Kind::Number: return "a number";
    case Kind::String: return "a string";
    case Kind::Array: return "a list";
    case Kind::Object: return "an object";
    }
    return "?";
}

std::optional<Value> parse(std::string_view text, std::string &error) { return Parser(text).document(error); }

} // namespace d012::json
