// The library maths fingerprint (d012_math_check.hpp).
//
// Each function is run on the floats of a range that the game's VFPU code
// can give it, walked by their bit patterns a step apart (about a million a
// function; in the full check every one of the C runtime's, two billion
// a function), and the results' bits are
// hashed (FNV-1a, 64 bits). The inputs pass through a volatile, so the
// compiler cannot fold the calls away.
//
// Two sets are hashed: the port's own (d012_math.cpp, which the game uses)
// and, on Windows, the C runtime's (ucrtbase.dll's, Wine's under Proton),
// which the game used before. The first must be the same on every machine;
// the second shows whether the C runtimes differ.
//
// MIT licence; see port/README.md.

#include "d012_math_check.hpp"

#include "d012_net.hpp"

#include <bit>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#endif

#if !defined(_MSC_VER)  // under MSVC, <cmath>'s own declarations (d012_math.cpp)
extern "C" {
float sinf(float);
float cosf(float);
float asinf(float);
float log2f(float);
float exp2f(float);
}
#endif

namespace d012::math_check {
namespace {

using Function = float (*)(float);

struct Hash {
    std::uint64_t value = 1469598103934665603ull;
    void add(float f) {
        std::uint32_t bits = std::bit_cast<std::uint32_t>(f);
        // Every NaN counts as one: their payloads are not the game's business.
        if (std::isnan(f)) bits = 0x7FC00000u;
        for (int i = 0; i < 4; ++i) {
            value = (value ^ (bits & 0xFFu)) * 1099511628211ull;
            bits >>= 8u;
        }
    }
};

// Floats in order as integers: negative ones below zero.
std::int64_t ordered(float f) {
    const std::uint32_t bits = std::bit_cast<std::uint32_t>(f);
    return (bits & 0x80000000u) != 0u ? -static_cast<std::int64_t>(bits & 0x7FFFFFFFu) : bits;
}

float from_ordered(std::int64_t o) {
    return o < 0 ? std::bit_cast<float>(static_cast<std::uint32_t>(-o) | 0x80000000u)
                 : std::bit_cast<float>(static_cast<std::uint32_t>(o));
}

// `f` of every float from `low` to `high` times `scale`, `step` floats apart.
std::uint64_t sweep(Function f, float scale, float low, float high, std::int64_t step) {
    Hash hash;
    volatile float input = 0.0f;
    for (std::int64_t o = ordered(low); o <= ordered(high); o += step) {
        input = from_ordered(o);
        hash.add(f(input * scale));
    }
    return hash.value;
}

struct Case {
    const char *name;
    float scale;  // vsin, vcos and vrot pass the argument times pi/2
    float low;
    float high;
    std::int64_t step;
};

constexpr float kHalfPi = 1.57079632679489661923f;
constexpr Case kCases[] = {
    {"sin", kHalfPi, -512.0f, 512.0f, 1777},
    {"cos", kHalfPi, -512.0f, 512.0f, 1777},
    {"asin", 1.0f, -1.0f, 1.0f, 997},
    {"log2", 1.0f, 1.0e-30f, 1.0e30f, 1009},
    {"exp2", 1.0f, -140.0f, 128.0f, 1009},
};

// The five hashes of one set, each on a thread of its own.
std::string fingerprint(const Function (&functions)[5], bool full) {
    std::uint64_t hashes[5]{};
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i)
        threads.emplace_back([&, i] {
            hashes[i] = sweep(functions[i], kCases[i].scale, kCases[i].low, kCases[i].high,
                              full ? 1 : kCases[i].step);
        });
    for (std::thread &t : threads) t.join();
    std::string line;
    for (int i = 0; i < 5; ++i) {
        char part[48];
        std::snprintf(part, sizeof part, "%s%s %016llx", i != 0 ? " " : "", kCases[i].name,
                      static_cast<unsigned long long>(hashes[i]));
        line += part;
    }
    return line;
}

} // namespace

void run_if_asked() {
    // D012_MATH_CHECK, or settings.ini's online.math_check: 0 off, full every
    // input of the C runtime's set, anything else on.
    const std::string asked = net::setting("D012_MATH_CHECK", "online.math_check");
    const bool off = asked == "0";
    const bool on = !asked.empty() && !off;
    // Also whenever this copy has a battle link address set (a test setup).
    const bool test_link = !net::setting("D012_NET_BATTLE", "online.battle_link").empty();
    if (off || (!on && !test_link)) return;
    const bool full = asked == "full";
    const auto began = std::chrono::steady_clock::now();
    const Function ours[5] = {&sinf, &cosf, &asinf, &log2f, &exp2f};
    std::printf("[math] the port's: %s\n", fingerprint(ours, false).c_str());
#if defined(_WIN32)
    HMODULE crt = GetModuleHandleA("ucrtbase.dll");
    if (crt == nullptr) crt = LoadLibraryA("ucrtbase.dll");
    if (crt != nullptr) {
        const char *names[5] = {"sinf", "cosf", "asinf", "log2f", "exp2f"};
        Function theirs[5]{};
        bool found = true;
        for (int i = 0; i < 5; ++i) {
            theirs[i] = reinterpret_cast<Function>(reinterpret_cast<void *>(GetProcAddress(crt, names[i])));
            found = found && theirs[i] != nullptr;
        }
        if (found)
            std::printf("[math] the C runtime's%s: %s\n", full ? " (every input)" : "",
                        fingerprint(theirs, full).c_str());
    }
#endif
    const auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - began).count();
    std::printf("[math] (%lld ms)\n", static_cast<long long>(ms));
    std::fflush(stdout);
}

} // namespace d012::math_check
