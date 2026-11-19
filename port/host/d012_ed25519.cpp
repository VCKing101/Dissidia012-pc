// Checking an Ed25519 signature (d012_ed25519.hpp).
//
// The arithmetic follows TweetNaCl (Bernstein, van Gastel, Janssen, Lange,
// Schwabe, Smetsers; public domain): numbers modulo 2^255 - 19 in sixteen
// 16-bit limbs. It is slow and small, which suits one check per update. Nothing
// here is secret, so nothing needs to run in constant time. Unlike TweetNaCl
// the check refuses an S of the group's order or more (RFC 8032, section
// 5.1.7), so a signature has only one form.
//
// MIT licence; see port/README.md.

#include "d012_ed25519.hpp"

#include <algorithm>
#include <cstddef>
#include <vector>

namespace d012::ed25519 {
namespace {

// --- SHA-512 ---------------------------------------------------------------

constexpr std::array<std::uint64_t, 80> kRound = {
    0x428a2f98d728ae22ull, 0x7137449123ef65cdull, 0xb5c0fbcfec4d3b2full, 0xe9b5dba58189dbbcull, 0x3956c25bf348b538ull,
    0x59f111f1b605d019ull, 0x923f82a4af194f9bull, 0xab1c5ed5da6d8118ull, 0xd807aa98a3030242ull, 0x12835b0145706fbeull,
    0x243185be4ee4b28cull, 0x550c7dc3d5ffb4e2ull, 0x72be5d74f27b896full, 0x80deb1fe3b1696b1ull, 0x9bdc06a725c71235ull,
    0xc19bf174cf692694ull, 0xe49b69c19ef14ad2ull, 0xefbe4786384f25e3ull, 0x0fc19dc68b8cd5b5ull, 0x240ca1cc77ac9c65ull,
    0x2de92c6f592b0275ull, 0x4a7484aa6ea6e483ull, 0x5cb0a9dcbd41fbd4ull, 0x76f988da831153b5ull, 0x983e5152ee66dfabull,
    0xa831c66d2db43210ull, 0xb00327c898fb213full, 0xbf597fc7beef0ee4ull, 0xc6e00bf33da88fc2ull, 0xd5a79147930aa725ull,
    0x06ca6351e003826full, 0x142929670a0e6e70ull, 0x27b70a8546d22ffcull, 0x2e1b21385c26c926ull, 0x4d2c6dfc5ac42aedull,
    0x53380d139d95b3dfull, 0x650a73548baf63deull, 0x766a0abb3c77b2a8ull, 0x81c2c92e47edaee6ull, 0x92722c851482353bull,
    0xa2bfe8a14cf10364ull, 0xa81a664bbc423001ull, 0xc24b8b70d0f89791ull, 0xc76c51a30654be30ull, 0xd192e819d6ef5218ull,
    0xd69906245565a910ull, 0xf40e35855771202aull, 0x106aa07032bbd1b8ull, 0x19a4c116b8d2d0c8ull, 0x1e376c085141ab53ull,
    0x2748774cdf8eeb99ull, 0x34b0bcb5e19b48a8ull, 0x391c0cb3c5c95a63ull, 0x4ed8aa4ae3418acbull, 0x5b9cca4f7763e373ull,
    0x682e6ff3d6b2b8a3ull, 0x748f82ee5defb2fcull, 0x78a5636f43172f60ull, 0x84c87814a1f0ab72ull, 0x8cc702081a6439ecull,
    0x90befffa23631e28ull, 0xa4506cebde82bde9ull, 0xbef9a3f7b2c67915ull, 0xc67178f2e372532bull, 0xca273eceea26619cull,
    0xd186b8c721c0c207ull, 0xeada7dd6cde0eb1eull, 0xf57d4f7fee6ed178ull, 0x06f067aa72176fbaull, 0x0a637dc5a2c898a6ull,
    0x113f9804bef90daeull, 0x1b710b35131c471bull, 0x28db77f523047d84ull, 0x32caab7b40c72493ull, 0x3c9ebe0a15c9bebcull,
    0x431d67c49c100d4cull, 0x4cc5d4becb3e42b6ull, 0x597f299cfc657e2aull, 0x5fcb6fab3ad6faecull, 0x6c44198c4a475817ull};

std::uint64_t rotate(std::uint64_t x, unsigned bits) { return (x >> bits) | (x << (64u - bits)); }

void sha512_block(std::array<std::uint64_t, 8> &state, const std::uint8_t *block) {
    std::array<std::uint64_t, 80> w{};
    for (std::size_t i = 0; i < 16; ++i)
        for (std::size_t j = 0; j < 8; ++j) w[i] = (w[i] << 8) | block[i * 8 + j];
    for (std::size_t i = 16; i < 80; ++i) {
        const std::uint64_t s0 = rotate(w[i - 15], 1) ^ rotate(w[i - 15], 8) ^ (w[i - 15] >> 7);
        const std::uint64_t s1 = rotate(w[i - 2], 19) ^ rotate(w[i - 2], 61) ^ (w[i - 2] >> 6);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    std::array<std::uint64_t, 8> v = state;
    for (std::size_t i = 0; i < 80; ++i) {
        const std::uint64_t s1 = rotate(v[4], 14) ^ rotate(v[4], 18) ^ rotate(v[4], 41);
        const std::uint64_t choose = (v[4] & v[5]) ^ (~v[4] & v[6]);
        const std::uint64_t t1 = v[7] + s1 + choose + kRound[i] + w[i];
        const std::uint64_t s0 = rotate(v[0], 28) ^ rotate(v[0], 34) ^ rotate(v[0], 39);
        const std::uint64_t majority = (v[0] & v[1]) ^ (v[0] & v[2]) ^ (v[1] & v[2]);
        v = {t1 + s0 + majority, v[0], v[1], v[2], v[3] + t1, v[4], v[5], v[6]};
    }
    for (std::size_t i = 0; i < 8; ++i) state[i] += v[i];
}

std::array<std::uint8_t, 64> sha512(std::span<const std::uint8_t> message) {
    std::array<std::uint64_t, 8> state = {0x6a09e667f3bcc908ull, 0xbb67ae8584caa73bull, 0x3c6ef372fe94f82bull,
                                          0xa54ff53a5f1d36f1ull, 0x510e527fade682d1ull, 0x9b05688c2b3e6c1full,
                                          0x1f83d9abfb41bd6bull, 0x5be0cd19137e2179ull};
    std::size_t at = 0;
    for (; message.size() - at >= 128; at += 128) sha512_block(state, message.data() + at);
    std::array<std::uint8_t, 256> tail{};
    const std::size_t left = message.size() - at;
    std::copy_n(message.data() + at, left, tail.begin());
    tail[left] = 0x80;
    const std::size_t blocks = left < 112 ? 1 : 2;
    // The length in bits, in the last eight of the sixteen bytes kept for it.
    const std::uint64_t bits = static_cast<std::uint64_t>(message.size()) * 8u;
    for (std::size_t i = 0; i < 8; ++i)
        tail[blocks * 128 - 1 - i] = static_cast<std::uint8_t>(bits >> (i * 8));
    for (std::size_t i = 0; i < blocks; ++i) sha512_block(state, tail.data() + i * 128);
    std::array<std::uint8_t, 64> out{};
    for (std::size_t i = 0; i < 8; ++i)
        for (std::size_t j = 0; j < 8; ++j) out[i * 8 + j] = static_cast<std::uint8_t>(state[i] >> (56 - j * 8));
    return out;
}

// --- Numbers modulo 2^255 - 19 ---------------------------------------------

using Field = std::array<std::int64_t, 16>;

constexpr Field kOne = {1};
constexpr Field kD = {0x78a3, 0x1359, 0x4dca, 0x75eb, 0xd8ab, 0x4141, 0x0a4d, 0x0070,
                      0xe898, 0x7779, 0x4079, 0x8cc7, 0xfe73, 0x2b6f, 0x6cee, 0x5203};
constexpr Field kD2 = {0xf159, 0x26b2, 0x9b94, 0xebd6, 0xb156, 0x8283, 0x149a, 0x00e0,
                       0xd130, 0xeef3, 0x80f2, 0x198e, 0xfce7, 0x56df, 0xd9dc, 0x2406};
constexpr Field kBaseX = {0xd51a, 0x8f25, 0x2d60, 0xc956, 0xa7b2, 0x9525, 0xc760, 0x692c,
                          0xdc5c, 0xfdd6, 0xe231, 0xc0a4, 0x53fe, 0xcd6e, 0x36d3, 0x2169};
constexpr Field kBaseY = {0x6658, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666,
                          0x6666, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666};
constexpr Field kRootOfMinusOne = {0xa0b0, 0x4a0e, 0x1b27, 0xc4ee, 0xe478, 0xad2f, 0x1806, 0x2f43,
                                   0xd7a7, 0x3dfb, 0x0099, 0x2b4d, 0xdf0b, 0x4fc1, 0x2480, 0x2b83};

void carry(Field &o) {
    for (std::size_t i = 0; i < 16; ++i) {
        o[i] += 1ll << 16;
        const std::int64_t c = o[i] >> 16;
        o[i < 15 ? i + 1 : 0] += c - 1 + (i == 15 ? 37 * (c - 1) : 0);
        o[i] -= c * (1ll << 16);
    }
}

// Swaps p and q when `swap` is 1.
void select(Field &p, Field &q, std::int64_t swap) {
    const std::int64_t mask = ~(swap - 1);
    for (std::size_t i = 0; i < 16; ++i) {
        const std::int64_t t = mask & (p[i] ^ q[i]);
        p[i] ^= t;
        q[i] ^= t;
    }
}

std::array<std::uint8_t, 32> pack(const Field &n) {
    Field t = n;
    carry(t);
    carry(t);
    carry(t);
    Field m{};
    for (int round = 0; round < 2; ++round) {
        m[0] = t[0] - 0xffed;
        for (std::size_t i = 1; i < 15; ++i) {
            m[i] = t[i] - 0xffff - ((m[i - 1] >> 16) & 1);
            m[i - 1] &= 0xffff;
        }
        m[15] = t[15] - 0x7fff - ((m[14] >> 16) & 1);
        const std::int64_t borrow = (m[15] >> 16) & 1;
        m[14] &= 0xffff;
        select(t, m, 1 - borrow);
    }
    std::array<std::uint8_t, 32> out{};
    for (std::size_t i = 0; i < 16; ++i) {
        out[2 * i] = static_cast<std::uint8_t>(t[i] & 0xff);
        out[2 * i + 1] = static_cast<std::uint8_t>(t[i] >> 8);
    }
    return out;
}

Field unpack(const std::uint8_t *bytes) {
    Field o{};
    for (std::size_t i = 0; i < 16; ++i)
        o[i] = bytes[2 * i] + (static_cast<std::int64_t>(bytes[2 * i + 1]) << 8);
    o[15] &= 0x7fff;
    return o;
}

bool same(const Field &a, const Field &b) { return pack(a) == pack(b); }
std::uint8_t parity(const Field &a) { return pack(a)[0] & 1u; }

Field add(const Field &a, const Field &b) {
    Field o{};
    for (std::size_t i = 0; i < 16; ++i) o[i] = a[i] + b[i];
    return o;
}

Field subtract(const Field &a, const Field &b) {
    Field o{};
    for (std::size_t i = 0; i < 16; ++i) o[i] = a[i] - b[i];
    return o;
}

Field multiply(const Field &a, const Field &b) {
    std::array<std::int64_t, 31> t{};
    for (std::size_t i = 0; i < 16; ++i)
        for (std::size_t j = 0; j < 16; ++j) t[i + j] += a[i] * b[j];
    for (std::size_t i = 0; i < 15; ++i) t[i] += 38 * t[i + 16];
    Field o{};
    std::copy_n(t.begin(), 16, o.begin());
    carry(o);
    carry(o);
    return o;
}

Field square(const Field &a) { return multiply(a, a); }

Field inverse(const Field &a) {
    Field c = a;
    for (int bit = 253; bit >= 0; --bit) {
        c = square(c);
        if (bit != 2 && bit != 4) c = multiply(c, a);
    }
    return c;
}

// a^((p - 5) / 8)
Field power_2523(const Field &a) {
    Field c = a;
    for (int bit = 250; bit >= 0; --bit) {
        c = square(c);
        if (bit != 1) c = multiply(c, a);
    }
    return c;
}

// --- Points on the curve ---------------------------------------------------

using Point = std::array<Field, 4>;  // X, Y, Z, T

void add_point(Point &p, const Point &q) {
    const Field a = multiply(subtract(p[1], p[0]), subtract(q[1], q[0]));
    const Field b = multiply(add(p[0], p[1]), add(q[0], q[1]));
    const Field c = multiply(multiply(p[3], q[3]), kD2);
    const Field half_d = multiply(p[2], q[2]);
    const Field d = add(half_d, half_d);
    const Field e = subtract(b, a);
    const Field f = subtract(d, c);
    const Field g = add(d, c);
    const Field h = add(b, a);
    p[0] = multiply(e, f);
    p[1] = multiply(h, g);
    p[2] = multiply(g, f);
    p[3] = multiply(e, h);
}

void swap_points(Point &p, Point &q, std::int64_t swap) {
    for (std::size_t i = 0; i < 4; ++i) select(p[i], q[i], swap);
}

std::array<std::uint8_t, 32> pack_point(const Point &p) {
    const Field z = inverse(p[2]);
    std::array<std::uint8_t, 32> out = pack(multiply(p[1], z));
    out[31] ^= static_cast<std::uint8_t>(parity(multiply(p[0], z)) << 7);
    return out;
}

Point scalar_multiply(Point q, const std::uint8_t *scalar) {
    Point p = {Field{}, kOne, kOne, Field{}};
    for (int i = 255; i >= 0; --i) {
        const std::int64_t bit = (scalar[i / 8] >> (i & 7)) & 1;
        swap_points(p, q, bit);
        add_point(q, p);
        const Point doubled = p;
        add_point(p, doubled);
        swap_points(p, q, bit);
    }
    return p;
}

Point scalar_multiply_base(const std::uint8_t *scalar) {
    return scalar_multiply({kBaseX, kBaseY, kOne, multiply(kBaseX, kBaseY)}, scalar);
}

// The negative of the point packed in `bytes`; false when it is not on the curve.
bool unpack_negative(Point &r, const std::uint8_t *bytes) {
    r[2] = kOne;
    r[1] = unpack(bytes);
    Field number = square(r[1]);
    Field denominator = multiply(number, kD);
    number = subtract(number, r[2]);
    denominator = add(r[2], denominator);
    const Field den2 = square(denominator);
    const Field den4 = square(den2);
    const Field den6 = multiply(den4, den2);
    Field t = multiply(multiply(den6, number), denominator);
    t = power_2523(t);
    t = multiply(multiply(multiply(t, number), denominator), denominator);
    r[0] = multiply(t, denominator);
    if (!same(multiply(square(r[0]), denominator), number)) r[0] = multiply(r[0], kRootOfMinusOne);
    if (!same(multiply(square(r[0]), denominator), number)) return false;
    if (parity(r[0]) == (bytes[31] >> 7)) r[0] = subtract(Field{}, r[0]);
    r[3] = multiply(r[0], r[1]);
    return true;
}

// --- Numbers modulo the group's order --------------------------------------

constexpr std::array<std::int64_t, 32> kOrder = {0xed, 0xd3, 0xf5, 0x5c, 0x1a, 0x63, 0x12, 0x58, 0xd6, 0x9c, 0xf7,
                                                 0xa2, 0xde, 0xf9, 0xde, 0x14, 0,    0,    0,    0,    0,    0,
                                                 0,    0,    0,    0,    0,    0,    0,    0,    0,    0x10};

std::array<std::uint8_t, 32> reduce(const std::array<std::uint8_t, 64> &wide) {
    std::array<std::int64_t, 64> x{};
    for (std::size_t i = 0; i < 64; ++i) x[i] = wide[i];
    for (std::size_t i = 63; i >= 32; --i) {
        std::int64_t c = 0;
        std::size_t j = i - 32;
        for (; j < i - 12; ++j) {
            x[j] += c - 16 * x[i] * kOrder[j - (i - 32)];
            c = (x[j] + 128) >> 8;
            x[j] -= c * 256;
        }
        x[j] += c;
        x[i] = 0;
    }
    std::int64_t c = 0;
    for (std::size_t j = 0; j < 32; ++j) {
        x[j] += c - (x[31] >> 4) * kOrder[j];
        c = x[j] >> 8;
        x[j] &= 255;
    }
    for (std::size_t j = 0; j < 32; ++j) x[j] -= c * kOrder[j];
    std::array<std::uint8_t, 32> out{};
    for (std::size_t i = 0; i < 32; ++i) {
        x[i + 1] += x[i] >> 8;
        out[i] = static_cast<std::uint8_t>(x[i] & 255);
    }
    return out;
}

// Whether the 32 little-endian bytes are less than the group's order.
bool below_order(const std::uint8_t *s) {
    for (std::size_t i = 32; i-- > 0;) {
        if (s[i] < kOrder[i]) return true;
        if (s[i] > kOrder[i]) return false;
    }
    return false;
}

} // namespace

bool verify(const PublicKey &key, std::span<const std::uint8_t> message, const Signature &signature) {
    if (!below_order(signature.data() + 32)) return false;
    Point negative_key;
    if (!unpack_negative(negative_key, key.data())) return false;

    std::vector<std::uint8_t> hashed;
    hashed.reserve(64 + message.size());
    hashed.insert(hashed.end(), signature.begin(), signature.begin() + 32);
    hashed.insert(hashed.end(), key.begin(), key.end());
    hashed.insert(hashed.end(), message.begin(), message.end());
    const std::array<std::uint8_t, 32> h = reduce(sha512(hashed));

    // R must be S*B - h*A.
    Point p = scalar_multiply(negative_key, h.data());
    add_point(p, scalar_multiply_base(signature.data() + 32));
    const std::array<std::uint8_t, 32> r = pack_point(p);
    return std::equal(r.begin(), r.end(), signature.begin());
}

} // namespace d012::ed25519
