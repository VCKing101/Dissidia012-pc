// The mod being edited, to and from mod.json; see model.hpp. MIT licence;
// see port/README.md.

#include "model.hpp"

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <string_view>

namespace d012::bgm_maker {
namespace {

using json::Value;

constexpr int kFormat = 1;

std::string text_of(const Value &object, std::string_view name) {
    const Value *value = object.find(name);
    return value != nullptr && value->is(Value::Kind::String) ? value->text : std::string();
}

// The port's whole_number: a finite number with no fraction, at most 9e15.
std::optional<std::int64_t> whole_number(const Value *value) {
    if (value == nullptr || !value->is(Value::Kind::Number) || !std::isfinite(value->number) ||
        value->number != std::floor(value->number) || std::fabs(value->number) > 9.0e15)
        return std::nullopt;
    return static_cast<std::int64_t>(value->number);
}

Extra extra_of(const Value &object, std::initializer_list<std::string_view> known) {
    Extra extra;
    for (const auto &[name, value] : object.members)
        if (std::find(known.begin(), known.end(), name) == known.end()) extra.emplace_back(name, value);
    return extra;
}

void add_extra(Value &object, const Extra &extra, std::initializer_list<std::string_view> known) {
    for (const auto &[name, value] : extra)
        if (std::find(known.begin(), known.end(), name) == known.end()) object.members.emplace_back(name, value);
}

std::string trimmed(std::string_view text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string_view::npos) return {};
    const auto last = text.find_last_not_of(" \t\r\n");
    return std::string(text.substr(first, last - first + 1u));
}

LoopModel loop_from(const Value *loop) {
    LoopModel out;
    if (loop == nullptr) return out;
    if (loop->is(Value::Kind::String)) {
        if (loop->text == "whole") out.mode = LoopModel::Mode::Whole;
        else if (loop->text == "none") out.mode = LoopModel::Mode::None;
        return out;  // "auto", or anything else, is taken as auto
    }
    if (!loop->is(Value::Kind::Object)) return out;
    out.mode = LoopModel::Mode::Custom;
    const Value *start_s = loop->find("start_seconds");
    const Value *end_s = loop->find("end_seconds");
    if (start_s != nullptr || end_s != nullptr) {
        out.in_seconds = true;
        if (start_s != nullptr && start_s->is(Value::Kind::Number)) out.start_seconds = start_s->number;
        if (end_s != nullptr && end_s->is(Value::Kind::Number)) out.end_seconds = end_s->number;
        return out;
    }
    out.start = whole_number(loop->find("start")).value_or(0);
    out.end = whole_number(loop->find("end"));
    return out;
}

Value loop_value(const LoopModel &loop, std::optional<std::int64_t> samples) {
    switch (loop.mode) {
    case LoopModel::Mode::Auto: return json::string_value("auto");
    case LoopModel::Mode::Whole: return json::string_value("whole");
    case LoopModel::Mode::None: return json::string_value("none");
    case LoopModel::Mode::Custom: break;
    }
    Value out = json::object_value();
    if (loop.in_seconds) {
        out.members.emplace_back("start_seconds", json::number_value(loop.start_seconds));
        if (loop.end_seconds) out.members.emplace_back("end_seconds", json::number_value(*loop.end_seconds));
        return out;
    }
    out.members.emplace_back("start", json::number_value(static_cast<double>(loop.start)));
    // A loop to the file's end leaves "end" out: the port then loops to the
    // end, which is also right for a file it resamples.
    if (loop.end && !(samples && *loop.end == *samples))
        out.members.emplace_back("end", json::number_value(static_cast<double>(*loop.end)));
    return out;
}

} // namespace

ModModel model_from_document(const Value &document) {
    ModModel model;
    if (!document.is(Value::Kind::Object)) return model;
    model.id = text_of(document, "id");
    model.name = text_of(document, "name");
    model.version = text_of(document, "version");
    model.author = text_of(document, "author");
    model.description = text_of(document, "description");
    model.extra = extra_of(document, {"format", "id", "name", "version", "author", "description", "music"});
    const Value *music = document.find("music");
    const Value *series_list = music != nullptr ? music->find("series") : nullptr;
    if (series_list == nullptr || !series_list->is(Value::Kind::Array)) return model;
    for (const Value &entry : series_list->items) {
        if (!entry.is(Value::Kind::Object)) continue;
        SeriesModel series;
        series.key = text_of(entry, "key");
        series.name = text_of(entry, "name");
        series.order = whole_number(entry.find("order")).value_or(0);
        series.extra = extra_of(entry, {"key", "name", "order", "tracks"});
        const Value *tracks = entry.find("tracks");
        if (tracks != nullptr && tracks->is(Value::Kind::Array)) {
            for (const Value &item : tracks->items) {
                if (!item.is(Value::Kind::Object)) continue;
                TrackModel track;
                track.key = text_of(item, "key");
                track.title = text_of(item, "title");
                track.file = text_of(item, "file");
                track.loop = loop_from(item.find("loop"));
                track.extra = extra_of(item, {"key", "title", "file", "loop"});
                series.tracks.push_back(std::move(track));
            }
        }
        model.series.push_back(std::move(series));
    }
    return model;
}

Value document_from_model(const ModModel &model, SamplesOf samples, const void *context) {
    Value doc = json::object_value();
    doc.members.emplace_back("format", json::number_value(kFormat));
    doc.members.emplace_back("id", json::string_value(model.id));
    doc.members.emplace_back("name", json::string_value(model.name));
    doc.members.emplace_back("version", json::string_value(model.version));
    if (const std::string author = trimmed(model.author); !author.empty())
        doc.members.emplace_back("author", json::string_value(author));
    if (const std::string description = trimmed(model.description); !description.empty())
        doc.members.emplace_back("description", json::string_value(description));
    Value series_list = json::array_value();
    for (const SeriesModel &series : model.series) {
        Value entry = json::object_value();
        entry.members.emplace_back("key", json::string_value(series.key));
        entry.members.emplace_back("name", json::string_value(series.name));
        entry.members.emplace_back("order", json::number_value(static_cast<double>(series.order)));
        Value tracks = json::array_value();
        for (const TrackModel &track : series.tracks) {
            const std::optional<std::int64_t> length =
                samples != nullptr && !track.file.empty() ? samples(track.file, context) : std::nullopt;
            Value item = json::object_value();
            item.members.emplace_back("key", json::string_value(track.key));
            item.members.emplace_back("title", json::string_value(track.title));
            item.members.emplace_back("file", json::string_value(track.file));
            item.members.emplace_back("loop", loop_value(track.loop, length));
            add_extra(item, track.extra, {"key", "title", "file", "loop"});
            tracks.items.push_back(std::move(item));
        }
        entry.members.emplace_back("tracks", std::move(tracks));
        add_extra(entry, series.extra, {"key", "name", "order", "tracks"});
        series_list.items.push_back(std::move(entry));
    }
    Value music = json::object_value();
    music.members.emplace_back("series", std::move(series_list));
    doc.members.emplace_back("music", std::move(music));
    add_extra(doc, model.extra, {"format", "id", "name", "version", "author", "description", "music"});
    return doc;
}

std::string manifest_text(const ModModel &model, SamplesOf samples, const void *context) {
    return json::write(document_from_model(model, samples, context)) + "\n";
}

} // namespace d012::bgm_maker
