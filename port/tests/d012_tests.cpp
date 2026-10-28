// The port's own tests, run by tools/port/pk_ctest.cmd <build dir>: the game's
// manifest rules (host/d012_mod_manifest.cpp), the JSON writer and the
// Battle BGM Maker's model. No game is needed. The expected texts of the
// writer and the model are what the Python tool (port/tools/bgm_maker) writes
// for the same documents. MIT licence; see port/README.md.

#include "checks.hpp"
#include "d012_json.hpp"
#include "d012_mod_manifest.hpp"
#include "model.hpp"
#include "undo.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
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

void test_undo() {
    d012::bgm_maker::UndoStack<int> undo;
    undo.reset(1);
    expect(!undo.can_undo(), "nothing to undo at first");
    expect(undo.commit(2) && !undo.commit(2) && undo.commit(3), "commits only changes");
    expect(undo.undo() == 2 && undo.undo() == 1 && !undo.undo(), "undo back to the start");
    expect(undo.redo() == 2, "redo");
    expect(undo.commit(5) && !undo.can_redo(), "a new step drops the redo");
}

} // namespace

int main() {
    test_writer();
    test_model();
    test_manifest();
    test_checks();
    test_undo();
    if (g_failures == 0) std::cout << "all passed\n";
    return g_failures == 0 ? 0 : 1;
}
