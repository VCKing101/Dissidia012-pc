// The port's own tests, run by tools/port/pk_ctest.cmd <build dir>: the game's
// manifest rules (host/d012_mod_manifest.cpp) and the shared manifest cases
// (manifest_cases.json), the JSON writer, and the Battle BGM Maker's model,
// checks, names, saving and loop suggester. No game is needed. The expected
// texts of the writer and the model are what the Python tool
// (port/tools/bgm_maker) writes for the same documents. MIT licence; see
// port/README.md.

#include "checks.hpp"
#include "cheats/cwcheat.hpp"
#include "d012_battle_cheat_codes.hpp"
#include "d012_battle_relay.hpp"
#include "d012_discord_rules.hpp"
#include "d012_dlc_packs_plan.hpp"
#include "d012_frame_balance.hpp"
#include "d012_json.hpp"
#include "d012_mod_manifest.hpp"
#include "d012_no_window_rules.hpp"
#include "d012_online_rules.hpp"
#include "d012_summon_slot_rules.hpp"
#include "d012_update_rules.hpp"
#include "d012_versus_rules.hpp"
#include "d012_voice_scenes.hpp"
#include "model.hpp"
#include "names.hpp"
#include "psp_iso.hpp"
#include "saving.hpp"
#include "suggest.hpp"
#include "undo.hpp"

#if defined(_WIN32)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <chrono>
#include <cmath>
#include <thread>
#include <tuple>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {

namespace manifest = d012::mod_manifest;
using manifest::Problem;

int g_failures = 0;

void expect(bool ok, const std::string &what) {
    if (ok) return;
    ++g_failures;
    std::cout << "FAIL: " << what << "\n";
}

void expect_text(const std::string &got, const std::string &want, const std::string &what) {
    if (got == want) return;
    ++g_failures;
    std::cout << "FAIL: " << what << "\n--- got:\n" << got << "\n--- want:\n" << want << "\n";
}

d012::json::Value parse(const std::string &text) {
    std::string error;
    auto value = d012::json::parse(text, error);
    expect(value.has_value(), "parse: " + error);
    return value ? *value : d012::json::Value{};
}

// A scratch mod folder with empty audio files where the manifests point.
struct Folder {
    std::filesystem::path path;
    Folder() {
        path = std::filesystem::temp_directory_path() / "d012-tests-mod";
        std::filesystem::remove_all(path);
        std::filesystem::create_directories(path / "music");
        for (const char *name : {"a.mp3", "b.ogg"}) std::ofstream(path / "music" / name).put('\0');
    }
    ~Folder() {
        std::error_code ec;
        std::filesystem::remove_all(path, ec);
    }
};

std::string describe(const std::vector<Problem> &problems) {
    std::string out;
    for (const Problem &p : problems) {
        const char *level = p.level == Problem::Level::Error ? "error" : p.level == Problem::Level::Warning ? "warning" : "note";
        out += std::string(level) + " " + p.path + " (line " + std::to_string(p.line) + "): " + p.message + "\n";
    }
    return out;
}

void test_writer() {
    using namespace d012::json;
    expect_text(write(parse("[83.5, 0.0001, 1e-05, 1e16, 123456789.125, -2.5, 0.1, 1.5e-07, 3, -7, 0]")),
                "[\n  83.5,\n  0.0001,\n  1e-05,\n  1e+16,\n  123456789.125,\n  -2.5,\n  0.1,\n  1.5e-07,\n  3,\n  -7,\n  0\n]",
                "numbers as Python writes them");
    expect_text(write(parse("{\"t\": \"a\\\"b\\\\c\\n\\r\\t\\b\\f\\u0001/\xC3\xA9\xE2\x80\xA8\", \"e\": [], \"o\": {}, "
                            "\"n\": null, \"b\": [true, false]}")),
                "{\n  \"t\": \"a\\\"b\\\\c\\n\\r\\t\\b\\f\\u0001/\xC3\xA9\xE2\x80\xA8\",\n  \"e\": [],\n  \"o\": {},\n"
                "  \"n\": null,\n  \"b\": [\n    true,\n    false\n  ]\n}",
                "strings, empty containers, literals");
}

void test_model() {
    const std::string source =
        "{\n"
        "  \"format\": 1,\n"
        "  \"id\": \"my-music\",\n"
        "  \"name\": \"My music\",\n"
        "  \"version\": \"1.0.0\",\n"
        "  \"author\": \"  Someone \",\n"
        "  \"description\": \"\",\n"
        "  \"homepage\": \"https://example.org\",\n"
        "  \"music\": {\n"
        "    \"series\": [\n"
        "      {\n"
        "        \"key\": \"ff15\",\n"
        "        \"name\": \"FINAL FANTASY XV\",\n"
        "        \"order\": 2,\n"
        "        \"colour\": \"blue\",\n"
        "        \"tracks\": [\n"
        "          {\"key\": \"a\", \"title\": \"Caf\xC3\xA9\", \"file\": \"music/a.mp3\", \"loop\": {\"start\": 100, \"end\": 200}},\n"
        "          {\"key\": \"b\", \"title\": \"B\", \"file\": \"music/b.ogg\", \"loop\": \"whole\", \"note\": [1, 2]},\n"
        "          {\"key\": \"c\", \"title\": \"C\", \"file\": \"music/c.flac\", \"loop\": {\"start_seconds\": 1.5, \"end_seconds\": 83.25}},\n"
        "          {\"key\": \"d\", \"title\": \"D\", \"file\": \"music/d.mp3\"}\n"
        "        ]\n"
        "      },\n"
        "      {\"key\": \"empty\", \"name\": \"EMPTY\", \"tracks\": []}\n"
        "    ]\n"
        "  }\n"
        "}";
    // port/tools/bgm_maker: manifest_bytes(build_manifest(model_from_manifest(source)))
    const std::string python =
        "{\n"
        "  \"format\": 1,\n"
        "  \"id\": \"my-music\",\n"
        "  \"name\": \"My music\",\n"
        "  \"version\": \"1.0.0\",\n"
        "  \"author\": \"Someone\",\n"
        "  \"music\": {\n"
        "    \"series\": [\n"
        "      {\n"
        "        \"key\": \"ff15\",\n"
        "        \"name\": \"FINAL FANTASY XV\",\n"
        "        \"order\": 2,\n"
        "        \"tracks\": [\n"
        "          {\n"
        "            \"key\": \"a\",\n"
        "            \"title\": \"Caf\xC3\xA9\",\n"
        "            \"file\": \"music/a.mp3\",\n"
        "            \"loop\": {\n"
        "              \"start\": 100,\n"
        "              \"end\": 200\n"
        "            }\n"
        "          },\n"
        "          {\n"
        "            \"key\": \"b\",\n"
        "            \"title\": \"B\",\n"
        "            \"file\": \"music/b.ogg\",\n"
        "            \"loop\": \"whole\",\n"
        "            \"note\": [\n"
        "              1,\n"
        "              2\n"
        "            ]\n"
        "          },\n"
        "          {\n"
        "            \"key\": \"c\",\n"
        "            \"title\": \"C\",\n"
        "            \"file\": \"music/c.flac\",\n"
        "            \"loop\": {\n"
        "              \"start_seconds\": 1.5,\n"
        "              \"end_seconds\": 83.25\n"
        "            }\n"
        "          },\n"
        "          {\n"
        "            \"key\": \"d\",\n"
        "            \"title\": \"D\",\n"
        "            \"file\": \"music/d.mp3\",\n"
        "            \"loop\": \"auto\"\n"
        "          }\n"
        "        ],\n"
        "        \"colour\": \"blue\"\n"
        "      },\n"
        "      {\n"
        "        \"key\": \"empty\",\n"
        "        \"name\": \"EMPTY\",\n"
        "        \"order\": 0,\n"
        "        \"tracks\": []\n"
        "      }\n"
        "    ]\n"
        "  },\n"
        "  \"homepage\": \"https://example.org\"\n"
        "}\n";
    const d012::bgm_maker::ModModel model = d012::bgm_maker::model_from_document(parse(source));
    expect_text(d012::bgm_maker::manifest_text(model), python, "the model writes what the Python tool writes");
    expect(model.series.size() == 2 && model.series[0].tracks.size() == 4, "the model has every series and track");
    expect(model.series[0].tracks[2].loop.in_seconds && model.series[0].tracks[2].loop.end_seconds == 83.25,
           "a loop in seconds stays in seconds");
    // A loop that ends at the file's end leaves "end" out.
    d012::bgm_maker::ModModel ends = model;
    ends.series[0].tracks[0].loop.end = 5000;
    const auto samples = [](const std::string &file, const void *) -> std::optional<std::int64_t> {
        return file == "music/a.mp3" ? std::optional<std::int64_t>(5000) : std::nullopt;
    };
    const std::string text = d012::bgm_maker::manifest_text(ends, samples);
    expect(text.find("\"end\": 5000") == std::string::npos && text.find("\"start\": 100") != std::string::npos,
           "a loop to the end of the file leaves \"end\" out");
    // A series' "short" after its name when it is text; anything else kept as it is (as the Python tool writes).
    const std::string shorts =
        R"({"format": 1, "id": "s", "name": "S", "version": "1.0.0", "music": {"series": [)"
        R"({"key": "a", "name": "FINAL FANTASY XVI", "short": "FFXVI", "order": 1, "tracks": []},)"
        R"({"key": "b", "name": "B", "order": 2, "short": 5, "tracks": []}]}})";
    const d012::bgm_maker::ModModel short_model = d012::bgm_maker::model_from_document(parse(shorts));
    expect(short_model.series[0].short_name == "FFXVI" && short_model.series[1].short_name.empty(), "a series' short name");
    expect_text(d012::bgm_maker::manifest_text(short_model),
                "{\n  \"format\": 1,\n  \"id\": \"s\",\n  \"name\": \"S\",\n  \"version\": \"1.0.0\",\n  \"music\": {\n"
                "    \"series\": [\n      {\n        \"key\": \"a\",\n        \"name\": \"FINAL FANTASY XVI\",\n"
                "        \"short\": \"FFXVI\",\n        \"order\": 1,\n        \"tracks\": []\n      },\n      {\n"
                "        \"key\": \"b\",\n        \"name\": \"B\",\n        \"order\": 2,\n        \"tracks\": [],\n"
                "        \"short\": 5\n      }\n    ]\n  }\n}\n",
                "short names written as the Python tool writes them");
}

void test_manifest() {
    Folder folder;
    {
        const auto result = manifest::read_text("{\"id\": \"Bad\", \"name\": \"N\", \"version\": \"1\"}", folder.path);
        expect_text(describe(result.problems),
                    "error mod.id (line 1): \"id\" must be lower-case letters, digits, '.', '_' or '-', starting with "
                    "a letter or digit\n",
                    "an id the game refuses");
        expect(!result.manifest.loaded, "a mod with a bad id is left out");
    }
    {
        const auto result = manifest::read_text("{\"id\": \"ok\", \"name\": \"N\", \"version\": \"1\"}", folder.path);
        expect_text(describe(result.problems), "warning mod (line 1): the mod adds nothing this build knows (no \"music\")\n",
                    "a mod with no music");
        expect(result.manifest.loaded, "a mod with no music still loads");
    }
    {
        const std::string text = "{\n"
                                 "  \"id\": \"ok\", \"name\": \"N\", \"version\": \"1\", \"extra\": 1,\n"
                                 "  \"music\": {\"series\": [\n"
                                 "    {\"key\": \"s\", \"name\": \"S\", \"order\": 1.5, \"tracks\": [\n"
                                 "      {\"key\": \"a\", \"title\": \"A\", \"file\": \"music/a.mp3\", \"loop\": {\"start\": 10}},\n"
                                 "      {\"key\": \"a\", \"title\": \"A2\", \"file\": \"music/b.ogg\"},\n"
                                 "      {\"key\": \"c\", \"title\": \"C\", \"file\": \"../c.mp3\"},\n"
                                 "      {\"key\": \"d\", \"title\": \"D\", \"file\": \"music/b.ogg\", \"loop\": {\"start\": 5, \"end\": 5}},\n"
                                 "      {\"key\": \"e\", \"title\": \"E\", \"file\": \"music/missing.mp3\"}\n"
                                 "    ]},\n"
                                 "    {\"key\": \"s\", \"name\": \"Again\", \"tracks\": []}\n"
                                 "  ]}\n"
                                 "}";
        const auto result = manifest::read_text(text, folder.path);
        expect_text(describe(result.problems),
                    "warning mod.extra (line 2): the mod has an unknown member \"extra\", which is ignored\n"
                    "error series.0.order (line 4): series \"s\": \"order\" must be a whole number from -1000000 to "
                    "1000000; 0 is used\n"
                    "error series.0.tracks.1.key (line 6): track \"a\" appears twice in the mod; the second is left out\n"
                    "error series.0.tracks.2.file (line 7): track \"c\" \"file\" must not leave the mod's folder (\"..\")\n"
                    "error series.0.tracks.2 (line 7): track \"c\" is left out\n"
                    "error series.0.tracks.3.loop.end (line 8): track \"d\" \"loop\" \"end\" must be a whole number of "
                    "samples after \"start\"\n"
                    "error series.0.tracks.3 (line 8): track \"d\" is left out\n"
                    "error series.0.tracks.4.file (line 9): track \"e\" \"file\" music/missing.mp3 is not in the mod's "
                    "folder\n"
                    "error series.0.tracks.4 (line 9): track \"e\" is left out\n"
                    "error series.1.key (line 11): series \"s\" appears twice; the second is left out\n",
                    "the problems of a broken manifest, in the game's words");
        expect(result.manifest.loaded && result.manifest.series.size() == 1 &&
                   result.manifest.series[0].tracks.size() == 1,
               "the parts the game can play are kept");
        const auto &track = result.manifest.series[0].tracks[0];
        expect(track.loop.mode == manifest::Loop::Mode::Range && track.loop.start.value == 10 && !track.loop.end,
               "a loop with a start and no end");
    }
    expect(manifest::valid_key("ff15-battle_1.0") && !manifest::valid_key("-a") && !manifest::valid_key("A") &&
               !manifest::valid_key(std::string(65, 'a')),
           "keys");
    // d012::mods::id_number of "a": FNV-1a 32 of one byte.
    expect(manifest::id_number("a") == 0xE40C292Cu, "FNV-1a");
}

// The shared manifest cases (tests/manifest_cases.json), which the Python
// tool's tests run too: each manifest, with empty files where it points (and
// other mods' ids and track ids), gives the problems listed, by level and
// path, and by message where both tools say the same. With `print`, the
// problems found are printed as the file lists them instead.
void test_shared_cases(bool print) {
    using d012::json::Value;
    std::ifstream in(std::filesystem::path(D012_TESTS_DIR) / "manifest_cases.json", std::ios::binary);
    const std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    const Value cases = parse(text);
    const Value *list = cases.find("cases");
    expect(list != nullptr && !list->items.empty(), "the shared cases are there");
    if (list == nullptr) return;
    Value printed = d012::json::array_value();
    for (const Value &item : list->items) {
        const std::string name = item.find("name")->text;
        const std::filesystem::path folder = std::filesystem::temp_directory_path() / "d012-tests-case";
        std::filesystem::remove_all(folder);
        for (const Value &file : item.find("files")->items) {
            const std::filesystem::path path = folder / manifest::path_from_utf8(file.text);
            std::filesystem::create_directories(path.parent_path());
            std::ofstream(path).put('\0');
        }
        std::vector<d012::bgm_maker::ModSummary> others;
        if (const Value *list_of_others = item.find("others")) {
            for (const Value &other : list_of_others->items) {
                d012::bgm_maker::ModSummary summary;
                summary.folder = other.find("folder")->text;
                summary.id = other.find("id")->text;
                for (const Value &track : other.find("tracks")->items)
                    summary.tracks[manifest::id_number(track.text)] = track.text;
                others.push_back(summary);
            }
        }
        const Value *manifest_value = item.find("manifest");
        const std::string source = manifest_value != nullptr ? d012::json::write(*manifest_value) : item.find("text")->text;
        std::string id;
        if (manifest_value != nullptr && manifest_value->find("id") != nullptr && manifest_value->find("id")->is(Value::Kind::String))
            id = manifest_value->find("id")->text;
        std::vector<Problem> got = d012::bgm_maker::check_text(source, id, folder, others);
        std::erase_if(got, [](const Problem &p) { return p.level == Problem::Level::Note; });
        std::filesystem::remove_all(folder);
        if (print) {
            Value entry = d012::json::object_value();
            entry.members.emplace_back("name", d012::json::string_value(name));
            Value problems = d012::json::array_value();
            for (const Problem &p : got) {
                Value one = d012::json::object_value();
                one.members.emplace_back("level", d012::json::string_value(p.level == Problem::Level::Error ? "error" : "warning"));
                one.members.emplace_back("path", d012::json::string_value(p.path));
                one.members.emplace_back("message", d012::json::string_value(p.message));
                problems.items.push_back(std::move(one));
            }
            entry.members.emplace_back("problems", std::move(problems));
            printed.items.push_back(std::move(entry));
            continue;
        }
        std::string want_text, got_text;
        const Value &want = *item.find("problems");
        for (std::size_t i = 0; i < std::max(want.items.size(), got.size()); ++i) {
            if (i < want.items.size()) {
                const Value &w = want.items[i];
                const Value *message = w.find("message");
                want_text += w.find("level")->text + " " + w.find("path")->text + (message ? ": " + message->text : "") + "\n";
                if (i < got.size())
                    got_text += std::string(got[i].level == Problem::Level::Error ? "error" : "warning") + " " + got[i].path +
                                (message ? ": " + got[i].message : "") + "\n";
            } else {
                got_text += std::string(got[i].level == Problem::Level::Error ? "error" : "warning") + " " + got[i].path +
                            ": " + got[i].message + "\n";
            }
        }
        expect_text(got_text, want_text, "shared case: " + name);
    }
    if (print) std::cout << d012::json::write(printed) << "\n";
}

// Saving: refused with an error left, the old mod.json kept in .backups (the
// last 20), nothing written when nothing changed, a new mod's waiting files
// moved into its folder, copied files no track uses deleted.
void test_saving() {
    namespace bm = d012::bgm_maker;
    const std::filesystem::path root = std::filesystem::temp_directory_path() / "d012-tests-saving";
    std::filesystem::remove_all(root);
    const std::filesystem::path mods = root / "mods";
    const std::filesystem::path staging = root / "staging";
    std::filesystem::create_directories(staging / "music");
    for (const char *name : {"a.mp3", "b.mp3"}) std::ofstream(staging / "music" / name).put('\0');
    std::filesystem::create_directories(mods);

    bm::SaveRequest request;
    request.mods = mods;
    request.folder = "new-one";
    request.is_new = true;
    request.staging = staging;
    request.model.id = "new-one";
    request.model.name = "New";
    request.model.version = "1.0.0";
    bm::SeriesModel series;
    series.key = "s";
    series.name = "S";
    bm::TrackModel track;
    track.key = "a";
    track.title = "A";
    track.file = "music/a.mp3";
    track.loop.mode = bm::LoopModel::Mode::Custom;
    track.loop.start = 100;
    track.loop.end = 5000;
    series.tracks.push_back(track);
    request.model.series.push_back(series);
    request.added = {staging / "music" / "a.mp3", staging / "music" / "b.mp3"};
    // The file is 5,000 samples long: a loop to its end leaves "end" out.
    request.facts = [](const std::string &) -> std::optional<bm::FileFacts> {
        bm::FileFacts facts;
        facts.rate = 44'100;
        facts.samples = 5000;
        return facts;
    };
    bm::SaveRequest refused = request;
    refused.model.series[0].tracks[0].loop.end = 5001;
    bm::SaveResult result = bm::save_mod(refused);
    expect(!result.saved && bm::has_errors(result.problems) && !std::filesystem::exists(mods / "new-one"),
           "a loop past the end of the file is not saved");

    result = bm::save_mod(request);
    const std::filesystem::path folder = mods / "new-one";
    expect(result.saved && result.error.empty() && result.backup.empty(), "a new mod saved: " + result.error);
    expect(std::filesystem::is_regular_file(folder / "music" / "a.mp3") && !std::filesystem::exists(folder / "music" / "b.mp3") &&
               !std::filesystem::exists(staging),
           "its files moved in, the unused copy deleted");
    std::ifstream in(folder / "mod.json", std::ios::binary);
    const std::string saved((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    expect(saved == result.text && saved.find("\"start\": 100") != std::string::npos &&
               saved.find("\"end\"") == std::string::npos,
           "mod.json written, \"end\" left out at the file's end");

    bm::SaveRequest again = request;
    again.is_new = false;
    again.staging.clear();
    again.added.clear();
    result = bm::save_mod(again);
    expect(result.saved && result.unchanged && !std::filesystem::exists(folder / ".backups"), "no change, nothing written");

    for (int i = 0; i < 22; ++i) {
        again.model.version = "1.0." + std::to_string(i + 1);
        result = bm::save_mod(again);
        expect(result.saved && !result.unchanged && result.backup.starts_with(".backups/mod-"), "a change kept a backup");
    }
    std::size_t backups = 0;
    for ([[maybe_unused]] const auto &entry : std::filesystem::directory_iterator(folder / ".backups")) ++backups;
    expect(backups == bm::kKeptBackups, "the last 20 backups kept, not " + std::to_string(backups));

    bm::SaveRequest taken = request;
    taken.staging.clear();
    taken.added.clear();
    taken.model.series.clear();
    result = bm::save_mod(taken);
    expect(!result.saved && !bm::problems_at(result.problems, "mod.folder").empty(), "a new mod's folder already there");
    std::error_code ec;
    std::filesystem::remove_all(root, ec);
}

void test_checks() {
    Folder folder;
    d012::bgm_maker::ModModel model;
    model.id = "taken";
    model.name = "N";
    model.version = "1";
    d012::bgm_maker::ModSummary other;
    other.folder = "someone-else";
    other.id = "taken";
    const auto problems = d012::bgm_maker::check_mod(model, folder.path, {other});
    expect(!d012::bgm_maker::problems_at(problems, "mod.id").empty(), "an id another mod has");
    expect(d012::bgm_maker::problems_at(problems, "mod", true).size() == 1, "problems below a path");
    expect(!d012::bgm_maker::folder_name_problem("a/b", folder.path).empty() &&
               !d012::bgm_maker::folder_name_problem("music", folder.path.parent_path() / "d012-tests-mod").empty() &&
               d012::bgm_maker::folder_name_problem("new-one", folder.path).empty(),
           "new folder names");
}

// The Python tool's names for the same texts (manifest.py key_from_text,
// unique_key, clip_bytes; server.py safe_file_name, title_from_file).
void test_names() {
    namespace names = d012::bgm_maker::names;
    const std::pair<const char *, const char *> keys[] = {
        {"Blinded by Light!", "blinded-by-light"},
        {"Caf\xC3\xA9 Ol\xC3\xA9", "cafe-ole"},
        {"FINAL FANTASY XV", "final-fantasy-xv"},
        {"  --Hello__World..  ", "hello-world"},
        {"\xE6\x9B\xB2\xE3\x81\xAE\xE3\x82\xBF\xE3\x82\xA4\xE3\x83\x88\xE3\x83\xAB", "track"},
        {"\xEF\xBC\xA6\xEF\xBD\x95\xEF\xBD\x8C\xEF\xBD\x8C \xEF\xBD\x97\xEF\xBD\x89\xEF\xBD\x84\xEF\xBD\x94\xEF\xBD\x88",
         "full-width"},
        {"Stra\xC3\x9F" "e \xC2\xBD", "strae-12"},
        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"},
        {"-.-", "track"},
        {"\xC3\x86r\xC3\xB8sk\xC3\xB8" "bing", "rskbing"},
        {"\xC4\xB2ssel \xC4\x90uro", "ijssel-uro"},
    };
    for (const auto &[text, key] : keys) expect_text(names::key_from_text(text), key, std::string("key of ") + text);

    expect_text(names::unique_key("battle", {"battle"}), "battle-2", "unique key");
    expect_text(names::unique_key("battle", {"battle", "battle-2"}), "battle-3", "unique key, twice taken");
    const std::string long_key(64, 'a');
    expect_text(names::unique_key(long_key, {long_key}), std::string(62, 'a') + "-2", "unique key at the length limit");
    expect_text(names::unique_key("theme", {}, {manifest::id_number("my-mod:theme")}, "my-mod"), "theme-2",
                "unique key, its number taken by another mod");

    struct File {
        const char *name;
        const char *safe;
        const char *title;
    };
    const File files[] = {
        {"03_Blinded_by_Light.brstm", "03_Blinded_by_Light.brstm", "Blinded by Light"},
        {"C:\\Music\\Caf\xC3\xA9 Ol\xC3\xA9 (Remix).MP3", "Cafe Ole (Remix).mp3", "Caf\xC3\xA9 Ol\xC3\xA9 (Remix)"},
        {"\xE6\x9B\xB2.flac", "track.flac", "\xE6\x9B\xB2"},
        {"..hidden.ogg", "hidden.ogg", "..hidden"},
        {"song.bwav",
         "ERROR \"song.bwav\": BWAV files are not supported (FFmpeg cannot decode them); use the BRSTM version of the track",
         "song"},
        {"notes.txt", "ERROR \"notes.txt\" is not .mp3, .ogg, .oga, .opus, .flac, .wav, .brstm, .bcstm or .bfstm", "notes"},
        {"x/y/Track 01 - Battle.wav", "Track 01 - Battle.wav", "Track 01 - Battle"},
        {"1234 Song.mp3", "1234 Song.mp3", "1234 Song"},
        {"12-Song.opus", "12-Song.opus", "Song"},
        {"___.mp3", "track.mp3", "Track"},
    };
    for (const File &file : files) {
        std::string error;
        const std::string safe = names::safe_file_name(file.name, error);
        expect_text(safe.empty() ? "ERROR " + error : safe, file.safe, std::string("file name of ") + file.name);
        expect_text(names::title_from_file(file.name), file.title, std::string("title of ") + file.name);
    }
    expect_text(names::clip_bytes("Caf\xC3\xA9", 4), "Caf", "clip inside a character");
    expect_text(names::clip_bytes("Caf\xC3\xA9", 3), "Caf", "clip before a character");
    expect_text(names::clip_bytes("ab  ", 4), "ab", "clip trailing space");
    expect_text(names::clip_bytes("\xE6\x9B\xB2\xE6\x9B\xB2", 4), "\xE6\x9B\xB2", "clip a CJK title");
    expect_text(names::upper_case("Final Fantasy \xC3\xA9t\xC3\xA9 \xC3\xB7"), "FINAL FANTASY \xC3\x89T\xC3\x89 \xC3\xB7",
                "upper case");
}

// The loop suggester finds a loop that is there: a tune of notes (an intro,
// a body played twice, the body's start fading out), its loop exactly the
// body's length.
void test_suggest() {
    constexpr int kRate = 22050;
    std::uint32_t seed = 12345u;
    const auto random = [&] {
        seed = seed * 1664525u + 1013904223u;
        return seed >> 8u;
    };
    const auto notes = [&](double seconds) {
        std::vector<float> out;
        while (out.size() < static_cast<std::size_t>(seconds * kRate)) {
            const double freq = 220.0 * std::pow(2.0, static_cast<double>(random() % 19u) / 12.0);
            const std::size_t count = static_cast<std::size_t>(kRate * (0.25 + 0.25 * (random() % 3u)));
            for (std::size_t i = 0; i < count && out.size() < static_cast<std::size_t>(seconds * kRate); ++i) {
                const double env = std::min(1.0, i / 100.0) * std::min(1.0, (count - i) / 200.0);
                out.push_back(static_cast<float>(0.3 * env * std::sin(2.0 * 3.14159265358979 * freq * i / kRate)));
            }
        }
        return out;
    };
    const std::vector<float> intro = notes(3.0);
    const std::vector<float> body = notes(12.5);
    std::vector<float> tune = intro;
    tune.insert(tune.end(), body.begin(), body.end());
    tune.insert(tune.end(), body.begin(), body.end());
    const std::size_t fade = 2u * kRate;
    for (std::size_t i = 0; i < fade; ++i) tune.push_back(body[i] * static_cast<float>(1.0 - static_cast<double>(i) / fade));
    const auto result = d012::bgm_maker::suggest_loops(tune, kRate, -1, 5.0, [](double) {});
    expect(!result.candidates.empty(), "the suggester finds the tune's loop");
    if (!result.candidates.empty()) {
        const auto &best = result.candidates.front();
        expect(best.length == static_cast<std::int64_t>(body.size()),
               "the loop's length to the sample: " + std::to_string(best.length) + " for " + std::to_string(body.size()));
        expect(best.score > 0.999, "the seam matches");
        expect(best.start >= static_cast<std::int64_t>(intro.size()) - kRate / 10, "the loop starts after the intro");
    }
    // Kept end: starts for the end of the second body.
    const std::int64_t end = static_cast<std::int64_t>(intro.size() + 2u * body.size());
    const auto starts = d012::bgm_maker::suggest_loops(tune, kRate, end, 5.0, [](double) {});
    expect(!starts.candidates.empty() && starts.candidates.front().end == end &&
               starts.candidates.front().start == end - static_cast<std::int64_t>(body.size()),
           "starts for a kept end");
}

void test_undo() {
    d012::bgm_maker::UndoStack<int> undo;
    undo.reset(1);
    expect(!undo.can_undo(), "nothing to undo at first");
    expect(undo.commit(2) && !undo.commit(2) && undo.commit(3), "commits only changes");
    expect(undo.undo() == 2 && undo.undo() == 1 && !undo.undo(), "undo back to the start");
    expect(undo.redo() == 2, "redo");
    expect(undo.commit(5) && !undo.can_redo(), "a new step drops the redo");
}


// ---- The PSP / PPSSPP export (bgm_maker/psp_iso.cpp) ------------------------------

namespace psp = d012::psp_iso;

void put_le(psp::Bytes &out, std::size_t at, std::uint32_t value, int size) {
    for (int i = 0; i < size; ++i) out[at + i] = static_cast<std::uint8_t>(value >> (8 * i));
}
void put_be(psp::Bytes &out, std::size_t at, std::uint32_t value, int size) {
    for (int i = 0; i < size; ++i) out[at + size - 1 - i] = static_cast<std::uint8_t>(value >> (8 * i));
}
std::uint32_t get_le(const psp::Bytes &in, std::size_t at) {
    return in[at] | in[at + 1] << 8 | in[at + 2] << 16 | static_cast<std::uint32_t>(in[at + 3]) << 24;
}

// An ISO 9660 directory record.
psp::Bytes iso_record(const std::string &ident, std::uint32_t lba, std::uint32_t size, bool dir) {
    psp::Bytes r(33 + ident.size() + (ident.size() % 2 == 0 ? 1 : 0), 0);
    r[0] = static_cast<std::uint8_t>(r.size());
    put_le(r, 2, lba, 4), put_be(r, 6, lba, 4), put_le(r, 10, size, 4), put_be(r, 14, size, 4);
    r[18] = 110, r[19] = 1, r[20] = 1;  // 2010-01-01
    r[25] = dir ? 2 : 0;
    put_le(r, 28, 1, 2), put_be(r, 30, 1, 2);
    r[32] = static_cast<std::uint8_t>(ident.size());
    std::copy(ident.begin(), ident.end(), r.begin() + 33);
    return r;
}

// A small image laid out as the game's disc: PACKAGE.BIN with the two main
// archives, then MOVIE/DES06_001.PMF, a BGM folder with one file, path tables.
struct FakeDisc {
    std::filesystem::path path;
    psp::Bytes movie, music, other;
    std::uint32_t bgm_lba = 24, l_table = 18, m_table = 19;
};

FakeDisc make_fake_disc(const std::filesystem::path &path) {
    FakeDisc d;
    d.path = path;
    constexpr std::uint32_t S = 2048;
    psp::BgmTable table;
    table.version = 0x20100831u;
    psp::MessageTable names;
    for (int i = 0; i < 3; ++i) {
        psp::BgmRow row;
        row.id = static_cast<std::int16_t>(i);
        row.name_text = static_cast<std::int16_t>(i);
        row.volume = static_cast<std::int8_t>(40 + 10 * i);  // 40, 50, 60: the median is 50
        row.weight = 2;
        const std::string file = "B01_0" + std::to_string(i) + ".at3";
        std::copy(file.begin(), file.end(), row.file.begin());
        table.rows.push_back(row);
        names.push_back(psp::title_text("FFI - Track " + std::to_string(i)));
    }
    names.emplace_back();  // a string the table leaves out
    d.other.assign(100, 0x5A);
    psp::Mpk main{0x20070606u, {{"other.bin", d.other, 0}, {"bgm_entry.bin", psp::write_bgm_table(table), 0}}};
    psp::Mpk lang{0x20070606u, {{"bgm_name.bin", psp::write_message_table(names), 0}}};
    const psp::Bytes main_bin = psp::write_mpk(main), lang_bin = psp::write_mpk(lang);
    // Sectors: 16 PVD, 17 end, 18/19 path tables, 20-25 folders, 26 the music, 27 PACKAGE_INFO.BIN,
    // 28.. PACKAGE.BIN (one sector per archive), then the movie (3 sectors).
    const std::uint32_t pkg = 28, movie = 30, end = 33;
    psp::Bytes image(end * S, 0);
    const auto place = [&](std::uint32_t lba, const psp::Bytes &data) { std::copy(data.begin(), data.end(), image.begin() + lba * S); };
    d.music.assign(1500, 0x11);
    d.movie.resize(3 * S - 100);
    for (std::size_t i = 0; i < d.movie.size(); ++i) d.movie[i] = static_cast<std::uint8_t>(i * 7);
    place(26, d.music);
    place(pkg, main_bin);
    place(pkg + 1, lang_bin);
    place(movie, d.movie);
    psp::Bytes info(16 + 2 * 12, 0);
    put_le(info, 8, 2, 4);
    std::uint32_t h1 = psp::package_hash("general_archive/main/main.bin");
    std::uint32_t h2 = psp::package_hash("general_archive/main/EN/main_lang.bin");
    put_le(info, 16, h1, 4), put_le(info, 20, 0x00500000u | 0, 4), put_le(info, 24, 0x10000000u | static_cast<std::uint32_t>(main_bin.size()), 4);
    put_le(info, 28, h2, 4), put_le(info, 32, 0x00500000u | 1, 4), put_le(info, 36, 0x10000000u | static_cast<std::uint32_t>(lang_bin.size()), 4);
    place(27, info);
    // Folders: root 20, PSP_GAME 21, USRDIR 22, DATA 23, BGM 24, MOVIE 25.
    const auto folder = [&](std::uint32_t lba, std::uint32_t parent, std::vector<psp::Bytes> records) {
        psp::Bytes extent;
        for (const auto &r : {iso_record(std::string(1, '\0'), lba, S, true), iso_record(std::string(1, '\1'), parent, S, true)})
            extent.insert(extent.end(), r.begin(), r.end());
        for (const auto &r : records) extent.insert(extent.end(), r.begin(), r.end());
        place(lba, extent);
    };
    folder(20, 20, {iso_record("PSP_GAME", 21, S, true)});
    folder(21, 20, {iso_record("USRDIR", 22, S, true)});
    folder(22, 21, {iso_record("DATA", 23, S, true)});
    folder(23, 22, {iso_record("BGM", d.bgm_lba, S, true), iso_record("MOVIE", 25, S, true),
                    iso_record("PACKAGE.BIN;1", pkg, 2 * S, false), iso_record("PACKAGE_INFO.BIN;1", 27, static_cast<std::uint32_t>(info.size()), false)});
    folder(24, 23, {iso_record("B01_01.AT3;1", 26, static_cast<std::uint32_t>(d.music.size()), false)});
    folder(25, 23, {iso_record("DES06_001.PMF;1", movie, static_cast<std::uint32_t>(d.movie.size()), false)});
    // Path tables (L little-endian, M big-endian).
    const std::vector<std::tuple<std::string, std::uint32_t, std::uint16_t>> dirs = {
        {std::string(1, '\0'), 20, 1}, {"PSP_GAME", 21, 1}, {"USRDIR", 22, 2}, {"DATA", 23, 3}, {"BGM", 24, 4}, {"MOVIE", 25, 4}};
    psp::Bytes l, m;
    for (const auto &[name, lba, parent] : dirs) {
        psp::Bytes e(8 + name.size() + name.size() % 2, 0);
        e[0] = static_cast<std::uint8_t>(name.size());
        std::copy(name.begin(), name.end(), e.begin() + 8);
        psp::Bytes be = e;
        put_le(e, 2, lba, 4), put_le(e, 6, parent, 2);
        put_be(be, 2, lba, 4), put_be(be, 6, parent, 2);
        l.insert(l.end(), e.begin(), e.end());
        m.insert(m.end(), be.begin(), be.end());
    }
    place(d.l_table, l);
    place(d.m_table, m);
    psp::Bytes pvd(S, 0);
    pvd[0] = 1;
    std::copy_n("CD001", 5, pvd.begin() + 1);
    pvd[6] = 1;
    put_le(pvd, 80, end, 4), put_be(pvd, 84, end, 4);
    put_le(pvd, 132, static_cast<std::uint32_t>(l.size()), 4), put_be(pvd, 136, static_cast<std::uint32_t>(l.size()), 4);
    put_le(pvd, 140, d.l_table, 4), put_be(pvd, 148, d.m_table, 4);
    const psp::Bytes root = iso_record(std::string(1, '\0'), 20, S, true);
    std::copy(root.begin(), root.end(), pvd.begin() + 156);
    place(16, pvd);
    psp::Bytes terminator(S, 0);
    terminator[0] = 255;
    std::copy_n("CD001", 5, terminator.begin() + 1);
    place(17, terminator);
    std::ofstream(path, std::ios::binary).write(reinterpret_cast<const char *>(image.data()), static_cast<std::streamsize>(image.size()));
    return d;
}

void test_psp_iso() {
    // Formats.
    expect(psp::package_hash("general_archive/main/main.bin") == 0x93307C1Cu &&
               psp::package_hash("GENERAL_ARCHIVE/main/EN/main_lang.bin") == 0xB1E92C5Eu,
           "the package name hash (as on the disc's PACKAGE_INFO.BIN)");
    const auto sha = [](const std::string &text) {
        psp::Sha1 s;
        s.update(reinterpret_cast<const std::uint8_t *>(text.data()), text.size());
        return s.hex();
    };
    expect(sha("") == "da39a3ee5e6b4b0d3255bfef95601890afd80709" && sha("abc") == "a9993e364706816aba3e25717850c26c9cd0d89d" &&
               sha("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq") == "84983e441c3bd26ebaae4aa1f95129e5e54670f1",
           "SHA-1 (FIPS 180 examples)");
    // tools/character_text.py: write_table([bytes([0x41, 0, 0, 0])]).hex() == "010002000400d9edab90"
    const psp::Bytes table = psp::write_message_table({psp::Bytes{0x41, 0, 0, 0}});
    expect(table == psp::Bytes{0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0xD9, 0xED, 0xAB, 0x90}, "a message table as character_text.py writes it");
    std::string error;
    const auto back = psp::read_message_table(table, error);
    expect(back && back->size() == 1 && (*back)[0] == psp::Bytes({0x41, 0, 0, 0}), "a message table read back");
    expect(psp::text_title(psp::title_text("Caf\xC3\xA9 - \xE6\x97\xA5 X")) == "Caf? - ? X", "names outside ASCII as '?'");
    psp::Mpk archive{7, {{"a.bin", {1, 2, 3}, 0}, {"long_name.bin", psp::Bytes(40, 9), 5}}};
    const psp::Bytes mpk = psp::write_mpk(archive);
    const auto mpk_back = psp::read_mpk(mpk, error);
    expect(mpk_back && mpk_back->version == 7 && mpk_back->files.size() == 2 && mpk_back->files[1].data == psp::Bytes(40, 9) &&
               mpk_back->files[1].extra == 5 && mpk.size() % 16 == 0 && get_le(mpk, 16) == 48 && get_le(mpk, 20) == 80,
           "an MPK archive: names after the entries, data 16-byte aligned");
    psp::BgmTable rows{0x20100831u, 0, {psp::BgmRow{}}};
    rows.rows[0].id = 0x4000, rows.rows[0].volume = -3, rows.rows[0].stage_mask = {1, 2};
    const psp::Bytes bent = psp::write_bgm_table(rows);
    const auto bent_back = psp::read_bgm_table(bent, error);
    expect(bent.size() == 16 + 0x2C && bent_back && bent_back->rows[0].id == 0x4000 && bent_back->rows[0].volume == -3 &&
               bent_back->rows[0].stage_mask[1] == 2 && bent_back->rows[0].unlock[3] == -1,
           "a BENT table");

    // The encoder's file under Sony's header.
    psp::Bytes encoded = {'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 16, 0, 0, 0,
                          0x70, 0x02, 2, 0, 0x44, 0xAC, 0, 0, 0, 0, 0, 0, 0x80, 0x01, 0, 0, 'd', 'a', 't', 'a', 0x00, 0x03, 0, 0};
    encoded.resize(encoded.size() + 768, 0xAB);
    const auto at3 = psp::at3_file(encoded, 50000, std::pair<std::int64_t, std::int64_t>{1000, 20000}, error);
    expect(at3.has_value(), "an ATRAC3 file made: " + error);
    if (at3) {
        const auto find = [&](const char *tag) {
            for (std::size_t pos = 12; pos + 8 <= at3->size(); pos += 8 + get_le(*at3, pos + 4))
                if (std::equal(tag, tag + 4, at3->begin() + static_cast<std::ptrdiff_t>(pos))) return pos;
            return std::size_t{0};
        };
        const std::size_t fact = find("fact"), smpl = find("smpl"), data = find("data");
        expect(fact && smpl && data && get_le(*at3, fact + 8) == 20000 && get_le(*at3, fact + 12) == psp::kEncoderDelay &&
                   get_le(*at3, smpl + 8 + 36 + 8) == 1000 + psp::kEncoderDelay &&
                   get_le(*at3, smpl + 8 + 36 + 12) == 19999 + psp::kEncoderDelay && get_le(*at3, data + 4) == 768 &&
                   get_le(*at3, 4) == at3->size() - 8,
               "fact (loop end, delay), smpl (start and end included, with the delay), the frames");
    }
    expect(!psp::at3_file(encoded, 50000, std::pair<std::int64_t, std::int64_t>{1000, 2000}, error), "a loop under 6,144 samples is refused");
    std::vector<std::int16_t> pcm(2 * 10000);
    for (std::size_t i = 0; i < pcm.size(); ++i) pcm[i] = static_cast<std::int16_t>(i);
    const auto input = psp::encoder_input(pcm, std::pair<std::int64_t, std::int64_t>{2000, 9000});
    expect(input.size() == 2 * (9000 + psp::kTailSamples) && input[2 * 9000] == pcm[2 * 2000] &&
               input[2 * 9000 - 1] == pcm[2 * 9000 - 1],
           "the encoder's input: the audio to the loop end, then the loop's start again");

    // A whole image.
    const std::filesystem::path root = std::filesystem::temp_directory_path() / "d012-tests-psp";
    std::filesystem::remove_all(root);
    std::filesystem::create_directories(root);
    const FakeDisc disc = make_fake_disc(root / "disc.iso");
    const auto music = psp::read_disc_music(disc.path, error);
    expect(music && music->rows == 3 && music->names == 4 && music->volume == 50 && music->room() == 253 &&
               music->titles[2] == "FFI - Track 2",
           "the fake disc's music table: " + error);
    std::vector<psp::Track> tracks;
    std::vector<psp::Bytes> files;
    for (int i = 0; i < 2; ++i) {
        files.emplace_back(3000 + 2048 * i, static_cast<std::uint8_t>(0xC0 + i));
        tracks.push_back({i == 0 ? "FFXVI - Away" : "FFXVI - Ifrit", root / ("t" + std::to_string(i) + ".at3")});
        std::ofstream(tracks.back().at3, std::ios::binary).write(reinterpret_cast<const char *>(files.back().data()), static_cast<std::streamsize>(files.back().size()));
    }
    int calls = 0;
    const bool made = psp::make_iso(disc.path, root / "out.iso", tracks, [&](std::uint64_t, std::uint64_t) { ++calls; return true; }, error);
    expect(made && calls > 0 && !std::filesystem::exists(root / "out.iso.part"), "an image made: " + error);
    if (made) {
        const auto after = psp::read_disc_music(root / "out.iso", error);
        expect(after && after->rows == 5 && after->names == 6 && after->titles[3] == "FFXVI - Away" &&
                   after->titles[4] == "FFXVI - Ifrit" && after->ids.count(0x4001) == 1,
               "the made image's table has the two rows after the disc's: " + error);
        const auto read = [&](const char *path) { return psp::read_iso_file(root / "out.iso", path, error); };
        expect(read("/PSP_GAME/USRDIR/DATA/MOVIE/DES06_001.PMF") == disc.movie, "the file after PACKAGE.BIN moved, unchanged");
        expect(read("/PSP_GAME/USRDIR/DATA/BGM/B01_01.AT3") == disc.music, "the disc's music unchanged");
        expect(read("/PSP_GAME/USRDIR/DATA/BGM/MD00000000.AT3") == files[0] &&
                   read("/PSP_GAME/USRDIR/DATA/BGM/MD00000001.AT3") == files[1],
               "the added music in DATA/BGM");
        psp::Bytes image;
        {
            std::ifstream in(root / "out.iso", std::ios::binary);
            image.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
        }
        const std::uint32_t blocks = get_le(image, 16 * 2048 + 80);
        std::size_t entry = disc.l_table * 2048;
        for (int skip = 0; skip < 4; ++skip) entry += 8 + image[entry] + image[entry] % 2;  // to the fifth, BGM
        const std::uint32_t l_bgm = get_le(image, entry + 2);
        expect(image.size() == static_cast<std::size_t>(blocks) * 2048 && blocks > 33, "the volume size is the image's");
        expect(l_bgm != disc.bgm_lba && l_bgm < blocks, "the L path table follows the BGM folder");
        expect(!psp::make_iso(disc.path, root / "again.iso", tracks, [](std::uint64_t, std::uint64_t) { return false; }, error) &&
                   !std::filesystem::exists(root / "again.iso") && !std::filesystem::exists(root / "again.iso.part"),
               "a cancelled image leaves nothing behind");
        expect(!psp::make_iso(root / "out.iso", root / "twice.iso", tracks, nullptr, error),
               "an image made by the tool is refused as the original");
    }
    std::error_code ec;
    std::filesystem::remove_all(root, ec);
}

// The update check's version order and its reading of GitHub's answer
// (host/d012_update_rules.cpp).
void test_update_rules() {
    namespace rules = d012::update_rules;
    expect(rules::is_version("0.5.4") && rules::is_version("10.0.12") && rules::is_version("0.6.0-rc.1"),
           "versions are read");
    expect(!rules::is_version("0.5") && !rules::is_version("v0.5.4") && !rules::is_version("0.5.4-") &&
               !rules::is_version("0.5.4 ") && !rules::is_version("0.5.x") && !rules::is_version("0.5.4-RC"),
           "what is not a version is refused");
    expect(rules::compare_versions("0.5.5", "0.5.4") > 0, "0.5.5 is newer than 0.5.4");
    expect(rules::compare_versions("0.5.10", "0.5.9") > 0, "versions compare by number, not by text");
    expect(rules::compare_versions("1.0.0", "0.9.9") > 0, "the first number counts most");
    expect(rules::compare_versions("0.5.4", "0.5.4") == 0, "a version is the same as itself");
    expect(rules::compare_versions("0.6.0-rc.1", "0.6.0") < 0, "a test build is older than its release");
    expect(rules::compare_versions("0.6.0-rc.1", "0.5.4") > 0, "a test build is newer than an older release");
    expect(rules::compare_versions("0.6.0-rc.2", "0.6.0-rc.1") > 0, "test builds compare by their suffix");

    const std::string page = std::string(rules::kReleasePages) + "tag/v0.5.5";
    const auto answer = [&](const std::string &fields) { return "{\"url\": \"x\", " + fields + "}"; };
    std::string error;
    auto release = rules::parse_release(
        answer("\"tag_name\": \"v0.5.5\", \"html_url\": \"" + page + "\", \"draft\": false, \"prerelease\": false"),
        error);
    expect(release && release->version == "0.5.5" && release->page == page, "a release is read: " + error);
    release = rules::parse_release(answer("\"tag_name\": \"0.5.5\", \"html_url\": \"" + page + "\""), error);
    expect(release && release->version == "0.5.5", "a tag without its v is read: " + error);
    expect(!rules::parse_release(answer("\"tag_name\": \"v0.5.5\", \"html_url\": \"" + page + "\", \"prerelease\": true"),
                                 error),
           "a pre-release is not offered");
    expect(!rules::parse_release(answer("\"tag_name\": \"v0.5.5\", \"html_url\": \"" + page + "\", \"draft\": true"),
                                 error),
           "a draft is not offered");
    expect(!rules::parse_release(answer("\"tag_name\": \"latest\", \"html_url\": \"" + page + "\""), error),
           "a tag that is not a version is refused");
    expect(!rules::parse_release(answer("\"tag_name\": \"v0.5.5\", \"html_url\": \"https://example.com/v0.5.5\""), error),
           "a page elsewhere is not opened");
    expect(!rules::parse_release(answer("\"message\": \"Not Found\""), error), "an answer without a release is refused");
    expect(!rules::parse_release("<html>", error), "an answer that is not JSON is refused");

    // The files of a release, and which of them install it on Windows.
    const std::string downloads = std::string(rules::kReleaseDownloads) + "v0.5.5/";
    const auto asset = [&](const std::string &name, std::uint64_t size, const std::string &base) {
        return "{\"name\": \"" + name + "\", \"size\": " + std::to_string(size) + ", \"browser_download_url\": \"" +
               base + name + "\"}";
    };
    const auto with_assets = [&](const std::string &assets) {
        return rules::parse_release(answer("\"tag_name\": \"v0.5.5\", \"html_url\": \"" + page + "\", \"assets\": [" +
                                           assets + "]"),
                                    error);
    };
    const std::string setup = asset("Dissidia012-PC-Setup-0.5.5.exe", 80000000, downloads);
    const std::string sums = asset("SHA256SUMS", 408, downloads);
    const std::string signature = asset("SHA256SUMS.sig", 64, downloads);
    release = with_assets(setup + "," + sums + "," + signature);
    expect(release && release->assets.size() == 3, "a release's files are read: " + error);
    const auto update = release ? rules::windows_update(*release) : std::nullopt;
    expect(update && update->setup.size == 80000000 && update->setup.url == downloads + "Dissidia012-PC-Setup-0.5.5.exe" &&
               update->sums.name == "SHA256SUMS" && update->signature.name == "SHA256SUMS.sig",
           "a signed release can be installed");
    const auto installs = [&](const std::string &assets) {
        const auto read = with_assets(assets);
        return read && rules::windows_update(*read).has_value();
    };
    expect(!installs(setup + "," + sums), "a release with no signature is installed from its page");
    expect(!installs(sums + "," + signature), "a release with no setup is installed from its page");
    expect(!installs(asset("Dissidia012-PC-Setup-0.5.4.exe", 80000000, downloads) + "," + sums + "," + signature),
           "another version's setup is not this release's");
    expect(!installs(asset("Dissidia012-PC-Setup-0.5.5.exe", 80000000, "https://example.com/") + "," + sums + "," +
                     signature),
           "a setup from elsewhere is left out");
    expect(!installs(setup + "," + sums + "," + asset("SHA256SUMS.sig", 65, downloads)),
           "a signature of the wrong size is refused");
    expect(!installs(asset("Dissidia012-PC-Setup-0.5.5.exe", 0, downloads) + "," + sums + "," + signature),
           "an empty setup is refused");

    // The list of hashes.
    const std::string hash(64, 'a');
    const std::string list = std::string(64, 'b') + "  Dissidia012-0.5.5-Android-arm64.apk\n" + hash +
                             "  Dissidia012-PC-Setup-0.5.5.exe\n";
    expect(rules::listed_hash(list, "Dissidia012-PC-Setup-0.5.5.exe") == hash, "a file's hash is found");
    expect(rules::listed_hash(list, "Dissidia012-PC-Setup-0.5.4.exe").empty(), "another version's hash is not");
    expect(rules::listed_hash(list, "Setup-0.5.5.exe").empty(), "a part of a name does not match");
    expect(rules::listed_hash(std::string(64, 'A') + " *a.exe\r\n", "a.exe") == hash, "binary mode and capitals are read");
    expect(rules::listed_hash(std::string(64, 'x') + "  a.exe\n", "a.exe").empty(), "a hash that is not one is refused");

    // RFC 8032, section 7.1, test 3.
    const std::string key = "fc51cd8e6218a1a38da47ed00230f0580816ed13ba3303ac5deb911548908025";
    const std::string message = "\xaf\x82";
    const std::string signed_hex = "6291d657deec24024827e69c3abe01a30ce548a284743a445e3680d7db5ac3ac"
                                   "18ff9b538d16f290ae67f760984dc6594a7c15e9716ed28dc027beceea1ec40a";
    std::string good;
    for (std::size_t i = 0; i < signed_hex.size(); i += 2)
        good.push_back(static_cast<char>(std::stoi(signed_hex.substr(i, 2), nullptr, 16)));
    expect(rules::sums_signed(message, good, key), "a good signature is accepted");
    std::string damaged = good;
    damaged[10] = static_cast<char>(damaged[10] ^ 1);
    expect(!rules::sums_signed(message, damaged, key), "a damaged signature is refused");
    damaged = good;
    damaged[63] = static_cast<char>(damaged[63] ^ 0x40);
    expect(!rules::sums_signed(message, damaged, key), "a signature with an S out of range is refused");
    expect(!rules::sums_signed("\xaf\x83", good, key), "a changed list is refused");
    expect(!rules::sums_signed(message, good, std::string(key.size() - 1, 'f') + "e"), "another key's list is refused");
    expect(!rules::sums_signed(message, good, ""), "no key refuses everything");
    expect(!rules::sums_signed(message, good.substr(1), key), "a short signature is refused");
}

// What the player is told when the game cannot open its window, and its off
// switch (host/d012_no_window_rules.cpp, issue 24).
void test_no_window_rules() {
    namespace rules = d012::no_window_rules;
    const std::string why = "SDL_CreateWindow failed: Installed Vulkan doesn't implement the VK_KHR_surface extension";
    const std::string log = "C:\\Games\\Dissidia012\\data\\logs\\D012Native.log";
    const std::string windows = rules::message(why, rules::System::Windows, log);
    expect(windows.find("Reason: " + why + "\n") != std::string::npos, "the message gives the renderer's reason");
    expect(windows.find("AMD, NVIDIA or Intel") != std::string::npos, "Windows is told where its driver comes from");
    expect(windows.size() >= log.size() && windows.compare(windows.size() - log.size(), log.size(), log) == 0,
           "the message ends with the log's path");
    expect(rules::message(why, rules::System::Windows, "").find("log") == std::string::npos,
           "no log line without a log file");
    const std::string linux_text = rules::message(why, rules::System::Linux, "");
    expect(linux_text.find("Mesa") != std::string::npos && linux_text.find("AMD, NVIDIA or Intel") == std::string::npos,
           "Linux is told about its own drivers");
    expect(!rules::keep_running(nullptr) && !rules::keep_running("") && !rules::keep_running("0"),
           "unset, empty and 0 say what is wrong and end");
    expect(rules::keep_running("1"), "D012_HEADLESS_FALLBACK=1 runs on without a window");
}

void test_discord_rules() {
    namespace rules = d012::discord_rules;
    using Scene = rules::Scene;
    expect(std::string(rules::character_name(13)) == "Cloud" && std::string(rules::character_name(31)) == "Lightning" &&
               std::string(rules::character_name(32)) == "Feral Chaos",
           "characters are named by ID");
    expect(rules::character_name(0) == nullptr && rules::character_name(34) == nullptr &&
               rules::character_name(-1) == nullptr && rules::character_name(35) == nullptr,
           "no name for what is not a character");

    expect(rules::describe(Scene{}) == rules::Activity{"In the menus", ""}, "the menus before anything");
    Scene menus;
    menus.where = Scene::Where::Menus;
    menus.mode = 4;
    menus.fighters[0] = 13;
    expect(rules::describe(menus) == rules::Activity{"In the menus", ""}, "the menus name no fighter");
    Scene quick;
    quick.where = Scene::Where::Field;
    quick.mode = 4;
    quick.fighters[0] = 13;
    quick.fighters[1] = 14;
    expect(rules::describe(quick) == rules::Activity{"Quick Battle", "Cloud vs Sephiroth"}, "a Quick Battle");
    Scene online = quick;
    online.mode = 5;
    online.fighters[1] = 31;
    expect(rules::describe(online) == rules::Activity{"Online battle", "Cloud vs Lightning"}, "an online battle");
    Scene other = quick;
    other.mode = 1;
    other.fighters[1] = -1;
    expect(rules::describe(other) == rules::Activity{"In a battle", "As Cloud"}, "another mode, one fighter");
    Scene map = quick;
    map.mode = 0;
    expect(rules::describe(map) == rules::Activity{"Story Mode", ""}, "the world map names no fighter");

    const std::string frame = rules::encode(rules::Op::Frame, "{}");
    expect(frame == std::string("\x01\x00\x00\x00\x02\x00\x00\x00{}", 10), "a message is opcode, length, JSON");
    std::string buffer = frame.substr(0, 9);
    expect(!rules::take_message(buffer) && buffer.size() == 9, "half a message waits");
    buffer += frame.substr(9) + rules::encode(rules::Op::Ping, "[1]");
    auto first = rules::take_message(buffer);
    auto second = rules::take_message(buffer);
    expect(first && first->op == rules::Op::Frame && first->json == "{}" && second && second->op == rules::Op::Ping &&
               second->json == "[1]" && buffer.empty(),
           "messages are taken one by one");
    buffer = std::string("\x01\x00\x00\x00\xFF\xFF\xFF\x7F", 8);
    expect(rules::broken(buffer) && !rules::take_message(buffer), "a huge length is a broken stream");

    const d012::json::Value hello = parse(rules::handshake("123"));
    expect(hello.find("v") && hello.find("v")->number == 1 && hello.find("client_id") &&
               hello.find("client_id")->text == "123",
           "the handshake names the version and application");

    const rules::Activity fight{"Quick Battle", "Cloud vs Sephiroth"};
    const d012::json::Value set = parse(rules::set_activity(&fight, 4242, 1762650000, 7));
    const d012::json::Value *args = set.find("args");
    const d012::json::Value *activity = args ? args->find("activity") : nullptr;
    expect(set.find("cmd") && set.find("cmd")->text == "SET_ACTIVITY" && set.find("nonce") &&
               set.find("nonce")->text == "7" && args && args->find("pid") && args->find("pid")->number == 4242,
           "SET_ACTIVITY names the process");
    expect(activity && activity->find("details")->text == "Quick Battle" &&
               activity->find("state")->text == "Cloud vs Sephiroth" &&
               activity->find("timestamps")->find("start")->number == 1762650000 &&
               activity->find("buttons")->items.size() == 1,
           "the activity has both lines, the start time and the button");
    const d012::json::Value bare = parse(rules::set_activity(nullptr, 1, 0, 1));
    expect(bare.find("args") && !bare.find("args")->find("activity"), "no activity clears the status");
    const rules::Activity menu{"In the menus", ""};
    const d012::json::Value no_state = parse(rules::set_activity(&menu, 1, 0, 1));
    expect(!no_state.find("args")->find("activity")->find("state"), "an empty second line is left out");

    const rules::Answer ready = rules::read_answer(R"({"cmd":"DISPATCH","evt":"READY","data":{"v":1}})");
    expect(ready.event == "READY" && ready.command == "DISPATCH" && ready.error.empty(), "READY is read");
    const rules::Answer refused = rules::read_answer(R"({"code":4000,"message":"Invalid Client ID"})");
    expect(refused.error == "Invalid Client ID", "a refusal's reason is read");
    const rules::Answer failed =
        rules::read_answer(R"({"cmd":"SET_ACTIVITY","evt":"ERROR","data":{"code":4000,"message":"bad"},"nonce":"2"})");
    expect(failed.event == "ERROR" && failed.error == "bad", "an error answer is read");
    expect(rules::read_answer("not json").event.empty(), "what is not JSON is nothing");
}

// Local versus: player 2's Customize (host/d012_versus_rules.cpp).
void test_versus_rules() {
    namespace rules = d012::versus_rules;
    using rules::Pad;
    using Trip = rules::Trip;
    using From = Trip::From;

    // Whose pad the game reads in the character room.
    const Trip none;
    expect(rules::room_pad(5, 0u, none) == Pad::Player1, "player 1's pick is player 1's");
    for (std::uint32_t state = 8; state <= 11; ++state)
        expect(rules::room_pad(state, 0u, none) == Pad::Player2, "player 2's pick is player 2's");
    expect(rules::room_pad(13, 0u, none) == Pad::Player1, "the setup screen is player 1's");
    expect(rules::room_pad(52, 0u, none) == Pad::Player1 && rules::room_pad(0, 0u, none) == Pad::Player1,
           "player 1's Customize is player 1's");

    // Player 2's Customize from their pick, as the room goes there and back.
    Trip trip{From::Pick};
    const std::uint32_t from_pick[] = {9, 9, 10, 11, 50, 51, 52, 52, 52, 0};
    for (const std::uint32_t state : from_pick) {
        rules::step(trip, true, state, 0u);
        expect(trip.active() && rules::room_pad(state, 0u, trip) == Pad::Player2,
               "player 2's pad from their pick to Customize and back, state " + std::to_string(state));
    }
    rules::step(trip, true, 8, 0u);
    expect(!trip.active() && rules::room_pad(8, 0u, trip) == Pad::Player2, "back at player 2's pick, the trip is over");

    // From the setup screen: player 1's pad again when the setup is back.
    trip = Trip{From::Setup};
    const std::uint32_t from_setup[] = {13, 14, 15, 50, 51, 52, 1, 2};
    for (const std::uint32_t state : from_setup) rules::step(trip, true, state, 0u);
    expect(trip.active() && trip.opened && rules::room_pad(52, 0u, trip) == Pad::Player2 &&
               rules::room_pad(0, 0u, trip) == Pad::Player2,
           "player 2's pad in Customize from the setup screen");
    expect(rules::room_pad(13, 0u, trip) == Pad::Player1, "the setup's own fade stays player 1's");
    rules::step(trip, true, 12, 0u);
    expect(!trip.active() && rules::room_pad(12, 0u, trip) == Pad::Player1, "back at the setup, player 1's pad");

    // A trip that goes somewhere else ends there; so does one without the room.
    trip = Trip{From::Pick};
    rules::step(trip, true, 4, 0u);
    expect(!trip.active(), "player 2's trip ends if the room goes to player 1's pick instead");
    trip = Trip{From::Setup};
    rules::step(trip, true, 9, 0u);
    expect(!trip.active(), "a trip from the setup screen does not pass through player 2's pick");
    trip = Trip{From::Pick};
    rules::step(trip, true, 52, 0u);
    rules::step(trip, false, 0, 0u);
    expect(!trip.active(), "no room, no trip");

    // Quick Battle (Party): the CPU's party is player 2's pick, 19 only on the way in.
    for (const std::uint32_t state : {18u, 20u, 21u, 22u})
        expect(rules::room_pad(state, 0u, none) == Pad::Player2, "the party's pick is player 2's, state " + std::to_string(state));
    expect(rules::room_pad(19, 20, none) == Pad::Player2, "the wait into the party's pick is player 2's");
    expect(rules::room_pad(19, 28, none) == Pad::Player1 && rules::room_pad(19, 16, none) == Pad::Player1,
           "the wait out of it, to the setup or back to player 1's party, is player 1's");
    expect(rules::room_pad(17, 0, none) == Pad::Player1 && rules::room_pad(29, 0, none) == Pad::Player1,
           "player 1's party and the party setup are player 1's");
    // Which cells Customize takes, as player 1's pick decides.
    expect(rules::customizable(13, 13) && rules::customizable(31, 31), "Cloud and Lightning can be customised");
    expect(!rules::customizable(rules::kChaos, rules::kChaos) &&
               !rules::customizable(rules::kFeralChaosCell, rules::kFeralChaosCell) &&
               !rules::customizable(rules::kLockedCell, 5) && !rules::customizable(rules::kRandomCell, 5) &&
               !rules::customizable(0, 0),
           "Chaos, Feral Chaos, a locked cell, the random cell and no character are refused");
    expect(rules::grid_takes_triangle(rules::kGridInput, 0, 0u), "a grid at rest takes triangle");
    expect(!rules::grid_takes_triangle(400u, 0, 0u) && !rules::grid_takes_triangle(rules::kGridInput, 3, 0u) &&
               !rules::grid_takes_triangle(rules::kGridInput, 0, 0x1000u) &&
               !rules::grid_takes_triangle(rules::kGridInput, 0, 0x4000u),
           "a grid leaving, busy or without input takes nothing");

    // Player 2's pick update: the grid's Customize result is started once.
    expect(rules::pick_customize(0u, 9u, rules::kGridCustomize), "result 5 on player 2's pick is Customize");
    expect(!rules::pick_customize(rules::kFlagFading, 9u, rules::kGridCustomize), "not again while fading out");
    expect(!rules::pick_customize(0u, 10u, rules::kGridCustomize), "not once the pick has moved on");
    expect(!rules::pick_customize(0u, 9u, 2u) && !rules::pick_customize(0u, 9u, 4u),
           "a pick and back are the game's own");

    // The setup screen: player 2's triangle, pressed, is the setup's Customize.
    const std::uint32_t tri = rules::kTriangle;
    expect(rules::setup_triangle(13u, 0u, tri, 0u, 0u), "player 2 presses triangle on the setup screen");
    expect(!rules::setup_triangle(13u, 0u, tri, tri, 0u), "held, not pressed again");
    expect(!rules::setup_triangle(13u, 0u, tri, 0u, tri), "player 1's own triangle is player 1's");
    expect(!rules::setup_triangle(13u, rules::kFlagFading, tri, 0u, 0u), "not while the setup fades out");
    expect(!rules::setup_triangle(9u, 0u, tri, 0u, 0u) && !rules::setup_triangle(5u, 0u, tri, 0u, 0u),
           "only on the setup screen");
    expect(rules::setup_chose_customize(0u, rules::kSetupCustomize), "the setup chose its Customize from the press");
    expect(!rules::setup_chose_customize(0u, 0u) && !rules::setup_chose_customize(0u, 1u),
           "nothing chosen, or Enter Battle, is no Customize");
    expect(!rules::setup_chose_customize(rules::kSetupCustomize, rules::kSetupCustomize),
           "a Customize chosen before the press is not player 2's");
    expect(rules::setup_took_customize(0u, rules::kFlagCustomize | rules::kFlagFading, rules::kBackToSetup),
           "the room took it as Customize");
    expect(!rules::setup_took_customize(0u, 0u, rules::kBackToSetup), "the setup did not take it");
    expect(!rules::setup_took_customize(rules::kFlagCustomize, rules::kFlagCustomize, rules::kBackToSetup),
           "a Customize already under way is not player 2's");
    expect(!rules::setup_took_customize(0u, rules::kFlagCustomize | rules::kFlagRuleset, rules::kBackToSetup) &&
               !rules::setup_took_customize(0u, rules::kFlagCustomize, 4u),
           "the ruleset, or a Customize going elsewhere, is not the setup's");
}


// Story scenes with Japanese voices (host/d012_voice_scenes.cpp).
void test_voice_scenes() {
    namespace vs = d012::voice_scenes;
    using Bytes = std::vector<std::uint8_t>;
    const auto set32 = [](Bytes &b, std::size_t at, std::uint32_t v) {
        if (b.size() < at + 4u) b.resize(at + 4u);
        for (std::size_t i = 0; i < 4u; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8u * i));
    };
    const auto get32 = [](const Bytes &b, std::size_t at) {
        return static_cast<std::uint32_t>(b[at]) | (static_cast<std::uint32_t>(b[at + 1u]) << 8u) |
               (static_cast<std::uint32_t>(b[at + 2u]) << 16u) | (static_cast<std::uint32_t>(b[at + 3u]) << 24u);
    };
    const auto line = [](std::u16string_view words) { return std::u16string(words) + u'\0'; };

    const auto scenes = vs::scenes();
    expect(scenes.size() == 302u && std::is_sorted(scenes.begin(), scenes.end()), "302 story scenes, sorted");
    for (const vs::VoiceName &v : vs::voice_names()) {
        const std::string_view japanese(v.japanese), english(v.english);
        expect(japanese.starts_with("voice/jp/") && english.starts_with("voice/jp/") && english.size() < 0x40u &&
                   japanese != english,
               std::string("a voice line's English name: ") + v.japanese);
    }

    // A language container, written and read back.
    std::vector<std::vector<std::u16string>> tables(2, std::vector<std::u16string>(150));
    tables[0][3] = line(u"Three");
    tables[1][3] = line(u"Trois");
    tables[0][149] = line(u"Last\nrow");
    const Bytes container = vs::write_container(tables);
    expect(vs::read_container(container) == tables, "a language container is read as it was written");

    // The narration: a line holding what the English release split over two
    // lines shows both, a row each; line 70 has line 72's words.
    std::vector<std::vector<std::u16string>> narration(2, std::vector<std::u16string>(146));
    for (auto &table : narration) {
        table[94] = line(u"But to him, perhaps they are not comrades at all.");
        table[95] = line(u"They may be mere strangers\naccidentally born in the same world.");
        table[70] = line(u"She travels in search of her crystal.");
        table[72] = line(u"The boy has spoken little.");
    }
    narration[1][95].clear();  // a language without the second part keeps its line
    const auto fixed = vs::read_container(vs::fix_text_file("text/M_/event/MOVIE/M_000.bin", vs::write_container(narration)));
    expect(fixed.size() == 2u && fixed[0][94] == line(u"But to him, perhaps they are not comrades at all.\nThey may be "
                                                      u"mere strangers\naccidentally born in the same world."),
           "a narration line holds the lines it was split into");
    expect(fixed.size() == 2u && fixed[0][95] == narration[0][95], "the second part stays as it was");
    expect(fixed.size() == 2u && fixed[0][70] == line(u"The boy has spoken little."), "line 70 has line 72's words");
    expect(fixed.size() == 2u && fixed[1][94] == narration[1][94], "a part missing, the line stays");

    // A line the Japanese script shows that has no English: the port's
    // words, in a table where it has none.
    std::vector<std::vector<std::u16string>> chapter(2, std::vector<std::u16string>(120));
    chapter[0][71] = line(u"What!?");
    chapter[1][70] = line(u"Hmm.");
    const auto filled = vs::read_container(vs::fix_text_file("text/M_/event/RT/R_UP01.bin", vs::write_container(chapter)));
    expect(filled.size() == 2u && filled[0][70] == line(u"Hmph.") && filled[0][71] == line(u"What!?"),
           "a line with no English gets the port's words");
    expect(filled.size() == 2u && filled[1][70] == line(u"Hmm."), "a line with words keeps them");
    expect(vs::fix_text_file("text/M_/event/RT/R_UP01.bin", Bytes{1, 2, 3}).empty(), "not a container: nothing");

    // A scene: the Japanese archive, the game's text, and the voice table's
    // line the game has no name for named as voice_names() says.
    const vs::VoiceName renamed = vs::voice_names().front();
    Bytes script(0x4C0u + 2u * 0x40u, 0u);
    for (std::size_t t = 0; t < 30u; ++t) set32(script, 0x408u + 4u * t, 0x48Cu);  // 30 empty tracks
    set32(script, 0x484u, 0x4C0u);
    const std::string kept = "voice/jp/event/up02/up02_103_001ti.at3";
    std::copy_n(renamed.japanese, std::string_view(renamed.japanese).size(), script.begin() + 0x4C0);
    std::copy(kept.begin(), kept.end(), script.begin() + 0x500);
    std::vector<std::vector<std::u16string>> their_lines(1, std::vector<std::u16string>(80, line(u"jp")));
    std::vector<std::vector<std::u16string>> our_lines(1, std::vector<std::u16string>(80, line(u"English")));
    our_lines[0][78].clear();  // a line the English text lacks
    const auto archive = [&](const Bytes &first, const Bytes &text) {
        Bytes out = {'A', 'R', 'C', 1};
        set32(out, 4u, 3u);
        out.resize(16u + 3u * 16u);
        const Bytes masw(12u, 7u);
        const std::pair<const char *, const Bytes *> parts[] = {{"R_UP", &first}, {"text", &text}, {"masw", &masw}};
        for (std::size_t i = 0; i < 3u; ++i) {
            set32(out, 16u + 16u * i, 1u);
            std::copy_n(parts[i].first, 4u, out.begin() + 16u + 16u * i + 4u);
            set32(out, 16u + 16u * i + 8u, static_cast<std::uint32_t>(out.size()));
            set32(out, 16u + 16u * i + 12u, static_cast<std::uint32_t>(parts[i].second->size()));
            out.insert(out.end(), parts[i].second->begin(), parts[i].second->end());
        }
        return out;
    };
    const Bytes japanese = archive(script, vs::write_container(their_lines));
    const Bytes own = archive(Bytes(40u, 9u), vs::write_container(our_lines));
    std::vector<vs::Voice> voices;
    const Bytes merged = vs::merge_scene(0x17C283AEu, japanese, own, voices);
    expect(merged.size() > 64u && get32(merged, 4u) == 3u, "a merged scene has the Japanese archive's chunks");
    if (merged.size() > 64u) {
        const auto chunk = [&](std::size_t i) {
            const std::uint32_t at = get32(merged, 16u + 16u * i + 8u), size = get32(merged, 16u + 16u * i + 12u);
            return Bytes(merged.begin() + at, merged.begin() + at + size);
        };
        for (std::size_t i = 0; i < 3u; ++i)
            expect(get32(merged, 16u + 16u * i + 8u) % 16u == 0u, "each chunk from a 16-byte boundary");
        Bytes want = script;
        std::fill(want.begin() + 0x4C0, want.begin() + 0x500, 0u);
        std::copy_n(renamed.english, std::string_view(renamed.english).size(), want.begin() + 0x4C0);
        expect(chunk(0) == want, "the Japanese script, its line named as the game has it");
        const auto text = vs::read_container(chunk(1));
        expect(text.size() == 1u && text[0][0] == line(u"English") && text[0][78] == line(u"Gaaah!"),
               "the game's text, the line it lacks filled");
        expect(chunk(2) == Bytes(12u, 7u), "the other chunks the Japanese archive's");
    }
    expect(voices.size() == 2u && voices[0].asked == renamed.english && voices[0].japanese == renamed.japanese &&
               voices[1].asked == kept && voices[1].japanese == kept,
           "the scene's voice lines, as the game asks for them and as the Japanese disc has them");
    expect(vs::merge_scene(0x17C283AEu, japanese, archive(Bytes(40u, 9u), Bytes{1, 2}), voices).empty(),
           "the game's text not a container: nothing");
    expect(vs::merge_scene(0x17C283AEu, Bytes(japanese.begin(), japanese.begin() + 100), own, voices).empty(),
           "a short archive: nothing");
}

// A story movie with the other disc's sound (host/d012_voice_scenes.cpp).
void test_movie_splice() {
    namespace vs = d012::voice_scenes;
    using Bytes = std::vector<std::uint8_t>;
    // A movie: a 2,048-byte header, then packs of a pack header and one PES
    // packet each (video, or sound with its 4 bytes), padded to 2,048 bytes.
    // `sound` is cut into packets of the sizes in `cuts`, between video packs.
    const auto movie = [](const Bytes &sound, const std::vector<std::size_t> &cuts, std::uint8_t video) {
        Bytes out(2048u, 0u);
        std::copy_n("PSMF0015", 8u, out.begin());
        out[10] = 0x08u;  // packs from 0x800
        const auto pack = [&](std::uint8_t id, const Bytes &body) {
            const std::size_t at = out.size();
            out.resize(at + 2048u, 0u);
            const std::uint8_t head[14] = {0, 0, 1, 0xBA, 0x44, 0, 4, 0, 4, 1, 1, 0x89, 0xC3, 0xF8};
            std::copy_n(head, 14u, out.begin() + at);
            std::size_t i = at + 14u;
            const auto pes = [&](std::uint8_t pes_id, const Bytes &data) {
                out[i] = 0; out[i + 1] = 0; out[i + 2] = 1; out[i + 3] = pes_id;
                out[i + 4] = static_cast<std::uint8_t>(data.size() >> 8u);
                out[i + 5] = static_cast<std::uint8_t>(data.size());
                std::copy(data.begin(), data.end(), out.begin() + i + 6);
                i += 6u + data.size();
            };
            pes(id, body);
            pes(0xBE, Bytes(at + 2048u - i - 6u, 0xFFu));  // padding to the pack's end
        };
        std::size_t taken = 0, number = 0;
        for (const std::size_t cut : cuts) {
            pack(0xE0, Bytes(300u + 7u * number, video));
            Bytes body = {0x81, 0x80, 5, 0x21, 0, 1, 0, static_cast<std::uint8_t>(++number)};
            const Bytes four = {0, static_cast<std::uint8_t>(number), 0, 0xF3};
            body.insert(body.end(), four.begin(), four.end());
            body.insert(body.end(), sound.begin() + taken, sound.begin() + taken + cut);
            taken += cut;
            pack(0xBD, body);
        }
        const std::uint8_t end[4] = {0, 0, 1, 0xB9};
        out.insert(out.end(), end, end + 4);
        return out;
    };
    Bytes ours_sound(3000u), their_sound(3010u);
    for (std::size_t i = 0; i < ours_sound.size(); ++i) ours_sound[i] = static_cast<std::uint8_t>(i % 7u);
    for (std::size_t i = 0; i < their_sound.size(); ++i) their_sound[i] = static_cast<std::uint8_t>(100u + i % 101u);
    const Bytes ours = movie(ours_sound, {1000, 1000, 1000}, 0x11);
    const Bytes theirs = movie(their_sound, {700, 1500, 800, 10}, 0x22);  // a frame longer, cut elsewhere
    const auto reader = [](const Bytes &file) {
        return [&file](std::uint64_t at, std::span<std::uint8_t> out) -> std::size_t {
            if (at >= file.size()) return 0u;
            const std::size_t n = std::min<std::size_t>(out.size(), file.size() - at);
            std::copy_n(file.begin() + static_cast<std::ptrdiff_t>(at), n, out.begin());
            return n;
        };
    };
    auto splice = vs::MovieSplice::open(reader(ours), reader(theirs));
    expect(splice != nullptr, "two movies open for a splice");
    if (!splice) return;
    // Read as a player would, in uneven pieces.
    Bytes spliced;
    for (std::uint64_t at = 0; at < ours.size();) {
        Bytes piece(std::min<std::size_t>(777u + at % 1000u, ours.size() - at));
        const std::size_t got = splice->read(at, piece);
        spliced.insert(spliced.end(), piece.begin(), piece.begin() + static_cast<std::ptrdiff_t>(got));
        at += got;
        if (got == 0u) break;
    }
    expect(!splice->broken(), "the movie's packs are as expected");
    expect(spliced.size() == ours.size(), "the spliced movie is the game's size");
    // Where each packet's sound is in the game's file: the same places hold
    // their sound, and every other byte is the game's.
    Bytes want = ours;
    std::size_t taken = 0;
    for (std::size_t at = 2048u; at + 2048u <= ours.size(); at += 2048u) {
        if (ours[at + 17u] != 0xBDu) continue;
        const std::size_t length = (std::size_t(ours[at + 18u]) << 8u) | ours[at + 19u];
        const std::size_t sound = at + 14u + 9u + 5u + 4u, end = at + 14u + 6u + length;
        for (std::size_t i = sound; i < end; ++i) want[i] = their_sound[taken++];
    }
    expect(taken == ours_sound.size() && spliced == want,
           "the game's movie with their sound byte for byte, each packet's 4 bytes the game's");
    expect(vs::MovieSplice::open(reader(ours), reader(Bytes(4096u, 0u))) == nullptr, "not a movie: no splice");
}

// The battle relay's STATUS query and answer (d012_battle_relay.hpp).
void test_relay_status() {
    namespace relay = d012::net::relay;
    const std::string query = relay::status_query(0xA1B2C3D4u, "0123456789abcdef-std");
    expect(query.size() >= relay::kStatusAnswer, "relay status: a query is at least as long as its answer");
    expect(query.compare(0, 4, "D0NL") == 0 && static_cast<std::uint8_t>(query[4]) == relay::kStatus,
           "relay status: a query is a STATUS packet");
    expect(static_cast<std::uint8_t>(query[relay::kHead + 4u]) == 20u &&
               query.compare(relay::kStatusQuery, 20u, "0123456789abcdef-std") == 0,
           "relay status: the query carries the version tag");
    expect(relay::status_query(1u, "").size() == relay::kStatusAnswer, "relay status: an empty tag is padded");

    // An answer as the relay writes it: the query's number, then the counts.
    std::string answer(query, 0u, relay::kHead);
    for (const std::uint32_t value : {0xA1B2C3D4u, 3u, 7u})
        for (int i = 0; i < 4; ++i) answer.push_back(static_cast<char>((value >> (8 * i)) & 0xFFu));
    const auto *bytes = reinterpret_cast<const std::uint8_t *>(answer.data());
    relay::Counts counts;
    expect(relay::read_status_answer(bytes, answer.size(), 0xA1B2C3D4u, counts) && counts.version == 3u &&
               counts.all == 7u,
           "relay status: an answer reads back");
    expect(!relay::read_status_answer(bytes, answer.size(), 0x01020304u, counts),
           "relay status: an answer to another query is not taken");
    expect(!relay::read_status_answer(bytes, answer.size() - 1u, 0xA1B2C3D4u, counts),
           "relay status: a short answer is not taken");
}

#if defined(_WIN32)
using TestSocket = SOCKET;
constexpr TestSocket kNoTestSocket = INVALID_SOCKET;
void close_test_socket(TestSocket s) { closesocket(s); }
#else
using TestSocket = int;
constexpr TestSocket kNoTestSocket = -1;
void close_test_socket(TestSocket s) { close(s); }
#endif

// A UDP socket on 127.0.0.1 and a port the system picks, and its address.
TestSocket loopback_socket(sockaddr_in &at) {
    const TestSocket s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == kNoTestSocket) return s;
    at = sockaddr_in{};
    at.sin_family = AF_INET;
    at.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(s, reinterpret_cast<const sockaddr *>(&at), sizeof at) != 0) {
        close_test_socket(s);
        return kNoTestSocket;
    }
#if defined(_WIN32)
    int size = sizeof at;
#else
    socklen_t size = sizeof at;
#endif
    getsockname(s, reinterpret_cast<sockaddr *>(&at), &size);
    return s;
}

// The datagrams waiting on `s`, after up to `ms` for the first.
std::vector<std::string> take_datagrams(TestSocket s, int ms) {
    std::vector<std::string> out;
    for (;;) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(s, &set);
        timeval time{0, (out.empty() ? ms : 0) * 1000};
        if (select(static_cast<int>(s + 1), &set, nullptr, nullptr, &time) <= 0) return out;
        char data[2048];
        const int got = static_cast<int>(recv(s, data, sizeof data, 0));
        if (got <= 0) return out;
        out.emplace_back(data, static_cast<std::size_t>(got));
    }
}

// Direct battles' packets (d012_battle_relay.hpp), and a relay on 127.0.0.1
// introducing two fighters, or not when one wants the relay.
void test_relay_introductions() {
    namespace relay = d012::net::relay;
    const auto bytes = [](const std::string &s) { return reinterpret_cast<const std::uint8_t *>(s.data()); };
    const relay::Endpoint home{htonl(0xC0A80102u), htons(27420)};    // 192.168.1.2
    const relay::Endpoint seen{htonl(0xCB007107u), htons(40001)};    // 203.0.113.7
    const relay::Endpoint loop{htonl(0x7F000001u), htons(50000)};    // 127.0.0.1

    const std::string ask = relay::ask(0x123456u, true, home);
    bool direct = false;
    relay::Endpoint local;
    expect(ask.size() == relay::kAskSize && static_cast<std::uint8_t>(ask[4]) == relay::kAsk &&
               static_cast<std::uint8_t>(ask[5]) == 0x56u && static_cast<std::uint8_t>(ask[7]) == 0x12u,
           "relay intro: an ASK carries its kind and the battle's tag");
    expect(relay::read_ask(bytes(ask), ask.size(), direct, local) && direct && local == home,
           "relay intro: an ASK reads back");
    const std::string no = relay::ask(1u, false, home);
    expect(relay::read_ask(bytes(no), no.size(), direct, local) && !direct, "relay intro: Always through the server");
    expect(!relay::read_ask(bytes(ask), ask.size() - 1u, direct, local), "relay intro: a short ASK is not taken");

    relay::Introduction intro{true, seen, home}, back;
    const std::string text = relay::introduction(0x123456u, intro);
    expect(text.size() == relay::kIntroSize && relay::read_introduction(bytes(text), text.size(), back) &&
               back.direct && back.seen == seen && back.local == home,
           "relay intro: an INTRO reads back");
    const std::string none = relay::introduction(1u, relay::Introduction{false, seen, home});
    expect(relay::read_introduction(bytes(none), none.size(), back) && !back.direct && back.seen == relay::Endpoint{} &&
               back.local == relay::Endpoint{},
           "relay intro: an INTRO without a direct path gives no address");
    expect(relay::may_introduce(seen, home) && relay::may_introduce(loop, loop) && !relay::may_introduce(loop, seen),
           "relay intro: an address on the relay's own computer is not given to another");

#if defined(_WIN32)
    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        expect(false, "relay intro: sockets");
        return;
    }
#endif
    constexpr std::uint16_t kPort = 27497;
    relay::Options options;
    options.port = kPort;
    options.loopback_only = true;
    options.name = "test-relay";
    relay::start(options);
    sockaddr_in to{};
    to.sin_family = AF_INET;
    to.sin_port = htons(kPort);
    to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sockaddr_in at_a{}, at_b{};
    const TestSocket a = loopback_socket(at_a), b = loopback_socket(at_b);
    expect(a != kNoTestSocket && b != kNoTestSocket, "relay intro: two loopback sockets");
    if (a == kNoTestSocket || b == kNoTestSocket) return;
    const auto say = [&](TestSocket s, const std::string &packet) {
        sendto(s, packet.data(), static_cast<int>(packet.size()), 0, reinterpret_cast<const sockaddr *>(&to), sizeof to);
    };
    // Each fighter asks until its INTRO comes; everything else that comes is
    // kept to check that no ASK was passed on.
    const auto introduce = [&](std::uint32_t tag, bool a_direct, bool b_direct, relay::Introduction &for_a,
                               relay::Introduction &for_b, bool &other) {
        bool got_a = false, got_b = false;
        other = false;
        const relay::Endpoint local_a{htonl(0x0A000001u), at_a.sin_port}, local_b{htonl(0x0A000002u), at_b.sin_port};
        for (int i = 0; i < 60 && !(got_a && got_b); ++i) {
            if (!got_a) say(a, relay::ask(tag, a_direct, local_a));
            if (!got_b) say(b, relay::ask(tag, b_direct, local_b));
            for (const auto &[s, intro, got] : {std::tuple{a, &for_a, &got_a}, std::tuple{b, &for_b, &got_b}})
                for (const std::string &d : take_datagrams(s, 50)) {
                    relay::Introduction read;
                    if (relay::read_introduction(bytes(d), d.size(), read) && d.compare(5, 3, relay::ask(tag, true, {}), 5, 3) == 0) {
                        *intro = read;
                        *got = true;
                    } else {
                        other = true;
                    }
                }
        }
        return got_a && got_b;
    };
    relay::Introduction for_a, for_b;
    bool other = false;
    expect(introduce(0x0A0B0Cu, true, true, for_a, for_b, other), "relay intro: both fighters introduced");
    expect(for_a.direct && for_a.seen == relay::Endpoint{at_b.sin_addr.s_addr, at_b.sin_port} &&
               for_a.local == relay::Endpoint{htonl(0x0A000002u), at_b.sin_port},
           "relay intro: the first fighter is given the second's addresses");
    expect(for_b.direct && for_b.seen == relay::Endpoint{at_a.sin_addr.s_addr, at_a.sin_port} &&
               for_b.local == relay::Endpoint{htonl(0x0A000001u), at_a.sin_port},
           "relay intro: the second fighter is given the first's");
    expect(!other, "relay intro: no ASK is passed on to the other fighter");
    expect(introduce(0x0A0B0Du, true, false, for_a, for_b, other) && !for_a.direct && !for_b.direct &&
               for_a.seen == relay::Endpoint{} && for_b.seen == relay::Endpoint{},
           "relay intro: with Always through the server on one side, neither is given an address");
    // The battle's own packets still pass between the two (INTROs answering
    // the last ASKs cleared first).
    take_datagrams(a, 200);
    take_datagrams(b, 200);
    std::string input(relay::kHead + 4u, '\0');
    std::copy_n(relay::kMagic, 4, input.begin());
    input[4] = 3;
    input[5] = 0x0C;
    input[6] = 0x0B;
    input[7] = 0x0A;
    say(a, input);
    const std::vector<std::string> passed = take_datagrams(b, 500);
    expect(passed.size() == 1u && passed[0] == input, "relay intro: an INPUT still passes through the relay");
    close_test_socket(a);
    close_test_socket(b);
}

// Play online's address file (d012_online_rules.hpp, online/server.json).
void test_online_rules() {
    namespace rules = d012::online_rules;
    std::string note;
    expect(rules::address_in(R"({"address": "play.example.net"})", note) == "play.example.net" && note.empty(),
           "online file: a host name");
    expect(rules::address_in(R"({"address": "203.0.113.7:27312", "note": "spare"})", note) == "203.0.113.7:27312",
           "online file: an address with a port, other members left alone");
    expect(rules::address_in(R"({"address": "[2001:db8::1]:27312"})", note) == "[2001:db8::1]:27312",
           "online file: a bracketed IPv6 address");
    expect(rules::address_in(R"({"address": ""})", note).empty() && note == rules::kNoServerYet,
           "online file: no server yet, said so");
    for (const char *bad : {"", "not json", R"({"server": "x"})", R"({"address": 5})",
                            R"({"address": "a b"})", R"({"address": "x;rm"})"})
        expect(rules::address_in(bad, note).empty() && !note.empty() && note != rules::kNoServerYet,
               std::string("online file: refused: ") + bad);
    std::ifstream file(std::filesystem::path(D012_TESTS_DIR) / ".." / ".." / "online" / "server.json");
    const std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    rules::address_in(text, note);
    expect(!text.empty() && (note.empty() || note == rules::kNoServerYet), "online file: the repository's own reads");
}

// Two copies with rollback played out in time alone: each runs its frames to
// its own clock without waiting, sends its input for `delay` frames on, and
// holds itself back as FrameBalance says. What it returns: how far apart the
// copies ran each frame, in ms.
struct BalanceRun {
    int frames = 3000;
    int one_way_us = 60000;
    int stall_at = -1;      // copy 1 stops for `stall_us` on this frame
    int stall_us = 0;
    int frame_us_0 = 33367;
    int frame_us_1 = 33367;
    bool balance = true;
    long long most_hold_us = 0;  // the most either was held on one frame
};

std::vector<double> play_balance(BalanceRun &run) {
    using d012::net::FrameBalance;
    using Clock = FrameBalance::Clock;
    constexpr std::uint64_t delay = 1u;
    struct Packet {
        long long at;
        std::uint64_t frame;
        std::int16_t slack;
    };
    struct Copy {
        FrameBalance balance;
        std::map<std::uint64_t, Clock::time_point> ran, came;
        std::vector<Packet> coming;
        long long next = 0;
        std::uint64_t frame = 0;
        std::vector<long long> times;
    } copies[2];
    const auto when = [](long long us) { return Clock::time_point{} + std::chrono::microseconds(us); };
    const std::uint64_t last = static_cast<std::uint64_t>(run.frames);
    while (copies[0].frame < last || copies[1].frame < last) {
        const int me = copies[1].frame >= last || (copies[0].frame < last && copies[0].next <= copies[1].next) ? 0 : 1;
        Copy &c = copies[me];
        Copy &other = copies[1 - me];
        const long long now = c.next;
        for (auto it = c.coming.begin(); it != c.coming.end();) {
            if (it->at > now) {
                ++it;
                continue;
            }
            c.came.emplace(it->frame, when(it->at));
            c.balance.hear(it->slack);
            it = c.coming.erase(it);
        }
        if (c.frame >= delay) c.ran[c.frame] = when(now);
        const auto slacks = d012::net::take_slacks(c.ran, c.came, c.frame);
        for (const auto slack : slacks) c.balance.note(slack);
        long long hold = 0;
        if (run.balance && !slacks.empty()) hold = c.balance.hold().count();
        run.most_hold_us = std::max(run.most_hold_us, hold);
        other.coming.push_back({now + run.one_way_us, c.frame + delay, c.balance.sent_slack()});
        c.times.push_back(now);
        c.next = now + (me == 0 ? run.frame_us_0 : run.frame_us_1) + hold;
        if (me == 1 && static_cast<int>(c.frame) == run.stall_at) c.next += run.stall_us;
        ++c.frame;
    }
    std::vector<double> apart;
    for (std::size_t i = 0; i < copies[0].times.size(); ++i)
        apart.push_back(static_cast<double>(std::llabs(copies[0].times[i] - copies[1].times[i])) / 1000.0);
    return apart;
}

void test_frame_balance() {
    using d012::net::FrameBalance;
    using d012::net::kNoSlack;
    using Clock = FrameBalance::Clock;
    using std::chrono::milliseconds;
    const auto at = [](int ms) { return Clock::time_point{} + milliseconds(ms); };

    // Which inputs count: one before its frame ran, one after, and neither of
    // the frames still open.
    std::map<std::uint64_t, Clock::time_point> ran{{5u, at(100)}, {6u, at(133)}, {7u, at(166)}};
    std::map<std::uint64_t, Clock::time_point> came{{5u, at(70)}, {6u, at(190)}, {9u, at(195)}};
    const auto slacks = d012::net::take_slacks(ran, came, 7u);
    expect(slacks.size() == 2u && slacks[0] == milliseconds(30) && slacks[1] == milliseconds(-57),
           "frame balance: an input that came after its frame counts, as less than 0");
    expect(ran.size() == 1u && ran.count(7u) == 1u, "frame balance: a frame without its input stays open");
    expect(came.size() == 1u && came.count(9u) == 1u, "frame balance: an input for a frame to come stays");

    // The hold: nothing until both are known, nothing behind, at most the cap.
    FrameBalance balance;
    balance.hear(40);
    for (std::size_t i = 0; i + 1u < FrameBalance::kFrames; ++i) balance.note(milliseconds(-60));
    expect(!balance.known() && balance.sent_slack() == kNoSlack && balance.hold().count() == 0,
           "frame balance: no hold before the slack is known");
    balance.note(milliseconds(-60));
    expect(balance.known() && balance.sent_slack() == -60, "frame balance: late inputs make a slack below 0");
    expect(balance.hold() == FrameBalance::kMostHold, "frame balance: far ahead, the most hold");
    balance.hear(-60);
    expect(balance.hold().count() == 0, "frame balance: level, no hold");
    balance.hear(-80);
    expect(balance.hold().count() == 0, "frame balance: behind, no hold");
    balance.hear(-56);
    expect(balance.hold() == std::chrono::microseconds(62), "frame balance: a sixty-fourth of the difference");

    // A stall of 130 ms on one copy, at a 120 ms round trip with every input
    // late: the copies come level again, as they do not without the hold.
    BalanceRun stall;
    stall.stall_at = 300;
    stall.stall_us = 130000;
    const std::vector<double> apart = play_balance(stall);
    expect(apart[299] < 3.0 && apart[305] > 120.0, "frame balance: the stall puts the copies 130 ms apart");
    expect(apart[900] < 6.0 && apart[2999] < 6.0, "frame balance: level again within 20 s of a stall");
    expect(stall.most_hold_us <= FrameBalance::kMostHold.count(), "frame balance: never more than the most hold");
    BalanceRun without = stall;
    without.balance = false;
    expect(play_balance(without)[2999] > 120.0, "frame balance: without the hold the stall stays");

    // Clocks a thousandth apart would drift a frame every 33 s.
    BalanceRun drift;
    drift.frames = 9000;
    drift.frame_us_1 = 33400;
    const std::vector<double> drifted = play_balance(drift);
    expect(drifted.back() < 6.0, "frame balance: clocks that differ stay level");
    BalanceRun distant = stall;
    distant.one_way_us = 150000;
    expect(play_balance(distant)[2999] < 6.0, "frame balance: level again at a 300 ms round trip");
}

} // namespace

// DLC packs (host/d012_dlc_packs_plan.cpp): made-up packs, files in memory.
namespace dlc_test {

namespace packs = d012::dlc_packs;
using Bytes = std::vector<std::uint8_t>;

struct Stick {
    std::map<std::string, Bytes> contents;  // by the source file's path
    int next = 0;

    packs::SourceFile add(packs::Pack &pack, const std::string &game_path, Bytes bytes) {
        const std::string where = pack.name + "/" + std::to_string(next++);
        packs::SourceFile file{packs::name_crc(game_path), bytes.size(), where};
        contents[where] = std::move(bytes);
        pack.files.push_back(file);
        return file;
    }
    packs::Plan plan(const std::vector<packs::Pack> &list, const packs::Pack &installed = {}) {
        return packs::plan(list, installed, [this](const packs::SourceFile &file) { return contents.at(file.path.string()); });
    }
};

void put_name(Bytes &bytes, std::size_t at, const std::string &name) {
    std::copy(name.begin(), name.end(), bytes.begin() + static_cast<std::ptrdiff_t>(at));
}

// One object table, as DissDlcToolkit writes it.
Bytes object_table(int slot, int id, int character, int costume, int flags, const std::string &model, const std::string &data) {
    Bytes bytes(8u + 0x44u, 0u);
    bytes[0] = 0x0Cu;
    bytes[4] = 1u;
    bytes[8] = static_cast<std::uint8_t>(id);
    bytes[9] = static_cast<std::uint8_t>(id >> 8);
    bytes[8 + 6] = static_cast<std::uint8_t>(character);
    bytes[8 + 7] = static_cast<std::uint8_t>(costume);
    bytes[8 + 8] = static_cast<std::uint8_t>(flags);
    bytes[8 + 9] = static_cast<std::uint8_t>(slot);
    std::fill_n(bytes.begin() + 8 + 0x1C, 8, std::uint8_t{0xFF});
    put_name(bytes, 8u + 0x24u, model);
    put_name(bytes, 8u + 0x34u, data);
    return bytes;
}

std::string table_name(int slot) {
    char text[32];
    std::snprintf(text, sizeof text, "dlc/obj/dlc_%03doe.bin", slot);
    return text;
}

// A costume of Onion Knight (character 5): fighter, assist, model and portrait.
packs::Pack costume_pack(Stick &stick, const std::string &name, int slot, int assist_slot, int costume) {
    packs::Pack pack;
    pack.name = name;
    char model[16], assist[16];
    std::snprintf(model, sizeof model, "P_THR100_%XP", static_cast<unsigned>(costume));
    std::snprintf(assist, sizeof assist, "P_THR100_%XP_A", static_cast<unsigned>(costume));
    stick.add(pack, table_name(slot), object_table(slot, 0x2C0 + slot - 1, 5, costume, 0, model, "p_thr100"));
    stick.add(pack, table_name(assist_slot), object_table(assist_slot, 0x2C0 + assist_slot - 1, 5, costume, 8, assist, "p_thr100_a"));
    stick.add(pack, std::string("obj/") + model + ".gmo", {1, 2, 3});
    stick.add(pack, std::string("obj/") + assist + ".gmo", {4, 5});
    stick.add(pack, std::string("menu/jp/battle/chara_image/") + model + ".gim", {6});
    return pack;
}

const packs::Served *served(const packs::Plan &plan, const std::string &game_path) {
    const std::uint32_t crc = packs::name_crc(game_path);
    for (const packs::Served &file : plan.files)
        if (file.crc == crc) return &file;
    return nullptr;
}

Bytes bgm_table(int slot, const std::vector<std::pair<int, std::string>> &tracks) {
    Bytes bytes(16u + 0x2Cu * tracks.size(), 0u);
    std::copy_n("BENT", 4, bytes.begin());
    bytes[8] = static_cast<std::uint8_t>(tracks.size());
    for (std::size_t i = 0; i < tracks.size(); ++i) {
        const std::size_t row = 16u + 0x2Cu * i;
        bytes[row] = static_cast<std::uint8_t>(tracks[i].first);
        bytes[row + 1u] = static_cast<std::uint8_t>(tracks[i].first >> 8);
        put_name(bytes, row + 0x1Cu, tracks[i].second);
    }
    (void)slot;
    return bytes;
}

packs::Pack music_pack(Stick &stick, const std::string &name, int slot, std::uint8_t mark) {
    packs::Pack pack;
    pack.name = name;
    char table[32], titles[32], track[32];
    std::snprintf(table, sizeof table, "dlc/bgm/dlc_%03d.bin", slot);
    std::snprintf(titles, sizeof titles, "text/jp/dlc/bgm_%03dt.bin", slot);
    std::snprintf(track, sizeof track, "ud_%02d%04x.at3", slot, static_cast<unsigned>(slot << 8));
    stick.add(pack, table, bgm_table(slot, {{slot << 8, track}}));
    stick.add(pack, titles, {mark});
    stick.add(pack, std::string("bgm/") + track, {mark, mark});
    return pack;
}

} // namespace dlc_test

void test_dlc_packs() {
    using namespace dlc_test;
    expect(packs::name_crc("dlc/obj/dlc_001oe.bin") == 0xEAC7BF53u, "the DLC name hash");
    expect(packs::name_crc("DLC/OBJ/DLC_001OE.BIN") == 0xEAC7BF53u, "the DLC name hash ignores case");
    expect(packs::crc_of_file_name("E2734686.EDAT") == 0xE2734686u, "a file name in upper case");
    expect(packs::crc_of_file_name("e2734686.edat") == 0xE2734686u, "a file name in lower case");
    expect(!packs::crc_of_file_name("Nepnep.edat"), "a name that is no hash");

    {  // On its own a pack is served as it was made.
        Stick stick;
        const packs::Pack pack = costume_pack(stick, "one", 200, 201, 5);
        const packs::Plan plan = stick.plan({pack});
        expect(plan.moved.empty() && plan.problems.empty(), "a pack on its own keeps its slots");
        expect(plan.files.size() == pack.files.size(), "a pack on its own: every file served");
        const packs::Served *row = served(plan, table_name(200));
        expect(row != nullptr && row->bytes == stick.contents.at(pack.files[0].path.string()), "a pack on its own: its table unchanged");
    }
    {  // Two packs built for the same slots both arrive.
        Stick stick;
        const packs::Pack first = costume_pack(stick, "first", 200, 201, 5);
        const packs::Pack second = costume_pack(stick, "second", 200, 201, 5);
        const packs::Plan plan = stick.plan({first, second});
        expect(plan.problems.empty(), "two packs: nothing left out");
        expect(plan.files.size() == first.files.size() + second.files.size(), "two packs: every file served");
        const packs::Served *gmo = served(plan, "obj/p_thr100_5p.gmo");
        expect(gmo != nullptr && gmo->pack == "first", "two packs: the first keeps its costume");
        const packs::Served *moved = served(plan, "obj/p_thr100_4p.gmo");
        expect(moved != nullptr && moved->pack == "second", "two packs: the second has the lowest free costume");
        const packs::Served *assist = served(plan, "obj/p_thr100_4p_a.gmo");
        expect(assist != nullptr && assist->pack == "second", "two packs: the assist moves with its fighter");
        expect(served(plan, "menu/jp/battle/chara_image/p_thr100_4p.gim") != nullptr, "two packs: the portrait moves");
        const packs::Served *row = served(plan, table_name(1));
        expect(row != nullptr && row->pack == "second", "two packs: the lowest free object slot");
        if (row != nullptr) {
            expect(row->bytes[8] == 0xC0u && row->bytes[9] == 0x02u, "a moved row has its slot's id");
            expect(row->bytes[8 + 7] == 4u, "a moved row has its new costume");
            expect(row->bytes[8 + 9] == 1u, "a moved row names its slot");
            expect(std::string(reinterpret_cast<const char *>(&row->bytes[8 + 0x24])) == "P_THR100_4P", "a moved row names its model");
        }
    }
    {  // What is on the memory stick keeps its place.
        Stick stick;
        packs::Pack installed;
        installed.name = "stick";
        stick.add(installed, table_name(200), object_table(200, 0x2C0 + 199, 5, 5, 0, "P_THR100_5P", "p_thr100"));
        stick.add(installed, "obj/p_thr100_5p.gmo", {9});
        const packs::Plan plan = stick.plan({costume_pack(stick, "pack", 200, 201, 5)}, installed);
        expect(served(plan, "obj/p_thr100_5p.gmo") == nullptr, "the memory stick's file is not replaced");
        expect(served(plan, "obj/p_thr100_4p.gmo") != nullptr, "a pack moves off the memory stick's costume");
        expect(served(plan, table_name(200)) == nullptr, "a pack moves off the memory stick's slot");
    }
    {  // Nine DLC costumes is all a character has.
        Stick stick;
        std::vector<packs::Pack> list;
        for (int i = 0; i < 10; ++i) list.push_back(costume_pack(stick, "pack" + std::to_string(i), 100 + i * 2, 101 + i * 2, 5));
        const packs::Plan plan = stick.plan(list);
        expect(plan.problems.size() == 1u, "a tenth costume is left out");
        int models = 0;
        for (const packs::Served &file : plan.files) models += file.pack == "pack9";
        expect(models == 0, "nothing of a pack left out is served");
    }
    {  // Two packs changing one of the game's own costumes: the first does.
        Stick stick;
        packs::Pack first, second;
        first.name = "first";
        second.name = "second";
        stick.add(first, table_name(10), object_table(10, 0x13, 5, 1, 0, "P_THR100_1P", "p_thr100"));
        stick.add(first, "obj/p_thr100_1p.gmo", {1});
        stick.add(second, table_name(11), object_table(11, 0x13, 5, 1, 0, "P_THR100_1P", "p_thr100"));
        stick.add(second, "obj/p_thr100_1p.gmo", {2});
        const packs::Plan plan = stick.plan({first, second});
        expect(plan.problems.size() == 1u, "one retail costume, two packs: a problem");
        const packs::Served *gmo = served(plan, "obj/p_thr100_1p.gmo");
        expect(gmo != nullptr && gmo->pack == "first", "one retail costume, two packs: the first's model");
        expect(served(plan, table_name(11)) == nullptr, "one retail costume, two packs: no second row");
    }
    {  // An attachment that moves takes its links with it.
        Stick stick;
        packs::Pack first, second;
        first.name = "first";
        second.name = "second";
        stick.add(first, table_name(60), object_table(60, 0x2C0 + 59, 0, 1, 0, "G_ATT_060", "g_att_060"));
        stick.add(first, "obj/g_att_060.gmo", {1});
        stick.add(first, "obj/g_att_060.objx", {2});
        stick.add(second, table_name(60), object_table(60, 0x2C0 + 59, 0, 1, 0, "G_ATT_060", "g_att_060"));
        stick.add(second, "obj/g_att_060.gmo", {3});
        stick.add(second, "obj/g_att_060.objx", {4});
        Bytes row = object_table(61, 0x2C0 + 60, 5, 6, 0, "P_THR100_6P", "p_thr100");
        row[8 + 0x1C] = static_cast<std::uint8_t>(0x2C0 + 59);
        row[8 + 0x1D] = static_cast<std::uint8_t>((0x2C0 + 59) >> 8);
        stick.add(second, table_name(61), row);
        const packs::Plan plan = stick.plan({first, second});
        expect(plan.problems.empty(), "attachments: nothing left out");
        const packs::Served *gmo = served(plan, "obj/g_att_001.gmo");
        expect(gmo != nullptr && gmo->pack == "second", "attachments: the second is renamed for its slot");
        expect(served(plan, "obj/g_att_001.objx") != nullptr, "attachments: its data file too");
        const packs::Served *linked = served(plan, table_name(61));
        expect(linked != nullptr && linked->bytes[8 + 0x1C] == 0xC0u && linked->bytes[8 + 0x1D] == 0x02u,
               "attachments: the costume's link follows the new id");
    }
    {  // Two music packs for one slot.
        Stick stick;
        const packs::Plan plan = stick.plan({music_pack(stick, "first", 14, 1), music_pack(stick, "second", 14, 2)});
        expect(plan.problems.empty(), "music: nothing left out");
        const packs::Served *table = served(plan, "dlc/bgm/dlc_001.bin");
        expect(table != nullptr && table->pack == "second", "music: the second takes a free slot");
        if (table != nullptr) {
            expect(table->bytes[16] == 0x00u && table->bytes[17] == 0x01u, "music: a free track number");
            expect(std::string(reinterpret_cast<const char *>(&table->bytes[16 + 0x1C])) == "ud_010100.at3", "music: a free file name");
        }
        const packs::Served *titles = served(plan, "text/jp/dlc/bgm_001t.bin");
        expect(titles != nullptr && titles->pack == "second", "music: the titles move with the table");
        const packs::Served *track = served(plan, "bgm/ud_010100.at3");
        expect(track != nullptr && track->pack == "second", "music: the track is served at its new name");
        const packs::Served *kept = served(plan, "bgm/ud_140e00.at3");
        expect(kept != nullptr && kept->pack == "first", "music: the first keeps its track");
    }
    {  // Something that is no object table.
        Stick stick;
        packs::Pack pack;
        pack.name = "bad";
        stick.add(pack, table_name(5), {0, 'P', 'S', 'P', 'E', 'D', 'A', 'T'});
        stick.add(pack, "obj/p_thr100_5p.gmo", {1});
        const packs::Plan plan = stick.plan({pack});
        expect(plan.problems.size() == 1u && plan.files.empty(), "an unreadable pack is left out whole");
    }
}

// The All summons cheat writes the game's 61 summons and nothing after them;
// an empty slot marked as owned is emptied, and no other slot is touched.
void test_summon_slots() {
    namespace slots = d012::summon_slots;
    const std::uint32_t end = slots::kFirstSlot + slots::kSummonCount * 4u;
    std::istringstream code(slots::kAllSummonsCode);
    std::string line;
    int writes = 0;
    while (std::getline(code, line)) {
        std::uint32_t address = 0, shape = 0;
        expect(std::sscanf(line.c_str(), "%x %x", &address, &shape) == 2, "summons code: " + line);
        std::getline(code, line);  // the value
        const std::uint32_t first = 0x08800000u + (address & 0x0FFFFFFFu);
        const std::uint32_t count = shape >> 16u, step = shape & 0xFFFFu;
        expect(first >= slots::kFirstSlot && first < slots::kFirstSlot + 4u, "summons code starts in the first slot");
        expect(count == slots::kSummonCount, "summons code writes each summon");
        expect(step == 4u, "summons code writes one byte of each slot");
        expect(first + (count - 1u) * step < end, "summons code stops at the last summon");
        ++writes;
    }
    expect(writes == 2, "summons code writes uses and flags");

    const slots::Slot marked{slots::kEmptyId, 1u, 2u, 3u};
    expect(slots::marked_but_empty(marked), "an empty slot marked as owned is found");
    const slots::Slot empty = slots::emptied(marked);
    expect(empty.id == slots::kEmptyId && empty.uses == 0u && empty.recharge == 0u && empty.flags == 1u,
           "it is left as a new game leaves an empty slot");
    expect(!slots::marked_but_empty(empty), "an emptied slot is sound");
    expect(!slots::marked_but_empty(slots::Slot{slots::kEmptyId, 0u, 0u, 1u}), "a new game's empty slot is left alone");
    expect(!slots::marked_but_empty(slots::Slot{60u, 10u, 0u, 3u}), "an owned summon is left alone");
}

// The game's memory for the battle cheats: everything is RAM, nothing is code.
class CheatRam final : public portablekit::cheats::Memory {
public:
    bool valid(std::uint32_t address, std::uint32_t size) const override {
        return address >= 0x08800000u && address + size <= 0x0A000000u;
    }
    std::uint8_t read8(std::uint32_t address) const override {
        const auto found = bytes.find(address);
        return found == bytes.end() ? 0u : found->second;
    }
    void write8(std::uint32_t address, std::uint8_t value) override {
        bytes[address] = value;
        written.push_back(address);
    }
    bool code(std::uint32_t, std::uint32_t) const override { return false; }

    void put32(std::uint32_t address, std::uint32_t value) {
        for (std::uint32_t i = 0; i < 4u; ++i) bytes[address + i] = static_cast<std::uint8_t>(value >> (8u * i));
    }
    std::uint32_t get32(std::uint32_t address) const {
        std::uint32_t value = 0u;
        for (std::uint32_t i = 0; i < 4u; ++i) value |= static_cast<std::uint32_t>(read8(address + i)) << (8u * i);
        return value;
    }

    std::map<std::uint32_t, std::uint8_t> bytes;
    std::vector<std::uint32_t> written;
};

portablekit::cheats::Cheat cheat_of(const char *code) {
    std::istringstream lines(code);
    std::string text = "_C1 test\n", line;
    while (std::getline(lines, line)) text += "_L " + line + "\n";
    const portablekit::cheats::CheatFile file = portablekit::cheats::parse(text, "ULUS10566");
    expect(file.cheats.size() == 1u && file.problems == 0u, std::string("the code reads: ") + code);
    return file.cheats.empty() ? portablekit::cheats::Cheat{} : file.cheats[0];
}

// Each battle cheat writes the fighter's own status block, whatever the word
// the public list's codes go through (0x08B633B0) holds: it is 0 in some
// battles, which is why Defeat with Select sometimes did nothing.
void test_battle_cheats() {
    namespace codes = d012::battle_cheat_codes;
    constexpr std::uint32_t kManagerPointer = 0x089D6A10u, kManager = 0x08B87668u, kListWord = 0x08B633B0u;
    constexpr std::uint32_t kPlayer = 0x09749190u, kOpponent = 0x09756A00u;
    constexpr std::uint32_t kPlayerStatus = 0x09765050u, kOpponentStatus = 0x09765E70u;
    const auto battle = [&](CheatRam &ram, std::uint32_t list_word, bool fighters) {
        ram.put32(kManagerPointer, kManager);
        ram.put32(kListWord, list_word);
        ram.put32(kManager + 0x38u, fighters ? kPlayer : 0u);
        ram.put32(kManager + 0x3Cu, fighters ? kOpponent : 0u);
        ram.put32(kPlayer + 0x52Cu, kPlayerStatus);
        ram.put32(kOpponent + 0x52Cu, kOpponentStatus);
    };
    const struct {
        const char *name, *code;
        std::uint32_t status, field, value;
    } cases[] = {
        {"Level 100", codes::kLevel100, kPlayerStatus, 0x00u, 100u},
        {"Infinite HP", codes::kInfiniteHp, kPlayerStatus, 0x08u, 0u},
        {"Max HP", codes::kMaxHp, kPlayerStatus, 0x0Cu, 999999u},
        {"Bravery", codes::kBravery, kPlayerStatus, 0x14u, 9999u},
        {"Infinite EX", codes::kInfiniteEx, kPlayerStatus, 0x18u, 0x461C4000u},
        {"Infinite Assist", codes::kInfiniteAssist, kPlayerStatus, 0x1Cu, 0x461C4000u},
        {"Opponent Max HP", codes::kEnemyMaxHp, kOpponentStatus, 0x0Cu, 100u},
        {"Opponent Bravery", codes::kEnemyBravery, kOpponentStatus, 0x14u, 0u},
        {"Opponent No EX", codes::kEnemyNoEx, kOpponentStatus, 0x18u, 0u},
        {"Opponent No Assist", codes::kEnemyNoAssist, kOpponentStatus, 0x1Cu, 0u},
        {"Defeat with Select", codes::kEnemyDefeat, kOpponentStatus, 0x08u, 999999u},
    };
    constexpr std::uint32_t kSelect = 0x1u;
    for (const auto &c : cases) {
        const portablekit::cheats::Cheat cheat = cheat_of(c.code);
        const std::string name = c.name;
        // A battle in which the list's word is empty, and one in which it is elsewhere.
        for (const std::uint32_t list_word : {0u, 0x09800000u}) {
            CheatRam ram;
            battle(ram, list_word, true);
            ram.put32(c.status + c.field, 0x12345678u);
            (void)portablekit::cheats::run(cheat, ram, kSelect);
            expect(ram.get32(c.status + c.field) == c.value, name + " writes the fighter's status block");
            expect(ram.written.size() == 4u, name + " writes one word and nothing else");
        }
        // In the menus there are no fighters.
        CheatRam menus;
        battle(menus, 0x09800000u, false);
        (void)portablekit::cheats::run(cheat, menus, kSelect);
        expect(menus.written.empty(), name + " writes nothing outside a battle");
    }
    // Select not held: the code's every line is skipped.
    CheatRam idle;
    battle(idle, 0u, true);
    (void)portablekit::cheats::run(cheat_of(codes::kEnemyDefeat), idle, 0u);
    expect(idle.written.empty(), "Defeat with Select writes nothing while Select is up");
}

int main(int argc, char **argv) {
    if (argc == 2 && std::string(argv[1]) == "--print-cases") {
        test_shared_cases(true);
        return 0;
    }
    test_writer();
    test_model();
    test_manifest();
    test_checks();
    test_saving();
    test_shared_cases(false);
    test_names();
    test_suggest();
    test_undo();
    test_psp_iso();
    test_update_rules();
    test_no_window_rules();
    test_discord_rules();
    test_versus_rules();
    test_voice_scenes();
    test_movie_splice();
    test_relay_status();
    test_relay_introductions();
    test_online_rules();
    test_dlc_packs();
    test_frame_balance();
    test_summon_slots();
    test_battle_cheats();
    if (g_failures == 0) std::cout << "all passed\n";
    return g_failures == 0 ? 0 : 1;
}
