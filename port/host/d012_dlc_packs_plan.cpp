// The plan for DLC packs; see d012_dlc_packs_plan.hpp.
//
// What the game reads (docs/research/character-assets.md, section 7):
// - dlc/obj/dlc_%03doe.bin, slots 0 to 255: {u32 0x0C, u32 1} and one 0x44-byte
//   object row. +0 is the row's id (DLC rows are 0x2C0 + slot - 1), +6 the
//   character, +7 the costume (4 to 12 are DLC 1 to DLC 9), +8 flags (8: an
//   assist), +9 the slot (not zero: the row's files are on the memory stick),
//   +0x1C four attached rows' ids, +0x24 the model's name, +0x34 the data name.
// - obj/<model>.gmo, .cosx, .exex and menu/jp/battle/chara_image/<model>.gim
//   and <model>_2.gim for a row; obj/<data name>.objx for an attachment.
// - dlc/bgm/dlc_%03d.bin, slots 0 to 19: "BENT", a date, u32 count, u32 0, then
//   0x2C-byte rows: +0 the track's id, +0x1C its file's name, bgm/<name>.
// - text/jp/dlc/bgm_%03dt.bin: the slot's track titles.
//
// MIT licence; see port/README.md.

#include "d012_dlc_packs_plan.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <map>
#include <set>
#include <utility>

namespace d012::dlc_packs {
namespace {

constexpr std::size_t kRowOffset = 8u;
constexpr std::size_t kRowSize = 0x44u;
constexpr std::size_t kAttached = 0x1Cu;
constexpr std::size_t kAttachedCount = 4u;
constexpr std::size_t kModelName = 0x24u;
constexpr std::size_t kDataName = 0x34u;
constexpr std::size_t kNameSize = 16u;

constexpr int kObjectSlots = 256;
constexpr std::uint16_t kFirstDlcRow = 0x2C0u;
constexpr std::uint16_t kLastDlcRow = 0x3BEu;
constexpr std::uint16_t kNoRow = 0xFFFFu;
constexpr int kFirstDlcCostume = 4;
constexpr int kLastDlcCostume = 12;

constexpr int kBgmSlots = 20;
constexpr int kBgmSlotsLooked = 100;  // a pack may name one the game never reads
constexpr std::size_t kBgmHeader = 16u;
constexpr std::size_t kBgmRow = 0x2Cu;
constexpr std::size_t kBgmFileName = 0x1Cu;
constexpr int kFirstTrackId = 0x100;
constexpr int kTrackIdsEnd = 0x4000;  // from here on they are mods' (bgm-playlist-design.md)

std::uint16_t load16(const std::vector<std::uint8_t> &bytes, std::size_t at) {
    return static_cast<std::uint16_t>(bytes[at] | bytes[at + 1u] << 8u);
}

std::uint32_t load32(const std::vector<std::uint8_t> &bytes, std::size_t at) {
    return bytes[at] | static_cast<std::uint32_t>(bytes[at + 1u]) << 8u |
           static_cast<std::uint32_t>(bytes[at + 2u]) << 16u | static_cast<std::uint32_t>(bytes[at + 3u]) << 24u;
}

void store16(std::vector<std::uint8_t> &bytes, std::size_t at, std::uint16_t value) {
    bytes[at] = static_cast<std::uint8_t>(value);
    bytes[at + 1u] = static_cast<std::uint8_t>(value >> 8u);
}

std::string load_name(const std::vector<std::uint8_t> &bytes, std::size_t at) {
    std::string name;
    for (std::size_t i = 0; i < kNameSize && bytes[at + i] != 0u; ++i) name += static_cast<char>(bytes[at + i]);
    return name;
}

void store_name(std::vector<std::uint8_t> &bytes, std::size_t at, const std::string &name) {
    std::fill_n(bytes.begin() + static_cast<std::ptrdiff_t>(at), kNameSize, std::uint8_t{0});
    std::copy_n(name.begin(), std::min(name.size(), kNameSize - 1u), bytes.begin() + static_cast<std::ptrdiff_t>(at));
}

std::string numbered(const char *format, int number) {
    char text[64];
    std::snprintf(text, sizeof text, format, number);
    return text;
}

std::uint32_t object_table(int slot) { return name_crc(numbered("dlc/obj/dlc_%03doe.bin", slot)); }
std::uint32_t bgm_table(int slot) { return name_crc(numbered("dlc/bgm/dlc_%03d.bin", slot)); }
std::uint32_t bgm_titles(int slot) { return name_crc(numbered("text/jp/dlc/bgm_%03dt.bin", slot)); }

// The files a row's model name leads to.
std::array<std::string, 5> model_files(const std::string &model) {
    return {"obj/" + model + ".gmo", "obj/" + model + ".cosx", "obj/" + model + ".exex",
            "menu/jp/battle/chara_image/" + model + ".gim", "menu/jp/battle/chara_image/" + model + "_2.gim"};
}

std::string costume_label(int costume) {
    return costume >= kFirstDlcCostume ? "DLC " + std::to_string(costume - kFirstDlcCostume + 1)
                                       : "costume " + std::to_string(costume);
}

using Files = std::map<std::uint32_t, const SourceFile *>;

Files index_of(const Pack &pack) {
    Files files;
    for (const SourceFile &file : pack.files) files.emplace(file.crc, &file);
    return files;
}

// A DLC object table's first row, whole: a file of the size the game reads.
bool is_object_table(const std::vector<std::uint8_t> &bytes) {
    return bytes.size() >= kRowOffset + kRowSize && load32(bytes, 0u) == 0x0Cu && load32(bytes, 4u) >= 1u;
}

bool is_bgm_table(const std::vector<std::uint8_t> &bytes) {
    return bytes.size() >= kBgmHeader && std::memcmp(bytes.data(), "BENT", 4u) == 0 &&
           bytes.size() >= kBgmHeader + kBgmRow * static_cast<std::size_t>(load32(bytes, 8u));
}

bool is_dlc_row(std::uint16_t id) { return id >= kFirstDlcRow && id <= kLastDlcRow; }

// What is taken, by what the memory stick holds and the packs before.
struct Taken {
    std::set<std::uint32_t> files;
    std::array<bool, kObjectSlots> object_slots{};
    std::set<std::uint16_t> rows;
    std::set<std::pair<int, int>> costumes;  // character, costume
    std::array<bool, kBgmSlots> bgm_slots{};
    std::set<int> tracks;
};

struct Row {
    int slot{};
    std::uint32_t table{};
    std::vector<std::uint8_t> bytes;  // the whole file

    [[nodiscard]] std::uint16_t id() const { return load16(bytes, kRowOffset); }
    [[nodiscard]] int character() const { return bytes[kRowOffset + 6u]; }
    [[nodiscard]] int costume() const { return bytes[kRowOffset + 7u]; }
    [[nodiscard]] bool assist() const { return (bytes[kRowOffset + 8u] & 8u) != 0u; }
    [[nodiscard]] std::string model() const { return load_name(bytes, kRowOffset + kModelName); }
    [[nodiscard]] std::string data_name() const { return load_name(bytes, kRowOffset + kDataName); }
    [[nodiscard]] bool fighter() const {
        const std::string name = model();
        return name.size() >= 2u && (name[0] == 'P' || name[0] == 'p') && name[1] == '_';
    }
};

// "P_THR100_5P_A" for costume 5 of the assist of the fighter named P_THR100.
std::string model_for_costume(const Row &row, int costume) {
    const std::string name = row.model();
    char tail[16];
    std::snprintf(tail, sizeof tail, "_%XP%s", static_cast<unsigned>(costume), row.assist() ? "_A" : "");
    return name.substr(0u, name.find('_', 2u)) + tail;
}

class Planner {
public:
    Planner(const Pack &installed, const ReadFile &read, Plan &plan) : read_(read), plan_(plan) { note_installed(installed); }

    void add(const Pack &pack) {
        pack_ = &pack;
        files_ = index_of(pack);
        claimed_.clear();
        moved_rows_.clear();
        std::vector<Row> rows;
        if (!read_rows(rows)) return;
        for (Row &row : rows)
            if (!row.fighter()) add_attachment(row);
        add_costumes(rows);
        add_music();
        for (const auto &[crc, file] : files_) {
            if (claimed_.contains(crc)) continue;
            if (taken_.files.contains(crc)) problem("a file with the name of one already in use is left out, " + hex(crc));
            else serve(crc, *file);
        }
    }

private:
    static std::string hex(std::uint32_t crc) {
        char text[16];
        std::snprintf(text, sizeof text, "%08x.edat", crc);
        return text;
    }

    void problem(const std::string &text) { plan_.problems.push_back("\"" + pack_->name + "\": " + text); }
    void moved(const std::string &text) { plan_.moved.push_back("\"" + pack_->name + "\": " + text); }

    void serve(std::uint32_t crc, const SourceFile &file) {
        Served served;
        served.crc = crc;
        served.path = file.path;
        served.size = file.size;
        served.pack = pack_->name;
        plan_.files.push_back(std::move(served));
        taken_.files.insert(crc);
    }

    void serve_bytes(std::uint32_t crc, std::vector<std::uint8_t> bytes) {
        Served served;
        served.crc = crc;
        served.size = bytes.size();
        served.rewritten = true;
        served.bytes = std::move(bytes);
        served.pack = pack_->name;
        plan_.files.push_back(std::move(served));
        taken_.files.insert(crc);
    }

    void note_installed(const Pack &installed) {
        const Files files = index_of(installed);
        for (const auto &[crc, file] : files) taken_.files.insert(crc);
        for (int slot = 0; slot < kObjectSlots; ++slot) {
            const auto found = files.find(object_table(slot));
            if (found == files.end()) continue;
            taken_.object_slots[static_cast<std::size_t>(slot)] = true;
            Row row;
            row.bytes = read_(*found->second);
            if (!is_object_table(row.bytes)) continue;
            taken_.rows.insert(row.id());
            if (row.fighter()) taken_.costumes.emplace(row.character(), row.costume());
        }
        for (int slot = 0; slot < kBgmSlots; ++slot) {
            const auto found = files.find(bgm_table(slot));
            if (found == files.end()) continue;
            taken_.bgm_slots[static_cast<std::size_t>(slot)] = true;
            const std::vector<std::uint8_t> bytes = read_(*found->second);
            if (!is_bgm_table(bytes)) continue;
            for (std::uint32_t i = 0; i < load32(bytes, 8u); ++i) taken_.tracks.insert(load16(bytes, kBgmHeader + kBgmRow * i));
        }
    }

    // False when the pack cannot be used at all.
    bool read_rows(std::vector<Row> &rows) {
        for (int slot = 0; slot < kObjectSlots; ++slot) {
            const auto found = files_.find(object_table(slot));
            if (found == files_.end()) continue;
            Row row;
            row.slot = slot;
            row.table = found->first;
            row.bytes = read_(*found->second);
            if (!is_object_table(row.bytes)) {
                problem("left out: its object table for slot " + std::to_string(slot) +
                        " cannot be read (DLC still encrypted as a PSP keeps it?)");
                return false;
            }
            claimed_.insert(row.table);
            rows.push_back(std::move(row));
        }
        return true;
    }

    // The slot and id a row gets: its own when free.
    std::optional<std::pair<int, std::uint16_t>> place(const Row &row) const {
        const std::uint16_t id = row.id();
        const auto free = [&](int slot) { return !taken_.object_slots[static_cast<std::size_t>(slot)]; };
        if (row.slot != 0 && free(row.slot) && !taken_.rows.contains(id)) return std::make_pair(row.slot, id);
        if (!is_dlc_row(id)) {
            // It takes the place of one of the game's own rows: only one can.
            if (taken_.rows.contains(id)) return std::nullopt;
            for (int slot = 1; slot < kObjectSlots; ++slot)
                if (free(slot)) return std::make_pair(slot, id);
            return std::nullopt;
        }
        for (int slot = 1; slot < kObjectSlots; ++slot) {
            const auto new_id = static_cast<std::uint16_t>(kFirstDlcRow + slot - 1);
            if (free(slot) && !taken_.rows.contains(new_id)) return std::make_pair(slot, new_id);
        }
        return std::nullopt;
    }

    void claim_model(const std::string &model) {
        for (const std::string &name : model_files(model)) claimed_.insert(name_crc(name));
    }

    void serve_model(const std::string &from, const std::string &to) {
        const auto sources = model_files(from);
        const auto targets = model_files(to);
        for (std::size_t i = 0; i < sources.size(); ++i) {
            const auto found = files_.find(name_crc(sources[i]));
            if (found == files_.end()) continue;
            claimed_.insert(found->first);
            const std::uint32_t target = name_crc(targets[i]);
            if (taken_.files.contains(target)) problem(sources[i] + " is left out: another pack has the file");
            else serve(target, *found->second);
        }
    }

    // Takes the slot and writes the row's table.
    void finish_row(Row &row, int slot, std::uint16_t id) {
        if (slot != row.slot) moved("object slot " + std::to_string(row.slot) + " is now " + std::to_string(slot));
        store16(row.bytes, kRowOffset, id);
        row.bytes[kRowOffset + 9u] = static_cast<std::uint8_t>(slot);
        taken_.object_slots[static_cast<std::size_t>(slot)] = true;
        taken_.rows.insert(id);
        serve_bytes(object_table(slot), row.bytes);
    }

    void add_attachment(Row &row) {
        const std::string model = row.model();
        const std::string data = row.data_name();
        const auto where = place(row);
        if (!where) {
            problem("attachment " + model + " is left out: no object slot is free");
            moved_rows_[row.id()] = kNoRow;
            claim_model(model);
            claimed_.insert(name_crc("obj/" + data + ".objx"));
            return;
        }
        const auto [slot, id] = *where;
        moved_rows_[row.id()] = id;
        std::string new_model = model;
        std::string new_data = data;
        if (taken_.files.contains(name_crc("obj/" + model + ".gmo")) ||
            taken_.files.contains(name_crc("obj/" + data + ".objx"))) {
            new_model = numbered("G_ATT_%03d", slot);
            new_data = numbered("g_att_%03d", slot);
            store_name(row.bytes, kRowOffset + kModelName, new_model);
            store_name(row.bytes, kRowOffset + kDataName, new_data);
        }
        serve_model(model, new_model);
        if (const auto found = files_.find(name_crc("obj/" + data + ".objx")); found != files_.end()) {
            claimed_.insert(found->first);
            serve(name_crc("obj/" + new_data + ".objx"), *found->second);
        }
        finish_row(row, slot, id);
    }

    [[nodiscard]] bool costume_free(const Row &row, int costume) const {
        return !taken_.costumes.contains({row.character(), costume}) &&
               !taken_.files.contains(name_crc("obj/" + model_for_costume(row, costume) + ".gmo"));
    }

    void add_costumes(std::vector<Row> &rows) {
        // A fighter and its assist of one costume move together.
        std::map<std::pair<int, int>, std::vector<Row *>> groups;
        for (Row &row : rows)
            if (row.fighter()) groups[{row.character(), row.costume()}].push_back(&row);
        for (auto &[key, group] : groups) {
            const int wanted = key.second;
            int costume = wanted;
            if (wanted >= kFirstDlcCostume && wanted <= kLastDlcCostume) {
                costume = 0;
                if (std::all_of(group.begin(), group.end(), [&](const Row *r) { return costume_free(*r, wanted); }))
                    costume = wanted;
                for (int c = kFirstDlcCostume; costume == 0 && c <= kLastDlcCostume; ++c)
                    if (std::all_of(group.begin(), group.end(), [&](const Row *r) { return costume_free(*r, c); }))
                        costume = c;
                if (costume == 0) {
                    problem(group.front()->model() + " is left out: the character's nine DLC costumes are taken");
                    for (const Row *row : group) claim_model(row->model());
                    continue;
                }
                if (costume != wanted)
                    moved(group.front()->model().substr(0u, group.front()->model().find('_', 2u)) + " " +
                          costume_label(wanted) + " is now " + costume_label(costume));
            }
            for (Row *row : group) add_costume_row(*row, wanted, costume);
            taken_.costumes.emplace(key.first, costume);
        }
    }

    void add_costume_row(Row &row, int wanted, int costume) {
        const std::string model = row.model();
        const auto where = place(row);
        if (!where) {
            problem(model + (is_dlc_row(row.id()) ? " is left out: no object slot is free"
                                                  : " is left out: another pack changes the same costume"));
            claim_model(model);
            return;
        }
        std::string new_model = model;
        if (costume != wanted) {
            new_model = model_for_costume(row, costume);
            row.bytes[kRowOffset + 7u] = static_cast<std::uint8_t>(costume);
            store_name(row.bytes, kRowOffset + kModelName, new_model);
        }
        for (std::size_t i = 0; i < kAttachedCount; ++i) {
            const std::size_t at = kRowOffset + kAttached + i * 2u;
            if (const auto found = moved_rows_.find(load16(row.bytes, at)); found != moved_rows_.end())
                store16(row.bytes, at, found->second);
        }
        serve_model(model, new_model);
        finish_row(row, where->first, where->second);
    }

    [[nodiscard]] int free_track(int slot) const {
        for (int id = std::max(slot << 8, kFirstTrackId); id < kTrackIdsEnd; ++id)
            if (!taken_.tracks.contains(id)) return id;
        return -1;
    }

    [[nodiscard]] std::string free_track_name(int slot, int id) const {
        char name[kNameSize];
        std::snprintf(name, sizeof name, "ud_%02d%04x.at3", slot, static_cast<unsigned>(id));
        for (int extra = 0; extra < 10 && taken_.files.contains(name_crc(std::string("bgm/") + name)); ++extra)
            std::snprintf(name, sizeof name, "ud_%02d%04x_%d.at3", slot, static_cast<unsigned>(id), extra);
        return name;
    }

    void add_music() {
        for (int wanted = 0; wanted < kBgmSlotsLooked; ++wanted) {
            const auto table = files_.find(bgm_table(wanted));
            if (table == files_.end()) continue;
            claimed_.insert(table->first);
            claimed_.insert(bgm_titles(wanted));
            std::vector<std::uint8_t> bytes = read_(*table->second);
            if (!is_bgm_table(bytes)) {
                problem("music slot " + std::to_string(wanted) + " is left out: its table cannot be read");
                continue;
            }
            const std::uint32_t count = load32(bytes, 8u);
            int slot = -1;
            if (wanted < kBgmSlots && !taken_.bgm_slots[static_cast<std::size_t>(wanted)]) slot = wanted;
            for (int s = 1; slot < 0 && s < kBgmSlots; ++s)
                if (!taken_.bgm_slots[static_cast<std::size_t>(s)]) slot = s;
            if (slot < 0) {
                problem("music slot " + std::to_string(wanted) + " is left out: all music slots are taken");
                for (std::uint32_t i = 0; i < count; ++i)
                    claimed_.insert(name_crc("bgm/" + load_name(bytes, kBgmHeader + kBgmRow * i + kBgmFileName)));
                continue;
            }
            if (slot != wanted) moved("music slot " + std::to_string(wanted) + " is now " + std::to_string(slot));
            taken_.bgm_slots[static_cast<std::size_t>(slot)] = true;
            for (std::uint32_t i = 0; i < count; ++i) add_track(bytes, kBgmHeader + kBgmRow * i, slot);
            serve_bytes(bgm_table(slot), std::move(bytes));
            if (const auto titles = files_.find(bgm_titles(wanted)); titles != files_.end()) serve(bgm_titles(slot), *titles->second);
        }
    }

    void add_track(std::vector<std::uint8_t> &bytes, std::size_t row, int slot) {
        int id = load16(bytes, row);
        if (taken_.tracks.contains(id)) {
            id = free_track(slot);
            if (id < 0) {
                problem("a track is left out: no track number is free");
                return;
            }
            store16(bytes, row, static_cast<std::uint16_t>(id));
        }
        taken_.tracks.insert(id);
        const std::string name = load_name(bytes, row + kBgmFileName);
        const std::uint32_t source = name_crc("bgm/" + name);
        const auto file = files_.find(source);
        if (file == files_.end()) return;
        claimed_.insert(source);
        std::string new_name = name;
        if (taken_.files.contains(source)) {
            new_name = free_track_name(slot, id);
            store_name(bytes, row + kBgmFileName, new_name);
        }
        serve(name_crc("bgm/" + new_name), *file->second);
    }

    const ReadFile &read_;
    Plan &plan_;
    Taken taken_;
    const Pack *pack_{};
    Files files_;
    std::set<std::uint32_t> claimed_;
    std::map<std::uint16_t, std::uint16_t> moved_rows_;
};

} // namespace

std::uint32_t name_crc(std::string_view game_path) {
    std::uint32_t crc = 0xFFFFFFFFu;
    for (const char c : game_path) {
        crc ^= static_cast<std::uint32_t>(std::tolower(static_cast<unsigned char>(c))) << 24u;
        for (int bit = 0; bit < 8; ++bit) crc = (crc & 0x80000000u) != 0u ? crc << 1u ^ 0x04C11DB7u : crc << 1u;
    }
    return ~crc;
}

std::optional<std::uint32_t> crc_of_file_name(std::string_view file_name) {
    constexpr std::size_t kDigits = 8u;
    if (file_name.size() != kDigits + 5u) return std::nullopt;
    std::string tail(file_name.substr(kDigits));
    std::transform(tail.begin(), tail.end(), tail.begin(), [](unsigned char c) { return std::tolower(c); });
    if (tail != ".edat") return std::nullopt;
    std::uint32_t crc = 0u;
    for (std::size_t i = 0; i < kDigits; ++i) {
        const auto c = static_cast<unsigned char>(file_name[i]);
        if (std::isxdigit(c) == 0) return std::nullopt;
        crc = crc << 4u | static_cast<std::uint32_t>(std::isdigit(c) != 0 ? c - '0' : std::tolower(c) - 'a' + 10);
    }
    return crc;
}

Plan plan(const std::vector<Pack> &packs, const Pack &installed, const ReadFile &read) {
    Plan result;
    Planner planner(installed, read, result);
    for (const Pack &pack : packs) planner.add(pack);
    std::sort(result.files.begin(), result.files.end(), [](const Served &a, const Served &b) { return a.crc < b.crc; });
    return result;
}

} // namespace d012::dlc_packs
