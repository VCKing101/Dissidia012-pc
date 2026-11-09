#pragma once

// What the Discord status says, and Discord's local message format: the parts
// of d012_discord.cpp that need no game and no Discord, so d012_tests checks
// them. MIT licence; see port/README.md.

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace d012::discord_rules {

// The Discord application whose name the status shows ("Playing ..."):
// made by the project on Discord's developer site. Its ID is public.
// settings.ini's discord.application_id, or D012_DISCORD_APP_ID, names
// another (for tests). Empty: no status.
inline constexpr const char *kApplicationId = "1569315126220169347";

// The status's button, to the project's releases.
inline constexpr const char *kButtonLabel = "Get the PC port";
inline constexpr const char *kButtonUrl = "https://github.com/VCKing101/Dissidia012-pc/releases/latest";

// A character's name by the game's character ID (1 to 34; docs/research/
// roster-and-save.md 1.1). Null for 0, the unidentified 34 and anything else.
[[nodiscard]] const char *character_name(int id);

// What the game is doing, sampled from its memory (d012_discord.cpp).
struct Scene {
    enum class Where { Starting, Menus, Field };
    Where where = Where::Starting;
    // On the battle overlay (battles and Story Mode's world map): the FIELD
    // object's mode (*(0x089D6A10) + 0x4C) and each side's character ID
    // (battle setup 0x09AFB8A0 + side * 0x1060 + 0x1058).
    std::uint32_t mode = 0;
    int fighters[2] = {0, 0};
    bool operator==(const Scene &) const = default;
};

// The two lines of the status. An empty `state` is left out.
struct Activity {
    std::string details;
    std::string state;
    bool operator==(const Activity &) const = default;
};

[[nodiscard]] Activity describe(const Scene &scene);

// Discord's messages over its local connection: a little-endian opcode and
// length, then JSON.
enum class Op : std::uint32_t { Handshake = 0, Frame = 1, Close = 2, Ping = 3, Pong = 4 };

[[nodiscard]] std::string encode(Op op, std::string_view json);

// Takes one whole message off the front of `buffer`, if one is there.
struct Message {
    Op op{};
    std::string json;
};
[[nodiscard]] std::optional<Message> take_message(std::string &buffer);
// More than any message of Discord's; a longer length means a broken stream.
inline constexpr std::uint32_t kMaxMessage = 64u * 1024u;
[[nodiscard]] bool broken(const std::string &buffer);

[[nodiscard]] std::string handshake(std::string_view application_id);
// SET_ACTIVITY for the process `pid`, with the time the game started
// (seconds since 1970) and the button. No activity clears the status.
[[nodiscard]] std::string set_activity(const Activity *activity, std::uint32_t pid, std::int64_t started,
                                       std::uint64_t nonce);

// What Discord answered, from a frame's JSON: its "evt" (READY, ERROR) or
// "cmd", and an error's message. Empty when the text is not JSON.
struct Answer {
    std::string event;
    std::string command;
    std::string error;
};
[[nodiscard]] Answer read_answer(std::string_view json);

} // namespace d012::discord_rules
