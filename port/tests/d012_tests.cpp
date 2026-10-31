// The port's own tests, run by tools/port/pk_ctest.cmd <build dir>: the game's
// manifest rules (host/d012_mod_manifest.cpp) and the shared manifest cases
// (manifest_cases.json), the JSON writer, and the Battle BGM Maker's model,
// checks, names, saving and loop suggester. No game is needed. The expected
// texts of the writer and the model are what the Python tool
// (port/tools/bgm_maker) writes for the same documents. MIT licence; see
// port/README.md.

#include "checks.hpp"
#include "d012_json.hpp"
#include "d012_mod_manifest.hpp"
#include "model.hpp"
#include "names.hpp"
#include "psp_iso.hpp"
#include "saving.hpp"
#include "suggest.hpp"
#include "undo.hpp"

#include <algorithm>
#include <cmath>
#include <tuple>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
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

} // namespace

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
    if (g_failures == 0) std::cout << "all passed\n";
    return g_failures == 0 ? 0 : 1;
}
