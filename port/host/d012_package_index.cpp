// The game's package index and the plan for serving mod files in its place;
// see d012_package_index.hpp. MIT licence; see port/README.md.

#include "d012_package_index.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <set>

namespace d012::package_index {
namespace {

std::uint32_t load32(std::span<const std::uint8_t> bytes, std::size_t at) {
    return static_cast<std::uint32_t>(bytes[at]) | static_cast<std::uint32_t>(bytes[at + 1u]) << 8u |
           static_cast<std::uint32_t>(bytes[at + 2u]) << 16u | static_cast<std::uint32_t>(bytes[at + 3u]) << 24u;
}

void store32(std::vector<std::uint8_t> &out, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(value >> (8 * i)));
}

const std::array<std::uint32_t, 256> &crc_table() {
    static const std::array<std::uint32_t, 256> table = [] {
        std::array<std::uint32_t, 256> t{};
        for (std::uint32_t i = 0; i < 256u; ++i) {
            std::uint32_t c = i << 24u;
            for (int k = 0; k < 8; ++k) c = (c & 0x80000000u) != 0u ? (c << 1u) ^ 0x04C11DB7u : c << 1u;
            t[i] = c;
        }
        return t;
    }();
    return table;
}

constexpr std::uint32_t kPackTag = 0x6B636170u;  // "pack"

} // namespace

std::uint32_t path_crc(std::string_view path) {
    std::uint32_t c = 0xFFFFFFFFu;
    for (char ch : path) {
        if (ch == '\\') ch = '/';
        if (ch >= 'A' && ch <= 'Z') ch = static_cast<char>(ch - 'A' + 'a');
        c = (c << 8u) ^ crc_table()[((c >> 24u) ^ static_cast<std::uint8_t>(ch)) & 0xFFu];
    }
    return c ^ 0xFFFFFFFFu;
}

std::optional<Index> parse(std::span<const std::uint8_t> bytes, std::string &error) {
    if (bytes.size() < kHeaderBytes || load32(bytes, 4u) != kPackTag) {
        error = "not a package index (no \"pack\" header)";
        return std::nullopt;
    }
    const std::uint32_t count = load32(bytes, 8u);
    if ((bytes.size() - kHeaderBytes) / kEntryBytes < count) {
        error = "the index names " + std::to_string(count) + " entries but holds " +
                std::to_string((bytes.size() - kHeaderBytes) / kEntryBytes);
        return std::nullopt;
    }
    Index index;
    index.magic = load32(bytes, 0u);
    index.reserved = load32(bytes, 12u);
    index.entries.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::size_t at = kHeaderBytes + i * kEntryBytes;
        const std::uint32_t word = load32(bytes, at + 8u);
        index.entries.push_back({load32(bytes, at), load32(bytes, at + 4u), word & kMaxEntrySize, word >> 28u});
        if (i != 0u && index.entries[i - 1u].crc >= index.entries[i].crc) {
            error = "the index is not sorted by CRC (entry " + std::to_string(i) + ")";
            return std::nullopt;
        }
    }
    return index;
}

std::vector<std::uint8_t> write(const Index &index) {
    std::vector<std::uint8_t> out;
    out.reserve(kHeaderBytes + index.entries.size() * kEntryBytes);
    store32(out, index.magic);
    store32(out, kPackTag);
    store32(out, static_cast<std::uint32_t>(index.entries.size()));
    store32(out, index.reserved);
    for (const Entry &entry : index.entries) {
        store32(out, entry.crc);
        store32(out, entry.sector);
        store32(out, entry.flags << 28u | (entry.size & kMaxEntrySize));
    }
    return out;
}

std::uint64_t end_sector(const Index &index) {
    std::uint64_t end = 0u;
    for (const Entry &entry : index.entries) end = std::max(end, entry.sector + sectors(entry.size));
    return end;
}

Plan plan(const Index &index, std::uint64_t package_size, const std::vector<Replacement> &replacements) {
    Plan result;
    Index patched = index;
    std::uint64_t next = std::max(end_sector(index), sectors(package_size));
    const std::uint64_t first_virtual = next;
    std::set<std::uint32_t> done;
    for (const Replacement &replacement : replacements) {
        if (!replacement.bytes || !done.insert(replacement.crc).second) continue;
        const auto found = std::lower_bound(patched.entries.begin(), patched.entries.end(), replacement.crc,
                                            [](const Entry &entry, std::uint32_t crc) { return entry.crc < crc; });
        if (found == patched.entries.end() || found->crc != replacement.crc) {
            result.missing.push_back(replacement.crc);
            continue;
        }
        const std::uint64_t size = replacement.bytes->size();
        if (size > kMaxEntrySize) {
            result.too_big.push_back(replacement.crc);
            continue;
        }
        const std::uint64_t room = sectors(found->size);
        if (sectors(size) <= room) {
            result.regions.push_back({static_cast<std::uint64_t>(found->sector) * kSector, room * kSector,
                                      replacement.bytes, replacement.name});
            ++result.in_place;
        } else {
            found->sector = static_cast<std::uint32_t>(next);
            result.regions.push_back(
                {next * kSector, sectors(size) * kSector, replacement.bytes, replacement.name});
            next += sectors(size);
            ++result.relocated;
        }
        found->size = static_cast<std::uint32_t>(size);
    }
    std::sort(result.regions.begin(), result.regions.end(),
              [](const Region &a, const Region &b) { return a.offset < b.offset; });
    result.index = write(patched);
    result.relocated_bytes = (next - first_virtual) * kSector;
    result.package_size = next > first_virtual ? next * kSector : package_size;
    return result;
}

std::size_t read(const Plan &plan, std::uint64_t offset, std::uint8_t *destination, std::size_t length,
                 const RealReader &read_real) {
    std::size_t done = 0u;
    while (done < length && offset < plan.package_size) {
        const std::size_t want = static_cast<std::size_t>(std::min<std::uint64_t>(length - done, plan.package_size - offset));
        // The first region that ends after `offset`.
        const auto region = std::upper_bound(plan.regions.begin(), plan.regions.end(), offset,
                                             [](std::uint64_t at, const Region &r) { return at < r.offset + r.length; });
        std::size_t count = 0u;
        if (region != plan.regions.end() && region->offset <= offset) {
            const std::uint64_t within = offset - region->offset;
            count = static_cast<std::size_t>(std::min<std::uint64_t>(want, region->length - within));
            const std::uint64_t have = region->bytes->size();
            const std::size_t copied =
                within < have ? static_cast<std::size_t>(std::min<std::uint64_t>(count, have - within)) : 0u;
            if (copied != 0u) std::memcpy(destination + done, region->bytes->data() + within, copied);
            std::memset(destination + done + copied, 0, count - copied);
        } else {
            const std::uint64_t until = region != plan.regions.end() ? region->offset : plan.package_size;
            count = static_cast<std::size_t>(std::min<std::uint64_t>(want, until - offset));
            const std::size_t got = read_real(offset, destination + done, count);
            if (got < count) std::memset(destination + done + got, 0, count - got);
        }
        done += count;
        offset += count;
    }
    return done;
}

} // namespace d012::package_index
