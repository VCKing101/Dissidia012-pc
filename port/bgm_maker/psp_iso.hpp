#pragma once

// The PSP / PPSSPP export's disc side: a copy of the player's Dissidia 012
// ISO (ULUS10566 v1.00) with tracks added to the game's own battle music
// list, for PPSSPP or a PSP with custom firmware. The track list is data:
// bgm_entry.bin (in general_archive/main/main.bin) gets a row per track and
// bgm_name.bin (in general_archive/main/EN/main_lang.bin) a name; both
// archives move to the end of PACKAGE.BIN, which grows into the place of the
// file after it (moved to the end of the image), and the music files go into
// DATA/BGM, whose directory moves to the end too. Nothing else in the image
// changes. The formats are docs/port/bgm-playlist-design.md (the table) and
// tools/character_text.py (message tables). No FFmpeg, no game code. MIT
// licence; see port/README.md.

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace d012::psp_iso {

using Bytes = std::vector<std::uint8_t>;

// The clean dump the port's setup accepts (Redump).
inline constexpr std::uint64_t kRetailSize = 1754529792u;
inline constexpr std::string_view kRetailSha1 = "ec23f5beac89312a2a25654e202ae1c997999c9e";
// The table the game loads the rows into has room for 256 (DLC rows share it).
inline constexpr int kTableRows = 256;
inline constexpr std::int16_t kFirstId = 0x4000;
// atracdenc's ATRAC3 encoder delay, measured against Sony's at3tool (whose
// fact says 1,024) through the same decoder; loop points count it.
inline constexpr std::uint32_t kEncoderDelay = 1083u;
// Samples of a loop's start encoded again after its end, so the frames the
// decoder reads past the loop end hold what follows the seam.
inline constexpr std::int64_t kTailSamples = 4096;
// Sony's tool refuses shorter loops; the game's own are all far longer.
inline constexpr std::int64_t kMinLoopSamples = 6144;
inline constexpr int kRate = 44100;

// The name hash of PACKAGE_INFO.BIN: CRC-32 (0x04C11DB7, most significant bit
// first) of the name with A-Z as a-z.
[[nodiscard]] std::uint32_t package_hash(std::string_view name);

// SHA-1, for the ISO check.
class Sha1 {
public:
    Sha1();
    void update(const std::uint8_t *data, std::size_t size);
    [[nodiscard]] std::string hex();  // finishes

private:
    void block(const std::uint8_t *data);
    std::array<std::uint32_t, 5> h_{};
    std::array<std::uint8_t, 64> buffer_{};
    std::size_t used_{};
    std::uint64_t length_{};
};

// "MPK " archives: a header, 16-byte entries (name, data offset, size, 0),
// the names, then the data, each part 16-byte aligned.
struct MpkFile {
    std::string name;
    Bytes data;
    std::uint32_t extra{};  // the entry's fourth word
};
struct Mpk {
    std::uint32_t version{};
    std::vector<MpkFile> files;
};
[[nodiscard]] std::optional<Mpk> read_mpk(const Bytes &data, std::string &error);
[[nodiscard]] Bytes write_mpk(const Mpk &archive);

// Message tables: each string's text as the game sees it (16-bit units,
// unmasked); none where the table leaves a string out.
using MessageTable = std::vector<std::optional<Bytes>>;
[[nodiscard]] std::optional<MessageTable> read_message_table(const Bytes &data, std::string &error);
[[nodiscard]] Bytes write_message_table(const MessageTable &table);
// A track name's text: its characters as units, anything outside printable
// ASCII as '?' (as the port writes names), and the end unit.
[[nodiscard]] Bytes title_text(std::string_view utf8);
[[nodiscard]] std::string text_title(const Bytes &text);

// bgm_entry.bin ("BENT"): rows of 0x2C bytes.
struct BgmRow {
    std::int16_t id{};
    std::int16_t name_text{};
    std::array<std::int16_t, 4> unlock{-1, -1, -1, -1};
    std::uint8_t flags{};
    std::int8_t volume{};
    std::int8_t weight{};
    std::uint8_t pad{};
    std::array<std::uint32_t, 2> stage_mask{};
    std::uint32_t mask18{};
    std::array<char, 16> file{};
};
struct BgmTable {
    std::uint32_t version{};
    std::int32_t pad{};
    std::vector<BgmRow> rows;
};
[[nodiscard]] std::optional<BgmTable> read_bgm_table(const Bytes &data, std::string &error);
[[nodiscard]] Bytes write_bgm_table(const BgmTable &table);

// An ATRAC3 file for the game: the frames of atracdenc's RIFF output
// (`encoded`) under the header Sony's at3tool writes (fmt, fact with the
// encoder delay, smpl with the loop). `loop`: start and end (excluded) in
// samples of the audio given to the encoder, which for a loop was the audio
// up to the end, then kTailSamples from the start again; none: plays once
// (`samples` long).
[[nodiscard]] std::optional<Bytes> at3_file(const Bytes &encoded, std::int64_t samples,
                                            std::optional<std::pair<std::int64_t, std::int64_t>> loop,
                                            std::string &error);

// What the encoder is given for a track: 44.1 kHz stereo 16-bit samples
// (interleaved), with the loop's tail added.
[[nodiscard]] std::vector<std::int16_t> encoder_input(const std::vector<std::int16_t> &samples,
                                                      std::optional<std::pair<std::int64_t, std::int64_t>> loop);
[[nodiscard]] Bytes wave_file(const std::vector<std::int16_t> &stereo);

// Called with work done and the whole of it; false stops (cancel).
using Progress = std::function<bool(std::uint64_t done, std::uint64_t total)>;

// The image is the clean dump: its size, then its SHA-1 (read once, which
// takes a while).
[[nodiscard]] bool check_retail(const std::filesystem::path &iso, const Progress &progress, std::string &error);

// The music table on the image, as the export needs it.
struct DiscMusic {
    int rows{};                     // in bgm_entry.bin
    int names{};                    // strings in bgm_name.bin
    std::int8_t volume{};           // the median of the offered rows' volumes
    std::set<std::int16_t> ids;
    std::vector<std::string> titles;  // of the rows, in order (for the Check)
    [[nodiscard]] int room() const { return kTableRows - rows; }
};
[[nodiscard]] std::optional<DiscMusic> read_disc_music(const std::filesystem::path &iso, std::string &error);

struct Track {
    std::string title;           // as the selector shows it
    std::filesystem::path at3;   // the file at3_file made
};

// Writes `out`: a copy of `original` with `tracks` added after the disc's
// rows, in order. Written to `out` + ".part" first, then renamed; nothing is
// left behind on failure or cancel. Never writes `original`.
[[nodiscard]] bool make_iso(const std::filesystem::path &original, const std::filesystem::path &out,
                            const std::vector<Track> &tracks, const Progress &progress, std::string &error);

// A file of an image by its path ("/PSP_GAME/USRDIR/DATA/BGM/MD00000000.AT3"),
// found through the directory records.
[[nodiscard]] std::optional<Bytes> read_iso_file(const std::filesystem::path &iso, std::string_view path,
                                                 std::string &error);

// The name a row gets for track `index` (0, 1, ...): "MD00000000.AT3".
[[nodiscard]] std::string track_file_name(int index);

} // namespace d012::psp_iso
