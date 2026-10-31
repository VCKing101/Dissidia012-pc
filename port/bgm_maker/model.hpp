#pragma once

// The mod being edited: what mod.json says, as far as it can be read, so a
// broken manifest can still be opened and fixed. Members this version does
// not know are kept and written back. The editor's model of the Python tool
// (port/tools/bgm_maker/bgmmaker/manifest.py, model_from_manifest and
// build_manifest), with the same member order, so both write the same file.
// MIT licence; see port/README.md.

#include "d012_json.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace d012::bgm_maker {

using Extra = std::vector<std::pair<std::string, json::Value>>;

struct LoopModel {
    enum class Mode { Auto, Whole, None, Custom };
    Mode mode{Mode::Auto};
    // A custom loop in samples of the file at its own rate...
    std::int64_t start{};
    std::optional<std::int64_t> end;  // none: the end of the file
    // ...or, as a manifest may give it, in seconds (kept until the file's rate
    // is known and it can become samples).
    bool in_seconds{};
    double start_seconds{};
    std::optional<double> end_seconds;
    bool operator==(const LoopModel &) const = default;
};

struct TrackModel {
    std::string key;
    std::string title;
    std::string file;  // relative to the mod's folder, with /
    LoopModel loop;
    Extra extra;
    // Not in the manifest: the key still follows the title (a track added
    // since the last save whose key was not typed). Once saved, players'
    // choices refer to the key, so it stays.
    bool auto_key{};
    bool operator==(const TrackModel &) const = default;
};

struct SeriesModel {
    std::string key;
    std::string name;
    std::string short_name;  // "short", for the PSP / PPSSPP export; empty: none (made from the name)
    std::int64_t order{};
    std::vector<TrackModel> tracks;
    Extra extra;
    bool auto_key{};  // as TrackModel's, following the name
    bool operator==(const SeriesModel &) const = default;
};

struct ModModel {
    std::string id;
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::vector<SeriesModel> series;
    Extra extra;
    bool operator==(const ModModel &) const = default;
};

// The model of a manifest document, as far as it can be read (anything of
// the wrong kind is taken as empty).
[[nodiscard]] ModModel model_from_document(const json::Value &document);

// The manifest for the model, members in the documented order. `samples(file)`
// gives a track file's length in samples when known: a custom loop that ends
// there leaves "end" out.
using SamplesOf = std::optional<std::int64_t> (*)(const std::string &file, const void *context);
[[nodiscard]] json::Value document_from_model(const ModModel &model, SamplesOf samples = nullptr,
                                              const void *context = nullptr);

// mod.json's text for the model: the document, then a new line.
[[nodiscard]] std::string manifest_text(const ModModel &model, SamplesOf samples = nullptr,
                                        const void *context = nullptr);

} // namespace d012::bgm_maker
