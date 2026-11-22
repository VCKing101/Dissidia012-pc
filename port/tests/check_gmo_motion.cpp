// Checks the port's animation rebuild (host/d012_gmo_motion.cpp) against
// the Python one (tools/move_retarget.py), with the game's files that
// tools/original_moves_refs.py wrote:
//
//     check_gmo_motion <dev dir> <recipe json>
//
// <dev dir>/disc/obj holds the fighters' models and <dev dir>/ref/anim the
// motion chunk Python built for each animation of the recipe that has an
// owner. The files are the game's, so this is a developer's check and no
// part of the test run. Exit code 0: every animation came out byte for byte
// as Python's. MIT licence; see port/README.md.

#include "d012_gmo_motion.hpp"
#include "d012_json.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace {

namespace gm = d012::gmo_motion;
using d012::json::Value;

std::optional<std::vector<std::uint8_t>> read_file(const std::filesystem::path &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::nullopt;
    return std::vector<std::uint8_t>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

const Value &member(const Value &object, std::string_view name, Value::Kind kind) {
    const Value *value = object.find(name);
    if (value == nullptr || !value->is(kind)) {
        throw gm::Error("the recipe's \"" + std::string(name) + "\" is not " + Value::kind_name(kind));
    }
    return *value;
}

// The object `name` of an animation; nothing when it is left out or null.
const Value *optional_object(const Value &object, std::string_view name) {
    const Value *value = object.find(name);
    if (value == nullptr || value->is(Value::Kind::Null)) return nullptr;
    if (!value->is(Value::Kind::Object)) throw gm::Error("the recipe's \"" + std::string(name) + "\" is not an object");
    return value;
}

template <std::size_t N> std::array<double, N> numbers(const Value &list) {
    if (!list.is(Value::Kind::Array) || list.items.size() != N) {
        throw gm::Error("the recipe has a list that is not " + std::to_string(N) + " numbers");
    }
    std::array<double, N> out{};
    for (std::size_t i = 0; i < N; ++i) {
        if (!list.items[i].is(Value::Kind::Number)) throw gm::Error("the recipe has a list that is not numbers");
        out[i] = list.items[i].number;
    }
    return out;
}

// A list of numbers, or nothing when it is left out, null or empty.
template <std::size_t N>
std::optional<std::array<double, N>> optional_numbers(const Value &object, std::string_view name) {
    const Value *list = object.find(name);
    if (list == nullptr || list->is(Value::Kind::Null)) return std::nullopt;
    if (list->is(Value::Kind::Array) && list->items.empty()) return std::nullopt;
    return numbers<N>(*list);
}

// An animation of the recipe as Recipe.anim_chunk (tools/original_moves.py)
// hands it to the rebuild.
gm::Retarget options_of(const Value &info) {
    gm::Retarget options;
    options.motion = member(info, "motion", Value::Kind::String).text;
    const double frames = member(info, "frames", Value::Kind::Number).number;
    if (frames != std::floor(frames) || frames < 0.0 || frames > 100000.0) {
        throw gm::Error("the recipe's \"frames\" is not a count of frames");
    }
    options.frames = static_cast<int>(frames);
    options.mirror = member(info, "mirror", Value::Kind::Bool).boolean;
    options.speed = member(info, "speed", Value::Kind::Number).number;
    options.start = member(info, "start", Value::Kind::Number).number;
    options.root_scale = member(info, "root_scale", Value::Kind::Number).number;
    options.trans_scale = member(info, "travel_scale", Value::Kind::Number).number;
    if (const Value *follow = optional_object(info, "follow")) {
        for (const auto &[bone, theirs] : follow->members) {
            if (!theirs.is(Value::Kind::String)) throw gm::Error("the recipe's \"follow\" names no bone for " + bone);
            options.follow.emplace_back(bone, theirs.text);
        }
    }
    if (const Value *held = optional_object(info, "held")) {
        for (const auto &[bone, values] : held->members) {
            if (!values.is(Value::Kind::Object)) throw gm::Error("the recipe's \"held\" has no values for " + bone);
            const gm::Held value{optional_numbers<4>(values, "rotate"), optional_numbers<3>(values, "translate")};
            options.held.emplace_back(bone, value);
        }
    }
    if (const Value *scales = optional_object(info, "scales")) {
        for (const auto &[bone, keys] : scales->members) {
            if (!keys.is(Value::Kind::Array)) throw gm::Error("the recipe's \"scales\" has no keys for " + bone);
            std::vector<gm::ScaleKey> list;
            for (const Value &key : keys.items) {
                if (!key.is(Value::Kind::Array) || key.items.size() != 2u || !key.items[0].is(Value::Kind::Number)) {
                    throw gm::Error("the recipe's \"scales\" has a key that is not [frame, [x, y, z]]");
                }
                list.push_back({key.items[0].number, numbers<3>(key.items[1])});
            }
            options.scales.emplace_back(bone, std::move(list));
        }
    }
    return options;
}

std::string upper(std::string text) {
    for (char &c : text) {
        if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
    }
    return text;
}

} // namespace

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "usage: check_gmo_motion <dev dir> <recipe json>\n";
        return 2;
    }
    const std::filesystem::path dev = argv[1];
    const auto recipe_bytes = read_file(argv[2]);
    if (!recipe_bytes) {
        std::cerr << argv[2] << " cannot be read\n";
        return 2;
    }
    std::string error;
    const auto recipe = d012::json::parse(
        std::string_view(reinterpret_cast<const char *>(recipe_bytes->data()), recipe_bytes->size()), error);
    if (!recipe) {
        std::cerr << "the recipe: " << error << "\n";
        return 2;
    }
    const Value *animations = recipe->find("animations");
    if (animations == nullptr || !animations->is(Value::Kind::Object)) {
        std::cerr << "the recipe has no animations\n";
        return 2;
    }

    const auto started = std::chrono::steady_clock::now();
    // A fighter's skeleton, read once.
    std::map<std::string, gm::Skeleton> skeletons;
    const auto skeleton = [&](const std::string &code) -> const gm::Skeleton & {
        const auto found = skeletons.find(code);
        if (found != skeletons.end()) return found->second;
        const std::filesystem::path path = dev / "disc" / "obj" / ("P_" + upper(code) + "_1P.gmo");
        const auto bytes = read_file(path);
        if (!bytes) throw gm::Error(path.string() + " cannot be read");
        return skeletons.emplace(code, gm::Skeleton(*bytes)).first->second;
    };
    int checked = 0, same = 0;
    for (const auto &[number, info] : animations->members) {
        if (info.find("owner") == nullptr) continue;  // an animation 012 Bartz has himself
        ++checked;
        std::string name = "animation " + number;
        try {
            name = member(info, "name", Value::Kind::String).text;
            gm::Retarget options = options_of(info);
            options.name = name + "_OG";
            const gm::Skeleton &target = skeleton("fiv100");
            const gm::Skeleton &source = skeleton(member(info, "owner", Value::Kind::String).text);
            const std::vector<std::uint8_t> built = gm::retarget(source, target, options);
            const auto expected = read_file(dev / "ref" / "anim" / (name + ".bin"));
            if (!expected) {
                std::cout << name << "  " << built.size() << " bytes  no reference file\n";
                continue;
            }
            const std::size_t shared = std::min(built.size(), expected->size());
            std::size_t first = 0u;
            while (first < shared && built[first] == (*expected)[first]) ++first;
            if (first == shared && built.size() == expected->size()) {
                std::cout << name << "  " << built.size() << " bytes  same\n";
                ++same;
            } else {
                std::cout << name << "  " << built.size() << " bytes  differs at offset " << first << " (Python's has "
                          << expected->size() << " bytes)\n";
            }
        } catch (const std::exception &failure) {
            std::cout << name << "  failed: " << failure.what() << "\n";
        }
    }
    const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count();
    std::cout << same << " of " << checked << " animations the same as Python's, in " << seconds << " s\n";
    return checked != 0 && same == checked ? 0 : 1;
}
