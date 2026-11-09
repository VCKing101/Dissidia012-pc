// Online play, phase 0: the per-frame state trace (d012_sync.hpp).
//
// The file: "D012SYNC", then u32 version (1), u32 page count, u32 page size,
// u32 first page's address; then one record per pad read: u64 read number,
// u32 flags (bit 0 battle loaded, bit 1 versus on), player 1's u32 buttons and
// 4 stick bytes (lx, ly, rx, ry) from the read's first sample, the sample's
// u32 time stamp, player 2's u32 buttons and 2 stick bytes plus 2 spare from
// raw pad 1 (+0x44, +0x48), and a u64 hash per page. Little endian.
//
// MIT licence; see port/README.md.

#include "d012_sync.hpp"

#include "snapshot.hpp"

#include "hle/hle_common.hpp"

#include "psprecomp/guest_memory.hpp"
#include "psprecomp/runtime.hpp"

#include <any>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

namespace d012::sync {
namespace {

constexpr std::uint32_t kFirstPage = 0x08000000u;
constexpr std::uint32_t kPageSize = 0x10000u;
constexpr std::uint32_t kRawPad1 = 0x08C15BF0u;
constexpr std::uint32_t kRtcGetCurrentClockLocalTime = 0xE7C27D1Bu;
constexpr std::uint32_t kRtcGetCurrentTick = 0x3F7AD767u;

struct Trace {
    std::FILE *file = nullptr;
    std::uint64_t reads = 0;
    std::uint32_t pages = 0;
    std::vector<std::uint64_t> hashes;
};

Trace &trace() {
    static Trace t = [] {
        Trace made;
        const char *path = std::getenv("D012_SYNC_TRACE");
        if (path == nullptr || *path == '\0') return made;
        made.file = std::fopen(path, "wb");
        if (made.file == nullptr) std::cerr << "[sync] cannot write " << path << std::endl;
        return made;
    }();
    return t;
}

std::uint64_t hash_page(const std::uint8_t *data) {
    std::uint64_t lanes[4] = {0x9E3779B97F4A7C15ull, 0xC2B2AE3D27D4EB4Full, 0x165667B19E3779F9ull,
                              0x85EBCA77C2B2AE63ull};
    for (std::uint32_t at = 0; at < kPageSize; at += 32u) {
        for (int k = 0; k < 4; ++k) {
            std::uint64_t word;
            std::memcpy(&word, data + at + k * 8u, 8u);
            lanes[k] = (lanes[k] ^ word) * 0x100000001B3ull;
            lanes[k] ^= lanes[k] >> 29;
        }
    }
    std::uint64_t h = lanes[0] ^ (lanes[1] * 3u) ^ (lanes[2] * 5u) ^ (lanes[3] * 7u);
    h ^= h >> 31;
    h *= 0x7FB5D329728EA185ull;
    return h ^ (h >> 27);
}

template <typename T> void put(std::FILE *file, T value) { std::fwrite(&value, sizeof value, 1u, file); }

} // namespace

void after_pad_read(psprecomp::GuestMemory &memory, std::uint32_t buffer, bool battle, bool versus) {
    Trace &t = trace();
    if (t.file == nullptr) return;
    if (t.reads == 0u) {
        t.pages = (memory.size() - (kFirstPage - psprecomp::GuestMemory::kPhysicalBase)) / kPageSize;
        t.hashes.resize(t.pages);
        std::fwrite("D012SYNC", 1u, 8u, t.file);
        put<std::uint32_t>(t.file, 1u);
        put<std::uint32_t>(t.file, t.pages);
        put<std::uint32_t>(t.file, kPageSize);
        put<std::uint32_t>(t.file, kFirstPage);
        std::cout << "[sync] tracing " << t.pages << " pages of guest RAM at every pad read" << std::endl;
    }
    put<std::uint64_t>(t.file, t.reads++);
    put<std::uint32_t>(t.file, (battle ? 1u : 0u) | (versus ? 2u : 0u));
    const bool sample = memory.contains(buffer & 0x1FFFFFFFu, 16u);
    put<std::uint32_t>(t.file, sample ? memory.load32(buffer + 4u) : 0u);
    for (std::uint32_t i = 8u; i < 12u; ++i) put<std::uint8_t>(t.file, sample ? memory.load8(buffer + i) : 0u);
    put<std::uint32_t>(t.file, sample ? memory.load32(buffer) : 0u);
    put<std::uint32_t>(t.file, memory.load32(kRawPad1 + 0x44u));
    put<std::uint32_t>(t.file, memory.load32(kRawPad1 + 0x48u));
    for (std::uint32_t p = 0; p < t.pages; ++p) {
        const std::uint8_t *data = memory.raw_pointer(kFirstPage + p * kPageSize, kPageSize);
        t.hashes[p] = data != nullptr ? hash_page(data) : 0u;
    }
    // D012_SYNC_DUMP=N[,N...]: guest RAM after those reads, to FILE.N.ram, for
    // a byte-level look at pages the hashes say differ.
    static const std::vector<std::uint64_t> dumps = [] {
        std::vector<std::uint64_t> at;
        const char *text = std::getenv("D012_SYNC_DUMP");
        for (const char *p = text; p != nullptr && *p != '\0';) {
            char *end = nullptr;
            at.push_back(std::strtoull(p, &end, 10));
            p = (end != nullptr && *end == ',') ? end + 1 : nullptr;
        }
        return at;
    }();
    const std::uint64_t read = t.reads - 1u;
    for (const std::uint64_t at : dumps) {
        if (at != read) continue;
        const std::string path = std::string(std::getenv("D012_SYNC_TRACE")) + "." + std::to_string(read) + ".ram";
        if (std::FILE *out = std::fopen(path.c_str(), "wb"); out != nullptr) {
            const std::uint32_t size = t.pages * kPageSize;
            std::fwrite(memory.raw_pointer(kFirstPage, size), 1u, size, out);
            std::fclose(out);
        }
    }
    std::fwrite(t.hashes.data(), sizeof(std::uint64_t), t.pages, t.file);
    if (t.reads % 64u == 0u) std::fflush(t.file);
}

std::uint64_t reads() { return trace().reads; }

void add_to_snapshots() {
    portablekit::snapshot::add(
        "sync trace", [] { return std::any(trace().reads); },
        [](const std::any &any) { trace().reads = std::any_cast<std::uint64_t>(any); });
}

std::uint32_t page_count(psprecomp::GuestMemory &memory) {
    return (memory.size() - (kFirstPage - psprecomp::GuestMemory::kPhysicalBase)) / kPageSize;
}

std::uint64_t hash_pages(psprecomp::GuestMemory &memory, std::uint32_t first, std::uint32_t count) {
    std::uint64_t h = 0;
    for (std::uint32_t p = first; p < first + count; ++p) {
        const std::uint8_t *data = memory.raw_pointer(kFirstPage + p * kPageSize, kPageSize);
        h = (h ^ (data != nullptr ? hash_page(data) : 0u)) * 0x100000001B3ull;
    }
    return h;
}

void install(portablekit::HleRegistrar &hle) {
    const char *text = std::getenv("D012_SYNC_EPOCH");
    if (text == nullptr || *text == '\0') return;
    const std::uint64_t epoch_us = std::strtoull(text, nullptr, 10) * 1'000'000ull;
    const auto now_us = [epoch_us] { return epoch_us + portablekit::kernel().now_us(); };
    if (hle.bound_function("sceRtc", kRtcGetCurrentClockLocalTime)) {
        hle.bind("sceRtc", kRtcGetCurrentClockLocalTime,
                 [now_us](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
                     const std::uint64_t now = now_us();
                     const auto seconds = static_cast<std::time_t>(now / 1'000'000u);
                     std::tm tm{};
#if defined(_WIN32)
                     gmtime_s(&tm, &seconds);
#else
                     gmtime_r(&seconds, &tm);
#endif
                     psprecomp::GuestMemory &memory = rt.memory();
                     const std::uint32_t out = portablekit::arg(ctx, 0);
                     memory.store16(out, static_cast<std::uint16_t>(tm.tm_year + 1900));
                     memory.store16(out + 2u, static_cast<std::uint16_t>(tm.tm_mon + 1));
                     memory.store16(out + 4u, static_cast<std::uint16_t>(tm.tm_mday));
                     memory.store16(out + 6u, static_cast<std::uint16_t>(tm.tm_hour));
                     memory.store16(out + 8u, static_cast<std::uint16_t>(tm.tm_min));
                     memory.store16(out + 10u, static_cast<std::uint16_t>(tm.tm_sec));
                     memory.store32(out + 12u, static_cast<std::uint32_t>(now % 1'000'000u));
                     portablekit::kernel().finish(ctx, 0u);
                 });
    }
    if (hle.bound_function("sceRtc", kRtcGetCurrentTick)) {
        hle.bind("sceRtc", kRtcGetCurrentTick, [now_us](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
            // Microseconds since 0001-01-01, 719162 days before the Unix epoch.
            constexpr std::uint64_t kUnixEpochTick = 719'162ull * 86'400ull * 1'000'000ull;
            const std::uint32_t out = portablekit::arg(ctx, 0);
            if (out != 0u) {
                const std::uint64_t tick = kUnixEpochTick + now_us();
                rt.memory().store32(out, static_cast<std::uint32_t>(tick));
                rt.memory().store32(out + 4u, static_cast<std::uint32_t>(tick >> 32u));
            }
            portablekit::kernel().finish(ctx, 0u);
        });
    }
    std::cout << "[sync] the PSP clock starts at Unix time " << text << " (UTC)" << std::endl;
}

} // namespace d012::sync
