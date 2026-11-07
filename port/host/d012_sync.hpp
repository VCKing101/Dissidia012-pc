#pragma once

// Online play, phase 0 (docs/port/online-play.md, section 6): do two runs fed
// the same inputs stay identical? MIT licence; see port/README.md.

#include <cstdint>

namespace portablekit {
class HleRegistrar;
}
namespace psprecomp {
class GuestMemory;
}

namespace d012::sync {

// D012_SYNC_TRACE=FILE: after each of the game's pad reads (the versus pad
// read wrapper, once a frame), appends a record to FILE: the read's number,
// whether a battle is loaded and versus is on, both players' pads as the game
// got them (the read's first sample, raw pad 1), and a hash of every 64 KiB
// page of guest RAM. tools/port/sync_compare.py compares two such files.
void after_pad_read(psprecomp::GuestMemory &memory, std::uint32_t buffer, bool battle, bool versus);

// Pad reads traced so far (the next read's number).
[[nodiscard]] std::uint64_t reads();

// Adds the trace's read count to save states (portablekit snapshot), so a
// restored state's reads keep their numbers.
void add_to_snapshots();

// D012_SYNC_EPOCH=SECONDS: the PSP clock's wall time starts at that Unix time
// instead of the host's clock at boot, in UTC rather than the host's time
// zone (sceRtcGetCurrentClockLocalTime, sceRtcGetCurrentTick; the game reads
// no other wall time), so two copies see the same dates and times.
void install(portablekit::HleRegistrar &hle);

// Guest RAM as the trace sees it: `page_count` 64 KiB pages from 0x08000000,
// and one hash of pages [first, first + count).
[[nodiscard]] std::uint32_t page_count(psprecomp::GuestMemory &memory);
[[nodiscard]] std::uint64_t hash_pages(psprecomp::GuestMemory &memory, std::uint32_t first, std::uint32_t count);

} // namespace d012::sync
