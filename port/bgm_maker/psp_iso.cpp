// The PSP / PPSSPP export's disc side; see psp_iso.hpp. MIT licence; see
// port/README.md.

#include "psp_iso.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <map>

namespace d012::psp_iso {
namespace {

constexpr std::uint32_t kSector = 2048u;
constexpr std::uint32_t kBgmEntryMagic = 0x544E4542u;  // "BENT" read little-endian
constexpr std::size_t kRowSize = 0x2Cu;
constexpr const char *kPackage = "/PSP_GAME/USRDIR/DATA/PACKAGE.BIN";
constexpr const char *kPackageInfo = "/PSP_GAME/USRDIR/DATA/PACKAGE_INFO.BIN";
constexpr const char *kDataDir = "/PSP_GAME/USRDIR/DATA";
constexpr const char *kBgmDir = "/PSP_GAME/USRDIR/DATA/BGM";
constexpr const char *kMainArchive = "general_archive/main/main.bin";
constexpr const char *kLangArchive = "general_archive/main/EN/main_lang.bin";
constexpr const char *kRowsFile = "bgm_entry.bin";
constexpr const char *kNamesFile = "bgm_name.bin";

// The game's key for hiding message text (main 0x089D68AA), as
// tools/character_text.py has it.
constexpr std::array<std::uint16_t, 16> kTextKey = {0xED98, 0x90AB, 0x5E28, 0x8442, 0x32B1, 0x2DDA, 0xA657, 0xC8BF,
                                                    0x0033, 0x579A, 0xF227, 0x4B4C, 0x36A7, 0x927D, 0x739D, 0x84CE};

std::uint16_t le16(const std::uint8_t *p) { return static_cast<std::uint16_t>(p[0] | p[1] << 8); }
std::uint32_t le32(const std::uint8_t *p) {
    return static_cast<std::uint32_t>(p[0]) | static_cast<std::uint32_t>(p[1]) << 8 |
           static_cast<std::uint32_t>(p[2]) << 16 | static_cast<std::uint32_t>(p[3]) << 24;
}
std::uint32_t be32(const std::uint8_t *p) {
    return static_cast<std::uint32_t>(p[3]) | static_cast<std::uint32_t>(p[2]) << 8 |
           static_cast<std::uint32_t>(p[1]) << 16 | static_cast<std::uint32_t>(p[0]) << 24;
}
void put16(std::uint8_t *p, std::uint32_t v) {
    p[0] = static_cast<std::uint8_t>(v);
    p[1] = static_cast<std::uint8_t>(v >> 8);
}
void put32(std::uint8_t *p, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) p[i] = static_cast<std::uint8_t>(v >> (8 * i));
}
void put_be32(std::uint8_t *p, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) p[3 - i] = static_cast<std::uint8_t>(v >> (8 * i));
}
void append16(Bytes &out, std::uint32_t v) {
    out.push_back(static_cast<std::uint8_t>(v));
    out.push_back(static_cast<std::uint8_t>(v >> 8));
}
void append32(Bytes &out, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
}
void append(Bytes &out, std::string_view text) { out.insert(out.end(), text.begin(), text.end()); }
std::size_t align16(std::size_t n) { return (n + 15u) & ~std::size_t{15}; }
std::uint32_t sectors(std::uint64_t bytes) { return static_cast<std::uint32_t>((bytes + kSector - 1u) / kSector); }

// ---- ISO 9660 ----------------------------------------------------------------

// A directory record, where it is in the image.
struct Record {
    std::uint64_t pos{};
    Bytes raw;
    [[nodiscard]] std::uint32_t lba() const { return le32(raw.data() + 2); }
    [[nodiscard]] std::uint32_t size() const { return le32(raw.data() + 10); }
    [[nodiscard]] bool is_dir() const { return (raw[25] & 2u) != 0u; }
    [[nodiscard]] std::string ident() const {
        return std::string(reinterpret_cast<const char *>(raw.data()) + 33, raw[32]);
    }
    [[nodiscard]] bool is_dot() const { return raw[32] == 1u && (raw[33] == 0u || raw[33] == 1u); }
    [[nodiscard]] std::string name() const {
        std::string id = ident();
        if (const auto semicolon = id.find(';'); semicolon != std::string::npos) id.resize(semicolon);
        return id;
    }
    void set_extent(std::uint32_t lba, std::uint32_t size) {
        put32(raw.data() + 2, lba);
        put_be32(raw.data() + 6, lba);
        put32(raw.data() + 10, size);
        put_be32(raw.data() + 14, size);
    }
};

struct Directory {
    Record self;                  // its record in its parent (the root's: the volume descriptor's)
    std::vector<Record> entries;  // with "." and ".."
};

class Image {
public:
    bool open(const std::filesystem::path &path, bool writable, std::string &error) {
        file_.open(path, std::ios::binary | std::ios::in | (writable ? std::ios::out : std::ios::openmode{}));
        if (!file_) {
            error = "cannot open it";
            return false;
        }
        file_.seekg(0, std::ios::end);
        length_ = static_cast<std::uint64_t>(file_.tellg());
        for (std::uint32_t lba = 16;; ++lba) {
            Bytes vd = read(static_cast<std::uint64_t>(lba) * kSector, kSector);
            if (vd.size() != kSector || std::memcmp(vd.data() + 1, "CD001", 5) != 0) {
                error = "it is not an ISO 9660 image";
                return false;
            }
            if (vd[0] == 255u) break;
            if (vd[0] == 1u && pvd_.empty()) pvd_ = vd;
            else if (vd[0] == 2u) {
                // A second tree (Joliet) would need patching too; UMD images have none.
                error = "it has a supplementary volume descriptor, which a UMD image does not";
                return false;
            }
            if (lba > 64) {
                error = "its volume descriptors do not end";
                return false;
            }
        }
        if (pvd_.empty()) {
            error = "it has no primary volume descriptor";
            return false;
        }
        volume_blocks = le32(pvd_.data() + 80);
        path_table_size = le32(pvd_.data() + 132);
        tables = {std::pair{le32(pvd_.data() + 140), false}, std::pair{le32(pvd_.data() + 144), false},
                  std::pair{be32(pvd_.data() + 148), true}, std::pair{be32(pvd_.data() + 152), true}};
        Record root;
        root.pos = 16u * kSector + 156u;
        root.raw.assign(pvd_.begin() + 156, pvd_.begin() + 190);
        if (!walk(root, "", 0, error)) return false;
        return true;
    }

    [[nodiscard]] Bytes read(std::uint64_t pos, std::size_t size) {
        Bytes out(size);
        file_.clear();
        file_.seekg(static_cast<std::streamoff>(pos));
        file_.read(reinterpret_cast<char *>(out.data()), static_cast<std::streamsize>(size));
        out.resize(static_cast<std::size_t>(std::max<std::streamsize>(file_.gcount(), 0)));
        return out;
    }
    bool write(std::uint64_t pos, const Bytes &data) {
        file_.clear();
        file_.seekp(static_cast<std::streamoff>(pos));
        file_.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(data.size()));
        return static_cast<bool>(file_);
    }
    // Whole sectors from `lba`: `data`, then zeros.
    bool write_sectors(std::uint32_t lba, const Bytes &data) {
        Bytes padded = data;
        padded.resize(static_cast<std::size_t>(sectors(data.size())) * kSector);
        return write(static_cast<std::uint64_t>(lba) * kSector, padded);
    }
    bool write_record(const Record &record) { return write(record.pos, record.raw); }
    [[nodiscard]] Bytes file_data(const Record &record) {
        return read(static_cast<std::uint64_t>(record.lba()) * kSector, record.size());
    }
    bool flush() {
        file_.flush();
        return static_cast<bool>(file_);
    }

    std::uint64_t length_{};
    std::uint32_t volume_blocks{};
    std::uint32_t path_table_size{};
    std::array<std::pair<std::uint32_t, bool>, 4> tables{};  // L, optional L, M, optional M (big-endian)
    std::map<std::string, Directory> dirs;                     // "/PSP_GAME/..."; the root is "/"
    std::map<std::string, Record> files;

private:
    bool walk(const Record &dir, const std::string &path, int depth, std::string &error) {
        if (depth > 16 || dir.size() > (1u << 22)) {
            error = "its directory tree is not as an ISO 9660 image's";
            return false;
        }
        Directory d;
        d.self = dir;
        const Bytes data = read(static_cast<std::uint64_t>(dir.lba()) * kSector, dir.size());
        if (data.size() != dir.size()) {
            error = "a directory lies past its end";
            return false;
        }
        for (std::size_t pos = 0; pos < data.size();) {
            const std::uint8_t n = data[pos];
            if (n == 0u) {
                pos = (pos / kSector + 1u) * kSector;
                continue;
            }
            if (n < 34u || pos + n > data.size()) {
                error = "a directory record is cut short";
                return false;
            }
            Record r;
            r.pos = static_cast<std::uint64_t>(dir.lba()) * kSector + pos;
            r.raw.assign(data.begin() + static_cast<std::ptrdiff_t>(pos), data.begin() + static_cast<std::ptrdiff_t>(pos + n));
            if (33u + r.raw[32] > n) {
                error = "a directory record's name is cut short";
                return false;
            }
            d.entries.push_back(r);
            pos += n;
        }
        const std::string key = path.empty() ? "/" : path;
        dirs[key] = d;
        for (const Record &r : d.entries) {
            if (r.is_dot()) continue;
            const std::string child = path + "/" + r.name();
            if (r.is_dir()) {
                if (!walk(r, child, depth + 1, error)) return false;
            } else {
                files[child] = r;
            }
        }
        return true;
    }

    std::fstream file_;
    Bytes pvd_;
};

// A record like `model` for another file.
Record file_record(const Record &model, const std::string &ident, std::uint32_t lba, std::uint32_t size) {
    const std::uint8_t n = model.raw[32];
    const std::size_t system_use = 33u + n + (n % 2u == 0u ? 1u : 0u);
    Record r;
    r.raw.assign(model.raw.begin(), model.raw.begin() + 33);
    append(r.raw, ident);
    if (ident.size() % 2u == 0u) r.raw.push_back(0u);
    if (system_use < model.raw.size()) r.raw.insert(r.raw.end(), model.raw.begin() + static_cast<std::ptrdiff_t>(system_use), model.raw.end());
    r.raw[0] = static_cast<std::uint8_t>(r.raw.size());
    r.raw[32] = static_cast<std::uint8_t>(ident.size());
    r.set_extent(lba, size);
    return r;
}

// Records laid into sectors, none crossing a sector's end.
Bytes directory_extent(const std::vector<Record> &records) {
    Bytes out;
    for (const Record &r : records) {
        if (out.size() / kSector != (out.size() + r.raw.size() - 1u) / kSector)
            out.resize((out.size() / kSector + 1u) * kSector, 0u);
        out.insert(out.end(), r.raw.begin(), r.raw.end());
    }
    out.resize(static_cast<std::size_t>(sectors(out.size())) * kSector, 0u);
    return out;
}

// ---- The package and the music table ------------------------------------------

struct PackageEntry {
    std::size_t index{};
    std::uint32_t sector{};  // from PACKAGE.BIN's start
    std::uint32_t size{};
};

std::optional<PackageEntry> package_entry(const Bytes &info, std::string_view name) {
    if (info.size() < 16u) return std::nullopt;
    const std::uint32_t count = le32(info.data() + 8);
    if (16u + static_cast<std::uint64_t>(count) * 12u > info.size()) return std::nullopt;
    const std::uint32_t hash = package_hash(name);
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint8_t *p = info.data() + 16 + i * 12u;
        if (le32(p) == hash) return PackageEntry{i, le32(p + 4) & 0xFFFFFu, le32(p + 8) & 0x0FFFFFFFu};
    }
    return std::nullopt;
}

// What make_iso and read_disc_music both read.
struct Music {
    Bytes info;
    PackageEntry main_entry, lang_entry;
    Mpk main, lang;
    std::size_t rows_file{}, names_file{};  // into main.files, lang.files
    BgmTable table;
    MessageTable names;
};

bool read_music(Image &image, Music &m, std::string &error) {
    const auto package = image.files.find(kPackage);
    const auto info = image.files.find(kPackageInfo);
    if (package == image.files.end() || info == image.files.end()) {
        error = "it has no PSP_GAME/USRDIR/DATA/PACKAGE.BIN; is it Dissidia 012 (ULUS10566)?";
        return false;
    }
    m.info = image.file_data(info->second);
    const auto main_entry = package_entry(m.info, kMainArchive);
    const auto lang_entry = package_entry(m.info, kLangArchive);
    if (!main_entry || !lang_entry) {
        error = "its package has no main archives; is it Dissidia 012 (ULUS10566)?";
        return false;
    }
    m.main_entry = *main_entry;
    m.lang_entry = *lang_entry;
    const std::uint32_t base = package->second.lba();
    const auto load = [&](const PackageEntry &entry, const char *what, Mpk &out) {
        const Bytes data = image.read(static_cast<std::uint64_t>(base + entry.sector) * kSector, entry.size);
        std::string why;
        auto archive = data.size() == entry.size ? read_mpk(data, why) : std::nullopt;
        if (!archive) {
            error = std::string(what) + " cannot be read" + (why.empty() ? "" : " (" + why + ")");
            return false;
        }
        out = std::move(*archive);
        return true;
    };
    if (!load(m.main_entry, kMainArchive, m.main) || !load(m.lang_entry, kLangArchive, m.lang)) return false;
    const auto find = [](const Mpk &archive, std::string_view name) {
        for (std::size_t i = 0; i < archive.files.size(); ++i)
            if (archive.files[i].name == name) return i;
        return archive.files.size();
    };
    m.rows_file = find(m.main, kRowsFile);
    m.names_file = find(m.lang, kNamesFile);
    if (m.rows_file == m.main.files.size() || m.names_file == m.lang.files.size()) {
        error = "its archives have no battle music table";
        return false;
    }
    auto table = read_bgm_table(m.main.files[m.rows_file].data, error);
    auto names = table ? read_message_table(m.lang.files[m.names_file].data, error) : std::nullopt;
    if (!table || !names) {
        error = "its battle music table cannot be read (" + error + ")";
        return false;
    }
    m.table = std::move(*table);
    m.names = std::move(*names);
    return true;
}

DiscMusic summary(const Music &m) {
    DiscMusic out;
    out.rows = static_cast<int>(m.table.rows.size());
    out.names = static_cast<int>(m.names.size());
    std::vector<int> volumes;
    for (const BgmRow &row : m.table.rows) {
        out.ids.insert(row.id);
        if (row.weight != 0 && row.volume > 0) volumes.push_back(row.volume);
        const auto at = static_cast<std::size_t>(row.name_text);
        out.titles.push_back(row.name_text >= 0 && at < m.names.size() && m.names[at] ? text_title(*m.names[at])
                                                                                       : std::string());
    }
    std::sort(volumes.begin(), volumes.end());
    // As the port gives its mod rows (host/d012_bgm_playlist.cpp).
    out.volume = volumes.empty() ? std::int8_t{100} : static_cast<std::int8_t>(volumes[volumes.size() / 2u]);
    return out;
}

bool keep_going(const Progress &progress, std::uint64_t done, std::uint64_t total) {
    return !progress || progress(done, total);
}

// Copies `from` to `to`, all of it.
bool copy_image(const std::filesystem::path &from, const std::filesystem::path &to, std::uint64_t &done,
               std::uint64_t total, const Progress &progress, std::string &error) {
    std::ifstream in(from, std::ios::binary);
    std::ofstream out(to, std::ios::binary | std::ios::trunc);
    if (!in || !out) {
        error = !in ? "the original cannot be read" : "the new ISO cannot be written there";
        return false;
    }
    std::vector<char> buffer(4u << 20u);
    while (in) {
        in.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        const std::streamsize got = in.gcount();
        if (got <= 0) break;
        out.write(buffer.data(), got);
        if (!out) {
            error = "writing the new ISO failed (is the disk full?)";
            return false;
        }
        done += static_cast<std::uint64_t>(got);
        if (!keep_going(progress, done, total)) {
            error = "cancelled";
            return false;
        }
    }
    out.close();
    if (!out) {
        error = "writing the new ISO failed (is the disk full?)";
        return false;
    }
    return true;
}

bool read_whole(const std::filesystem::path &path, Bytes &out) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;
    out.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    return true;
}

bool patch(const std::filesystem::path &original, const std::filesystem::path &part, const std::vector<Track> &tracks,
           std::uint64_t &done, std::uint64_t total, const Progress &progress, std::string &error) {
    Image source;
    if (!source.open(original, false, error)) return false;
    Music m;
    if (!read_music(source, m, error)) return false;
    const DiscMusic disc = summary(m);
    if (static_cast<int>(tracks.size()) > disc.room()) {
        error = "the game's list has room for " + std::to_string(disc.room()) + " more tracks, not " +
                std::to_string(tracks.size());
        return false;
    }

    // The rows and names.
    for (std::size_t n = 0; n < tracks.size(); ++n) {
        const auto id = static_cast<std::int16_t>(kFirstId + static_cast<int>(n));
        if (disc.ids.count(id) != 0u) {
            error = "the image's table already has rows from " + std::to_string(kFirstId) +
                    " on; was it made by this tool? Pick the original ISO";
            return false;
        }
        BgmRow row;
        row.id = id;
        row.name_text = static_cast<std::int16_t>(m.names.size());
        row.flags = 0;  // streamed from DATA/BGM, as the port's mod rows
        row.volume = disc.volume;
        row.weight = 1;  // offered by the selector; no stage bits, so never the random default
        const std::string file = track_file_name(static_cast<int>(n));
        std::copy(file.begin(), file.end(), row.file.begin());
        m.table.rows.push_back(row);
        m.names.push_back(title_text(tracks[n].title));
    }
    m.main.files[m.rows_file].data = write_bgm_table(m.table);
    m.lang.files[m.names_file].data = write_message_table(m.names);
    const Bytes new_main = write_mpk(m.main);
    const Bytes new_lang = write_mpk(m.lang);

    // Where they go: after PACKAGE.BIN, which grows into the place of the
    // extent after it; that file moves to the end of the image.
    const Record package = source.files.at(kPackage);
    const std::uint32_t package_sectors = sectors(package.size());
    const std::uint32_t package_end = package.lba() + package_sectors;
    const Record *mover = nullptr;
    std::string mover_path;
    bool directory_next = false;
    std::uint32_t next_lba = 0xFFFFFFFFu;
    for (const auto &[path, record] : source.files)
        if (record.lba() >= package_end && record.lba() < next_lba) {
            next_lba = record.lba();
            mover = &record;
            mover_path = path;
        }
    for (const auto &[path, dir] : source.dirs)
        if (dir.self.lba() >= package_end && dir.self.lba() <= next_lba) {
            next_lba = dir.self.lba();
            directory_next = true;
        }
    const std::uint32_t main_sector = package_sectors;
    const std::uint32_t lang_sector = main_sector + sectors(new_main.size());
    const std::uint32_t new_package_sectors = lang_sector + sectors(new_lang.size());
    if (mover == nullptr || directory_next ||
        package.lba() + new_package_sectors > mover->lba() + sectors(mover->size()) ||
        new_package_sectors >= (1u << 20u)) {
        error = "the image is not laid out as the game's disc (no room after PACKAGE.BIN)";
        return false;
    }
    if (source.length_ < static_cast<std::uint64_t>(source.volume_blocks) * kSector) {
        error = "the image is shorter than its volume";
        return false;
    }
    const auto bgm = source.dirs.find(kBgmDir);
    const auto data_dir = source.dirs.find(kDataDir);
    if (bgm == source.dirs.end() || data_dir == source.dirs.end()) {
        error = "it has no PSP_GAME/USRDIR/DATA/BGM folder";
        return false;
    }
    for (const Record &r : bgm->second.entries)
        if (!r.is_dot() && r.is_dir()) {
            error = "its BGM folder has folders in it, which the game's does not";
            return false;
        }
    const Bytes moved = source.file_data(*mover);
    if (moved.size() != mover->size()) {
        error = mover_path + " cannot be read";
        return false;
    }

    Image out;
    if (!out.open(part, true, error)) {
        error = "the copy cannot be opened (" + error + ")";
        return false;
    }
    std::uint32_t end = source.volume_blocks;
    const auto fail = [&](const std::string &what) {
        error = what + " (is the disk full?)";
        return false;
    };

    // The extent after PACKAGE.BIN to the end first: its place is about to be written.
    const std::uint32_t mover_lba = end;
    if (!out.write_sectors(mover_lba, moved)) return fail("writing " + mover_path + " failed");
    end += sectors(moved.size());
    if (!out.write_sectors(package.lba() + main_sector, new_main) ||
        !out.write_sectors(package.lba() + lang_sector, new_lang))
        return fail("writing the package failed");
    // Their PACKAGE_INFO.BIN records (hash and category bits kept).
    Bytes info = m.info;
    for (const auto &[entry, sector, size] : {std::tuple{m.main_entry, main_sector, new_main.size()},
                                               std::tuple{m.lang_entry, lang_sector, new_lang.size()}}) {
        std::uint8_t *p = info.data() + 16 + entry.index * 12u;
        put32(p + 4, (le32(p + 4) & ~0xFFFFFu) | sector);
        put32(p + 8, (le32(p + 8) & 0xF0000000u) | static_cast<std::uint32_t>(size));
    }
    if (!out.write(static_cast<std::uint64_t>(source.files.at(kPackageInfo).lba()) * kSector, info))
        return fail("writing PACKAGE_INFO.BIN failed");

    // The music.
    std::vector<Record> added;
    const Record *model = nullptr;
    for (const Record &r : bgm->second.entries)
        if (!r.is_dot()) model = &r;
    if (model == nullptr) {
        error = "its BGM folder is empty";
        return false;
    }
    for (std::size_t n = 0; n < tracks.size(); ++n) {
        Bytes data;
        if (!read_whole(tracks[n].at3, data) || data.empty()) {
            error = "the encoded file of \"" + tracks[n].title + "\" cannot be read";
            return false;
        }
        if (!out.write_sectors(end, data)) return fail("writing the music failed");
        added.push_back(file_record(*model, track_file_name(static_cast<int>(n)) + ";1", end,
                                    static_cast<std::uint32_t>(data.size())));
        end += sectors(data.size());
        done += data.size();
        if (!keep_going(progress, done, total)) {
            error = "cancelled";
            return false;
        }
    }

    // Records: PACKAGE.BIN's size, the moved file's place.
    Record package_record = package;
    package_record.set_extent(package.lba(), new_package_sectors * kSector);
    Record mover_record = *mover;
    mover_record.set_extent(mover_lba, mover->size());
    if (!out.write_record(package_record) || !out.write_record(mover_record)) return fail("writing a record failed");

    // The BGM folder, sorted, at the end.
    std::vector<Record> dots, entries;
    for (const Record &r : bgm->second.entries) (r.is_dot() ? dots : entries).push_back(r);
    entries.insert(entries.end(), added.begin(), added.end());
    std::sort(entries.begin(), entries.end(), [](const Record &a, const Record &b) { return a.ident() < b.ident(); });
    std::vector<Record> all = dots;
    all.insert(all.end(), entries.begin(), entries.end());
    const std::uint32_t bgm_lba = end;
    const std::uint32_t bgm_size = static_cast<std::uint32_t>(directory_extent(all).size());
    for (Record &r : all)
        if (r.is_dot() && r.raw[33] == 0u) r.set_extent(bgm_lba, bgm_size);
    const Bytes extent = directory_extent(all);
    if (!out.write_sectors(bgm_lba, extent)) return fail("writing the BGM folder failed");
    end += sectors(extent.size());
    const auto bgm_record = std::find_if(data_dir->second.entries.begin(), data_dir->second.entries.end(),
                                         [](const Record &r) { return !r.is_dot() && r.name() == "BGM"; });
    if (bgm_record == data_dir->second.entries.end()) {
        error = "the DATA folder has no record of BGM";
        return false;
    }
    Record new_bgm_record = *bgm_record;
    new_bgm_record.set_extent(bgm_lba, bgm_size);
    if (!out.write_record(new_bgm_record)) return fail("writing a record failed");

    // The path tables point at the BGM folder by its extent.
    const std::uint32_t old_bgm_lba = bgm->second.self.lba();
    for (const auto &[table_lba, big] : source.tables) {
        if (table_lba == 0u) continue;
        Bytes table = source.read(static_cast<std::uint64_t>(table_lba) * kSector, source.path_table_size);
        int hits = 0;
        for (std::size_t pos = 0; pos + 8u <= table.size();) {
            const std::uint8_t n = table[pos];
            if (n == 0u) break;
            std::uint8_t *at = table.data() + pos + 2;
            if ((big ? be32(at) : le32(at)) == old_bgm_lba) {
                big ? put_be32(at, bgm_lba) : put32(at, bgm_lba);
                ++hits;
            }
            pos += 8u + n + (n & 1u);
        }
        if (hits != 1) {
            error = "a path table does not name the BGM folder once";
            return false;
        }
        if (!out.write(static_cast<std::uint64_t>(table_lba) * kSector, table)) return fail("writing a path table failed");
    }

    // The volume's size, and the image's.
    Bytes size_field(8);
    put32(size_field.data(), end);
    put_be32(size_field.data() + 4, end);
    if (!out.write(16u * kSector + 80u, size_field) || !out.flush()) return fail("writing the volume size failed");
    return true;
}

} // namespace

// ---- Small formats -------------------------------------------------------------

std::uint32_t package_hash(std::string_view name) {
    std::uint32_t crc = 0xFFFFFFFFu;
    for (const char ch : name) {
        auto c = static_cast<std::uint8_t>(ch);
        if (c >= 'A' && c <= 'Z') c = static_cast<std::uint8_t>(c + 0x20);
        crc ^= static_cast<std::uint32_t>(c) << 24;
        for (int bit = 0; bit < 8; ++bit) crc = (crc & 0x80000000u) != 0u ? (crc << 1) ^ 0x04C11DB7u : crc << 1;
    }
    return crc ^ 0xFFFFFFFFu;
}

Sha1::Sha1() : h_{0x67452301u, 0xEFCDAB89u, 0x98BADCFEu, 0x10325476u, 0xC3D2E1F0u} {}

void Sha1::block(const std::uint8_t *p) {
    std::array<std::uint32_t, 80> w{};
    for (int i = 0; i < 16; ++i) w[i] = be32(p + 4 * i);
    const auto rol = [](std::uint32_t v, int s) { return (v << s) | (v >> (32 - s)); };
    for (int i = 16; i < 80; ++i) w[i] = rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    std::uint32_t a = h_[0], b = h_[1], c = h_[2], d = h_[3], e = h_[4];
    for (int i = 0; i < 80; ++i) {
        std::uint32_t f, k;
        if (i < 20) f = (b & c) | (~b & d), k = 0x5A827999u;
        else if (i < 40) f = b ^ c ^ d, k = 0x6ED9EBA1u;
        else if (i < 60) f = (b & c) | (b & d) | (c & d), k = 0x8F1BBCDCu;
        else f = b ^ c ^ d, k = 0xCA62C1D6u;
        const std::uint32_t t = rol(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = rol(b, 30);
        b = a;
        a = t;
    }
    h_[0] += a, h_[1] += b, h_[2] += c, h_[3] += d, h_[4] += e;
}

void Sha1::update(const std::uint8_t *data, std::size_t size) {
    length_ += size;
    if (used_ > 0u) {
        const std::size_t take = std::min(size, 64u - used_);
        std::memcpy(buffer_.data() + used_, data, take);
        used_ += take, data += take, size -= take;
        if (used_ < 64u) return;
        block(buffer_.data());
        used_ = 0;
    }
    for (; size >= 64u; data += 64, size -= 64) block(data);
    std::memcpy(buffer_.data(), data, size);
    used_ = size;
}

std::string Sha1::hex() {
    const std::uint64_t bits = length_ * 8u;
    const std::uint8_t one = 0x80u;
    update(&one, 1);
    const std::uint8_t zero = 0u;
    while (used_ != 56u) update(&zero, 1);
    std::uint8_t tail[8];
    for (int i = 0; i < 8; ++i) tail[i] = static_cast<std::uint8_t>(bits >> (56 - 8 * i));
    update(tail, 8);
    std::string out;
    char digits[9];
    for (const std::uint32_t v : h_) {
        std::snprintf(digits, sizeof digits, "%08x", v);
        out += digits;
    }
    return out;
}

std::optional<Mpk> read_mpk(const Bytes &data, std::string &error) {
    if (data.size() < 16u || std::memcmp(data.data(), "MPK ", 4) != 0) {
        error = "not an MPK archive";
        return std::nullopt;
    }
    Mpk archive;
    archive.version = le32(data.data() + 4);
    const std::uint32_t count = le32(data.data() + 8);
    if (16u + static_cast<std::uint64_t>(count) * 16u > data.size()) {
        error = "its entries are cut short";
        return std::nullopt;
    }
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint8_t *p = data.data() + 16 + i * 16u;
        const std::uint32_t name = le32(p), offset = le32(p + 4), size = le32(p + 8);
        const auto end = std::find(data.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(name, data.size())), data.end(), 0u);
        if (name >= data.size() || end == data.end() || static_cast<std::uint64_t>(offset) + size > data.size()) {
            error = "entry " + std::to_string(i) + " lies outside the archive";
            return std::nullopt;
        }
        MpkFile file;
        file.name.assign(data.begin() + name, end);
        file.data.assign(data.begin() + offset, data.begin() + offset + size);
        file.extra = le32(p + 12);
        archive.files.push_back(std::move(file));
    }
    return archive;
}

Bytes write_mpk(const Mpk &archive) {
    const std::size_t head = 16u + 16u * archive.files.size();
    std::vector<std::size_t> names;
    std::size_t pos = head;
    for (const MpkFile &f : archive.files) {
        names.push_back(pos);
        pos += f.name.size() + 1u;
    }
    pos = align16(pos);
    std::vector<std::size_t> offsets;
    for (const MpkFile &f : archive.files) {
        offsets.push_back(pos);
        pos = align16(pos + f.data.size());
    }
    Bytes out(pos, 0u);
    std::memcpy(out.data(), "MPK ", 4);
    put32(out.data() + 4, archive.version);
    put32(out.data() + 8, static_cast<std::uint32_t>(archive.files.size()));
    for (std::size_t i = 0; i < archive.files.size(); ++i) {
        const MpkFile &f = archive.files[i];
        std::uint8_t *p = out.data() + 16 + i * 16u;
        put32(p, static_cast<std::uint32_t>(names[i]));
        put32(p + 4, static_cast<std::uint32_t>(offsets[i]));
        put32(p + 8, static_cast<std::uint32_t>(f.data.size()));
        put32(p + 12, f.extra);
        std::memcpy(out.data() + names[i], f.name.data(), f.name.size());
        if (!f.data.empty()) std::memcpy(out.data() + offsets[i], f.data.data(), f.data.size());
    }
    return out;
}

namespace {
Bytes masked(const std::uint8_t *text, std::size_t size, std::size_t index) {
    Bytes out(text, text + size);
    for (std::size_t j = 0; j + 1u < size; j += 2u)
        put16(out.data() + j, le16(text + j) ^ kTextKey[(index + j / 2u) & 15u]);
    return out;
}
} // namespace

std::optional<MessageTable> read_message_table(const Bytes &data, std::string &error) {
    if (data.size() < 2u) {
        error = "the message table is cut short";
        return std::nullopt;
    }
    const std::uint16_t count = le16(data.data());
    if (2u + count * 2u > data.size()) {
        error = "the message table's offsets are cut short";
        return std::nullopt;
    }
    MessageTable table;
    for (std::size_t i = 0; i < count; ++i) {
        const std::size_t offset = static_cast<std::size_t>(le16(data.data() + 2 + i * 2u)) * 2u;
        if (offset == 0u) {
            table.emplace_back();
            continue;
        }
        if (offset + 2u > data.size() || offset + 2u + le16(data.data() + offset) > data.size()) {
            error = "string " + std::to_string(i) + " runs past the end of its table";
            return std::nullopt;
        }
        table.emplace_back(masked(data.data() + offset + 2, le16(data.data() + offset), i));
    }
    return table;
}

Bytes write_message_table(const MessageTable &table) {
    Bytes body;
    std::vector<std::uint16_t> offsets;
    const std::size_t start = 2u + table.size() * 2u;
    for (std::size_t i = 0; i < table.size(); ++i) {
        if (!table[i]) {
            offsets.push_back(0u);
            continue;
        }
        offsets.push_back(static_cast<std::uint16_t>((start + body.size()) / 2u));
        append16(body, static_cast<std::uint32_t>(table[i]->size()));
        const Bytes text = masked(table[i]->data(), table[i]->size(), i);
        body.insert(body.end(), text.begin(), text.end());
    }
    Bytes out;
    append16(out, static_cast<std::uint32_t>(table.size()));
    for (const std::uint16_t offset : offsets) append16(out, offset);
    out.insert(out.end(), body.begin(), body.end());
    return out;
}

Bytes title_text(std::string_view utf8) {
    Bytes out;
    for (std::size_t i = 0; i < utf8.size();) {
        const auto c = static_cast<unsigned char>(utf8[i]);
        std::size_t length = 1;
        std::uint32_t unit = '?';
        if (c < 0x80u) unit = c >= 0x20u && c < 0x7Fu ? c : '?';
        else if ((c & 0xE0u) == 0xC0u) length = 2;
        else if ((c & 0xF0u) == 0xE0u) length = 3;
        else if ((c & 0xF8u) == 0xF0u) length = 4;
        append16(out, unit);
        i += length;
    }
    append16(out, 0u);
    return out;
}

std::string text_title(const Bytes &text) {
    std::string out;
    for (std::size_t j = 0; j + 1u < text.size(); j += 2u) {
        const std::uint16_t unit = le16(text.data() + j);
        if (unit == 0u) break;
        out += unit >= 0x20u && unit < 0x7Fu ? static_cast<char>(unit) : '?';
    }
    return out;
}

std::optional<BgmTable> read_bgm_table(const Bytes &data, std::string &error) {
    if (data.size() < 16u || le32(data.data()) != kBgmEntryMagic) {
        error = "not a BENT table";
        return std::nullopt;
    }
    BgmTable table;
    table.version = le32(data.data() + 4);
    const auto count = static_cast<std::int32_t>(le32(data.data() + 8));
    table.pad = static_cast<std::int32_t>(le32(data.data() + 12));
    if (count < 0 || count > kTableRows || 16u + static_cast<std::size_t>(count) * kRowSize > data.size()) {
        error = "its rows are cut short";
        return std::nullopt;
    }
    for (std::int32_t i = 0; i < count; ++i) {
        const std::uint8_t *p = data.data() + 16 + static_cast<std::size_t>(i) * kRowSize;
        BgmRow row;
        row.id = static_cast<std::int16_t>(le16(p));
        row.name_text = static_cast<std::int16_t>(le16(p + 2));
        for (int k = 0; k < 4; ++k) row.unlock[k] = static_cast<std::int16_t>(le16(p + 4 + 2 * k));
        row.flags = p[12];
        row.volume = static_cast<std::int8_t>(p[13]);
        row.weight = static_cast<std::int8_t>(p[14]);
        row.pad = p[15];
        row.stage_mask = {le32(p + 16), le32(p + 20)};
        row.mask18 = le32(p + 24);
        std::memcpy(row.file.data(), p + 28, 16);
        table.rows.push_back(row);
    }
    return table;
}

Bytes write_bgm_table(const BgmTable &table) {
    Bytes out;
    append32(out, kBgmEntryMagic);
    append32(out, table.version);
    append32(out, static_cast<std::uint32_t>(table.rows.size()));
    append32(out, static_cast<std::uint32_t>(table.pad));
    for (const BgmRow &row : table.rows) {
        append16(out, static_cast<std::uint16_t>(row.id));
        append16(out, static_cast<std::uint16_t>(row.name_text));
        for (const std::int16_t flag : row.unlock) append16(out, static_cast<std::uint16_t>(flag));
        out.push_back(row.flags);
        out.push_back(static_cast<std::uint8_t>(row.volume));
        out.push_back(static_cast<std::uint8_t>(row.weight));
        out.push_back(row.pad);
        append32(out, row.stage_mask[0]);
        append32(out, row.stage_mask[1]);
        append32(out, row.mask18);
        out.insert(out.end(), row.file.begin(), row.file.end());
    }
    return out;
}

std::optional<Bytes> at3_file(const Bytes &encoded, std::int64_t samples,
                              std::optional<std::pair<std::int64_t, std::int64_t>> loop, std::string &error) {
    if (encoded.size() < 12u || std::memcmp(encoded.data(), "RIFF", 4) != 0 ||
        std::memcmp(encoded.data() + 8, "WAVE", 4) != 0) {
        error = "the encoder did not write a RIFF file";
        return std::nullopt;
    }
    std::size_t pos = 12;
    const std::uint8_t *frames = nullptr;
    std::uint32_t frames_size = 0;
    bool atrac3 = false;
    while (pos + 8u <= encoded.size()) {
        const std::uint32_t size = le32(encoded.data() + pos + 4);
        if (pos + 8u + size > encoded.size()) break;
        if (std::memcmp(encoded.data() + pos, "fmt ", 4) == 0 && size >= 14u)
            atrac3 = le16(encoded.data() + pos + 8) == 0x270u && le16(encoded.data() + pos + 10) == 2u &&
                     le32(encoded.data() + pos + 12) == static_cast<std::uint32_t>(kRate) &&
                     le16(encoded.data() + pos + 20) == 384u;
        if (std::memcmp(encoded.data() + pos, "data", 4) == 0) {
            frames = encoded.data() + pos + 8;
            frames_size = size;
        }
        pos += 8u + size + (size & 1u);
    }
    if (!atrac3 || frames == nullptr || frames_size == 0u || frames_size % 384u != 0u) {
        error = "the encoder did not write ATRAC3 at 132 kbps, stereo, 44.1 kHz";
        return std::nullopt;
    }
    if (loop && (loop->first < 0 || loop->first + kMinLoopSamples > loop->second || loop->second > samples)) {
        error = "the loop is shorter than " + std::to_string(kMinLoopSamples) + " samples or outside the track";
        return std::nullopt;
    }
    Bytes chunks;
    // fmt as Sony's at3tool writes it for 132 kbps stereo.
    append(chunks, "fmt ");
    append32(chunks, 32u);
    append16(chunks, 0x270u);
    append16(chunks, 2u);
    append32(chunks, static_cast<std::uint32_t>(kRate));
    append32(chunks, 16538u);
    append16(chunks, 384u);
    append16(chunks, 0u);
    append16(chunks, 14u);
    for (const std::uint8_t b : {0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00})
        chunks.push_back(b);
    append(chunks, "fact");
    append32(chunks, 8u);
    append32(chunks, static_cast<std::uint32_t>(loop ? loop->second : samples));
    append32(chunks, kEncoderDelay);
    if (loop) {
        append(chunks, "smpl");
        append32(chunks, 60u);
        for (const std::uint32_t v : {0u, 0u, 22676u, 60u, 0u, 0u, 0u, 1u, 24u}) append32(chunks, v);
        // The game's loop points count the encoder delay; the end is included.
        for (const std::uint32_t v : {0u, 0u, static_cast<std::uint32_t>(loop->first) + kEncoderDelay,
                                      static_cast<std::uint32_t>(loop->second - 1) + kEncoderDelay, 0u, 0u})
            append32(chunks, v);
    }
    append(chunks, "data");
    append32(chunks, frames_size);
    chunks.insert(chunks.end(), frames, frames + frames_size);
    Bytes out;
    append(out, "RIFF");
    append32(out, static_cast<std::uint32_t>(4u + chunks.size()));
    append(out, "WAVE");
    out.insert(out.end(), chunks.begin(), chunks.end());
    return out;
}

std::vector<std::int16_t> encoder_input(const std::vector<std::int16_t> &samples,
                                        std::optional<std::pair<std::int64_t, std::int64_t>> loop) {
    if (!loop) return samples;
    const auto at = [&](std::int64_t frame) { return samples.begin() + static_cast<std::ptrdiff_t>(frame * 2); };
    std::vector<std::int16_t> out(samples.begin(), at(loop->second));
    const std::int64_t tail_end = std::min<std::int64_t>(loop->first + kTailSamples, static_cast<std::int64_t>(samples.size() / 2u));
    out.insert(out.end(), at(loop->first), at(tail_end));
    return out;
}

Bytes wave_file(const std::vector<std::int16_t> &stereo) {
    const auto data = static_cast<std::uint32_t>(stereo.size() * 2u);
    Bytes out;
    append(out, "RIFF");
    append32(out, 36u + data);
    append(out, "WAVEfmt ");
    append32(out, 16u);
    append16(out, 1u);
    append16(out, 2u);
    append32(out, static_cast<std::uint32_t>(kRate));
    append32(out, static_cast<std::uint32_t>(kRate) * 4u);
    append16(out, 4u);
    append16(out, 16u);
    append(out, "data");
    append32(out, data);
    const std::size_t head = out.size();
    out.resize(head + data);
    for (std::size_t i = 0; i < stereo.size(); ++i) put16(out.data() + head + 2u * i, static_cast<std::uint16_t>(stereo[i]));
    return out;
}

// ---- The image -------------------------------------------------------------------

bool check_retail(const std::filesystem::path &iso, const Progress &progress, std::string &error) {
    std::error_code ec;
    const std::uint64_t size = std::filesystem::file_size(iso, ec);
    if (ec) {
        error = "it cannot be read";
        return false;
    }
    if (size != kRetailSize) {
        error = "it is " + std::to_string(size) + " bytes, but a clean dump of Dissidia 012 (USA, ULUS10566 v1.00) is " +
                std::to_string(kRetailSize) + " (Redump); pick the original, not a trimmed or patched copy";
        return false;
    }
    std::ifstream in(iso, std::ios::binary);
    if (!in) {
        error = "it cannot be read";
        return false;
    }
    Sha1 sha;
    std::vector<char> buffer(4u << 20u);
    std::uint64_t done = 0;
    while (in) {
        in.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        const std::streamsize got = in.gcount();
        if (got <= 0) break;
        sha.update(reinterpret_cast<const std::uint8_t *>(buffer.data()), static_cast<std::size_t>(got));
        done += static_cast<std::uint64_t>(got);
        if (!keep_going(progress, done, size)) {
            error = "cancelled";
            return false;
        }
    }
    if (done != size) {
        error = "reading it stopped early";
        return false;
    }
    if (sha.hex() != kRetailSha1) {
        error = "its SHA-1 is not the clean dump's (Redump): it was patched or is another version. "
                "Pick the original Dissidia 012 (USA) ISO";
        return false;
    }
    return true;
}

std::optional<DiscMusic> read_disc_music(const std::filesystem::path &iso, std::string &error) {
    Image image;
    if (!image.open(iso, false, error)) return std::nullopt;
    Music m;
    if (!read_music(image, m, error)) return std::nullopt;
    return summary(m);
}

bool make_iso(const std::filesystem::path &original, const std::filesystem::path &out, const std::vector<Track> &tracks,
              const Progress &progress, std::string &error) {
    std::error_code ec;
    if (std::filesystem::equivalent(original, out, ec)) {
        error = "the new ISO must not replace the original";
        return false;
    }
    std::filesystem::path part = out;
    part += ".part";
    std::uint64_t total = std::filesystem::file_size(original, ec);
    if (ec) {
        error = "the original cannot be read";
        return false;
    }
    for (const Track &track : tracks) total += std::filesystem::file_size(track.at3, ec);
    std::uint64_t done = 0;
    const bool made = copy_image(original, part, done, total, progress, error) &&
                      patch(original, part, tracks, done, total, progress, error);
    if (!made) {
        std::filesystem::remove(part, ec);
        return false;
    }
    std::filesystem::rename(part, out, ec);
    if (ec) {
        std::filesystem::remove(part, ec);
        error = "the new ISO cannot be put in place (" + ec.message() + ")";
        return false;
    }
    return true;
}

std::optional<Bytes> read_iso_file(const std::filesystem::path &iso, std::string_view path, std::string &error) {
    Image image;
    if (!image.open(iso, false, error)) return std::nullopt;
    const auto found = image.files.find(std::string(path));
    if (found == image.files.end()) {
        error = std::string(path) + " is not in it";
        return std::nullopt;
    }
    return image.file_data(found->second);
}

std::string track_file_name(int index) {
    char name[16];
    std::snprintf(name, sizeof name, "MD%08X.AT3", static_cast<unsigned>(index));
    return name;
}

} // namespace d012::psp_iso
