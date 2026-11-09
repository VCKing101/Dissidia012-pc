// What the Discord status says, and Discord's local message format
// (d012_discord_rules.hpp). MIT licence; see port/README.md.

#include "d012_discord_rules.hpp"

#include "d012_json.hpp"

#include <array>
#include <initializer_list>
#include <utility>

namespace d012::discord_rules {
namespace {

// By character ID, from 0 (docs/research/roster-and-save.md 1.1).
constexpr std::array<const char *, 35> kCharacters = {
    nullptr,          "Warrior of Light", "Garland",     "Firion",      "The Emperor", "Onion Knight",
    "Cloud of Darkness", "Cecil",         "Golbez",      "Bartz",       "Exdeath",     "Terra",
    "Kefka",          "Cloud",            "Sephiroth",   "Squall",      "Ultimecia",   "Zidane",
    "Kuja",           "Tidus",            "Jecht",       "Shantotto",   "Gabranth",    "Chaos",
    "Kain",           "Gilgamesh",        "Tifa",        "Laguna",      "Yuna",        "Prishe",
    "Vaan",           "Lightning",        "Feral Chaos", "Aerith",      nullptr,
};

// Story Mode's world map and gateway boards: the battle setup still holds
// the last battle's fighters there, so none are shown.
constexpr std::uint32_t kModeStoryMap = 0;

// The FIELD object's modes with a name of their own. The rest show as a
// battle.
const char *mode_name(std::uint32_t mode) {
    switch (mode) {
    case kModeStoryMap: return "Story Mode";
    case 4: return "Quick Battle";
    case 5: return "Online battle";  // Communication Mode, which the port plays online
    case 9: return "Watching a replay";
    default: return nullptr;
    }
}

json::Value member_object(std::initializer_list<std::pair<const char *, json::Value>> members) {
    json::Value out = json::object_value();
    for (const auto &[name, value] : members) out.members.emplace_back(name, value);
    return out;
}

json::Value bool_value(bool on) {
    json::Value out;
    out.kind = json::Value::Kind::Bool;
    out.boolean = on;
    return out;
}

void put_u32(std::string &out, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<char>((value >> (8 * i)) & 0xFFu));
}

std::uint32_t get_u32(const std::string &in, std::size_t at) {
    std::uint32_t value = 0;
    for (int i = 0; i < 4; ++i) value |= static_cast<std::uint32_t>(static_cast<unsigned char>(in[at + i])) << (8 * i);
    return value;
}

} // namespace

const char *character_name(int id) {
    return id >= 0 && id < static_cast<int>(kCharacters.size()) ? kCharacters[static_cast<std::size_t>(id)] : nullptr;
}

Activity describe(const Scene &scene) {
    Activity out;
    if (scene.where != Scene::Where::Field) {
        out.details = "In the menus";
        return out;
    }
    const char *mode = mode_name(scene.mode);
    out.details = mode != nullptr ? mode : "In a battle";
    if (scene.mode == kModeStoryMap) return out;
    const char *first = character_name(scene.fighters[0]);
    const char *second = character_name(scene.fighters[1]);
    if (first != nullptr && second != nullptr)
        out.state = std::string(first) + " vs " + second;
    else if (first != nullptr)
        out.state = std::string("As ") + first;
    return out;
}

std::string encode(Op op, std::string_view json) {
    std::string out;
    out.reserve(8 + json.size());
    put_u32(out, static_cast<std::uint32_t>(op));
    put_u32(out, static_cast<std::uint32_t>(json.size()));
    out.append(json);
    return out;
}

std::optional<Message> take_message(std::string &buffer) {
    if (buffer.size() < 8 || broken(buffer)) return std::nullopt;
    const std::uint32_t length = get_u32(buffer, 4);
    if (buffer.size() < 8u + length) return std::nullopt;
    Message out{static_cast<Op>(get_u32(buffer, 0)), buffer.substr(8, length)};
    buffer.erase(0, 8u + length);
    return out;
}

bool broken(const std::string &buffer) {
    return buffer.size() >= 8 && get_u32(buffer, 4) > kMaxMessage;
}

std::string handshake(std::string_view application_id) {
    return json::write(member_object({{"v", json::number_value(1)},
                                      {"client_id", json::string_value(std::string(application_id))}}));
}

std::string set_activity(const Activity *activity, std::uint32_t pid, std::int64_t started, std::uint64_t nonce) {
    json::Value args = member_object({{"pid", json::number_value(static_cast<double>(pid))}});
    if (activity != nullptr) {
        json::Value body = member_object({{"details", json::string_value(activity->details)}});
        if (!activity->state.empty()) body.members.emplace_back("state", json::string_value(activity->state));
        body.members.emplace_back(
            "timestamps", member_object({{"start", json::number_value(static_cast<double>(started))}}));
        json::Value buttons = json::array_value();
        buttons.items.push_back(member_object(
            {{"label", json::string_value(kButtonLabel)}, {"url", json::string_value(kButtonUrl)}}));
        body.members.emplace_back("buttons", std::move(buttons));
        body.members.emplace_back("instance", bool_value(false));
        args.members.emplace_back("activity", std::move(body));
    }
    return json::write(member_object({{"cmd", json::string_value("SET_ACTIVITY")},
                                      {"args", std::move(args)},
                                      {"nonce", json::string_value(std::to_string(nonce))}}));
}

Answer read_answer(std::string_view text) {
    Answer out;
    std::string error;
    const std::optional<json::Value> value = json::parse(text, error);
    if (!value || !value->is(json::Value::Kind::Object)) return out;
    if (const json::Value *evt = value->find("evt"); evt != nullptr && evt->is(json::Value::Kind::String))
        out.event = evt->text;
    if (const json::Value *cmd = value->find("cmd"); cmd != nullptr && cmd->is(json::Value::Kind::String))
        out.command = cmd->text;
    // An error frame's data, or a CLOSE's body: {"code": 4000, "message": "..."}.
    const json::Value *data = value->find("data");
    const json::Value *holder = data != nullptr && data->is(json::Value::Kind::Object) ? data : &*value;
    if (const json::Value *message = holder->find("message");
        message != nullptr && message->is(json::Value::Kind::String) && (out.event == "ERROR" || data == nullptr))
        out.error = message->text;
    return out;
}

} // namespace d012::discord_rules
