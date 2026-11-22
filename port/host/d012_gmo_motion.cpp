// The skeletons and animations of the game's GMO models, and a motion
// rebuilt for another skeleton; see d012_gmo_motion.hpp.
//
// Every platform must give the same bytes, so the doubles here see only
// + - * / and square root, each expression in the order the Python tools
// (tools/gmo_motion.py, tools/move_retarget.py) work it out, and a product
// is never fused with a sum: build this file with -ffp-contract=off (GCC,
// Clang) and never with -ffast-math. Python's errors (a division by zero,
// the square root of a negative number, a value too large to pack) are
// Errors here. MIT licence; see port/README.md.

#include "d012_gmo_motion.hpp"

#include <algorithm>
#include <bit>
#include <cfloat>
#include <cmath>
#include <limits>

#if defined(__FAST_MATH__) || defined(_M_FP_FAST) || (defined(FLT_EVAL_METHOD) && FLT_EVAL_METHOD != 0)
#error "d012_gmo_motion.cpp needs exact IEEE double arithmetic: no fast maths, no wider intermediates"
#endif
#if defined(__clang__)
#pragma STDC FP_CONTRACT OFF
#elif defined(_MSC_VER)
#pragma fp_contract(off)
#endif

namespace d012::gmo_motion {
namespace {

static_assert(std::numeric_limits<double>::is_iec559 && std::numeric_limits<float>::is_iec559);

using Bytes = std::span<const std::uint8_t>;

constexpr std::array<std::uint8_t, 16> kMagic = {'O', 'M', 'G', '.', '0', '0', '.', '1', 'P', 'S', 'P', 0, 0, 0, 0, 0};
constexpr int kMaxDepth = 64;  // chunks inside chunks; the game's files go five deep

// Chunk types (Sony's libgmo numbers). A type with 0x8000 set is a "half"
// chunk: eight bytes of header, then its payload.
constexpr std::uint16_t kHalf = 0x8000u;
constexpr std::uint16_t kFile = 0x0002u, kModel = 0x0003u, kBone = 0x0004u, kPart = 0x0005u, kMotion = 0x000Bu,
                        kFCurve = 0x000Cu, kParentBone = 0x0041u, kBlendBones = 0x0044u, kBlendOffsets = 0x0045u,
                        kTranslate = 0x0048u, kRotateQ = 0x004Bu, kFrameLoop = 0x00B1u, kFrameRate = 0x00B2u,
                        kAnimate = 0x00B3u, kFrameRepeat = 0x00B4u;
// Animate commands.
constexpr std::uint32_t kCommandTranslate = 0x48u, kCommandRotate = 0x4Bu, kCommandScale = 0x4Du;
// Curve interpolations, and the values each takes per dimension.
constexpr std::uint32_t kConstant = 0u, kLinear = 1u, kSpherical = 4u;
constexpr std::array<std::size_t, 5> kPerDim = {1u, 1u, 3u, 3u, 1u};
constexpr std::uint32_t kHalfFloats = 0x80u;  // in a curve's type: its keys are half floats
constexpr std::uint32_t kNoRef = 0xFFFFFFFFu;

constexpr Quat kIdentity = {0.0, 0.0, 0.0, 1.0};
constexpr double kPi = 3.141592653589793;

// ------------------------------------------------------------------ bytes

std::uint16_t load16(Bytes bytes, std::uint64_t at) {
    if (bytes.size() < 2u || at > bytes.size() - 2u) throw Error("a GMO chunk is too short");
    const std::size_t i = static_cast<std::size_t>(at);
    return static_cast<std::uint16_t>(bytes[i] | bytes[i + 1u] << 8u);
}

std::uint32_t load32(Bytes bytes, std::uint64_t at) {
    if (bytes.size() < 4u || at > bytes.size() - 4u) throw Error("a GMO chunk is too short");
    const std::size_t i = static_cast<std::size_t>(at);
    return static_cast<std::uint32_t>(bytes[i]) | static_cast<std::uint32_t>(bytes[i + 1u]) << 8u |
           static_cast<std::uint32_t>(bytes[i + 2u]) << 16u | static_cast<std::uint32_t>(bytes[i + 3u]) << 24u;
}

void store16(std::vector<std::uint8_t> &out, std::uint16_t value) {
    for (int i = 0; i < 2; ++i) out.push_back(static_cast<std::uint8_t>(value >> (8 * i)));
}

void store32(std::vector<std::uint8_t> &out, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(value >> (8 * i)));
}

// Zeros up to a multiple of four bytes, counted from `start`.
void pad4(std::vector<std::uint8_t> &out, std::size_t start) {
    while ((out.size() - start) % 4u != 0u) out.push_back(0u);
}

// A 32-bit float's value (struct's "<f").
double float_value(std::uint32_t bits) { return static_cast<double>(std::bit_cast<float>(bits)); }

// The nearest 32-bit float, ties to even (struct's "<f"); too large a value
// is an error, as in Python.
std::uint32_t float_bits(double value) {
    // From here up a finite value rounds to infinity.
    if ((value >= 0x1.ffffffp127 || value <= -0x1.ffffffp127) && value - value == 0.0) {
        throw Error("a value is too large for a float");
    }
    return std::bit_cast<std::uint32_t>(static_cast<float>(value));
}

// A half float's value, exactly (struct's "<e").
double half_value(std::uint16_t bits) {
    const std::uint64_t sign = static_cast<std::uint64_t>(bits >> 15u) << 63u;
    const std::uint64_t exponent = (bits >> 10u) & 0x1Fu;
    const std::uint64_t fraction = bits & 0x3FFu;
    if (exponent == 0x1Fu) {  // infinity, or not a number
        return std::bit_cast<double>(sign | (fraction == 0u ? 0x7FF0000000000000u : 0x7FF8000000000000u));
    }
    if (exponent == 0u) {  // zero and the subnormals: fraction * 2^-24
        const double value = static_cast<double>(fraction) * (1.0 / 16777216.0);
        return sign != 0u ? -value : value;
    }
    return std::bit_cast<double>(sign | (exponent + 1008u) << 52u | fraction << 42u);
}

// The nearest half float, ties to even, down to the subnormals and zero
// (struct's "<e"); too large a value is an error, as in Python.
std::uint16_t half_bits(double value) {
    const std::uint64_t bits = std::bit_cast<std::uint64_t>(value);
    const std::uint32_t sign = static_cast<std::uint32_t>(bits >> 63u) << 15u;
    const int biased = static_cast<int>((bits >> 52u) & 0x7FFu);
    const std::uint64_t fraction = bits & 0x000FFFFFFFFFFFFFu;
    if (biased == 0x7FF) return static_cast<std::uint16_t>(sign | (fraction == 0u ? 0x7C00u : 0x7E00u));
    const int exponent = biased - 1023;
    if (biased == 0 || exponent < -25) return static_cast<std::uint16_t>(sign);  // under half the smallest half
    if (exponent >= 16) throw Error("a value is too large for a half float");
    const std::uint64_t mantissa = fraction | 0x0010000000000000u;
    // A normal half keeps ten bits under the leading one; a subnormal one
    // counts in steps of 2^-24.
    const bool normal = exponent >= -14;
    const int shift = normal ? 42 : 28 - exponent;
    std::uint64_t kept = mantissa >> shift;
    const std::uint64_t rest = mantissa & ((std::uint64_t{1} << shift) - 1u);
    const std::uint64_t tie = std::uint64_t{1} << (shift - 1);
    if (rest > tie || (rest == tie && (kept & 1u) != 0u)) ++kept;
    // Rounding up past the last fraction carries into the exponent.
    const std::uint64_t packed = normal ? (static_cast<std::uint64_t>(exponent + 15) << 10u) + (kept - 0x400u) : kept;
    if (packed >= 0x7C00u) throw Error("a value is too large for a half float");
    return static_cast<std::uint16_t>(sign | packed);
}

// ------------------------------------------------------------------ chunks

// data[from:to] as Python slices it: cut to the data, empty when the ends
// cross.
Bytes slice(Bytes data, std::uint64_t from, std::uint64_t to) {
    const std::uint64_t size = data.size();
    from = std::min(from, size);
    to = std::min(to, size);
    return to > from ? data.subspan(static_cast<std::size_t>(from), static_cast<std::size_t>(to - from)) : Bytes{};
}

// One chunk:
//   u16 type, u16 header_size, u32 chunk_size          a half chunk: the payload follows
//   ... + u32 child_offset, u32 data_offset, name      others: args [header_size, data_offset), data
//                                                      [data_offset, child_offset), then its chunks
struct Chunk {
    std::uint16_t type{};
    Bytes name;     // up to its first zero byte
    Bytes args;
    Bytes data;
    Bytes payload;  // a half chunk's
    std::uint64_t below{};  // its chunks are [below, end) of the file
    std::uint64_t end{};

    [[nodiscard]] bool half() const { return (type & kHalf) != 0u; }
    [[nodiscard]] std::uint16_t kind() const { return static_cast<std::uint16_t>(type & 0x7FFFu); }
};

// The chunks that fill [pos, end) of a model, one after another.
std::vector<Chunk> chunks(Bytes data, std::uint64_t pos, std::uint64_t end) {
    std::vector<Chunk> out;
    while (pos + 8u <= end) {
        Chunk chunk;
        chunk.type = load16(data, pos);
        const std::uint64_t header = load16(data, pos + 2u);
        const std::uint64_t size = load32(data, pos + 4u);
        if (size < 8u || pos + size > end) throw Error("bad GMO chunk at " + std::to_string(pos));
        chunk.end = pos + size;
        if (chunk.half()) {
            chunk.payload = slice(data, pos + 8u, pos + size);
            chunk.below = chunk.end;
        } else {
            const std::uint64_t child = load32(data, pos + 8u);
            const std::uint64_t at = load32(data, pos + 12u);
            const Bytes raw = slice(data, pos + 16u, pos + header);
            chunk.name = raw.first(static_cast<std::size_t>(std::find(raw.begin(), raw.end(), 0u) - raw.begin()));
            chunk.args = slice(data, pos + header, pos + at);
            chunk.data = slice(data, pos + at, pos + child);
            chunk.below = pos + child;
        }
        out.push_back(chunk);
        pos += size;
    }
    if (pos != end) throw Error("trailing GMO bytes at " + std::to_string(pos));
    return out;
}

// Reads every chunk under `chunk`, so a model with a bad chunk anywhere is
// refused.
void check(Bytes data, const Chunk &chunk, int depth) {
    if (depth > kMaxDepth) throw Error("GMO chunks too deep");
    for (const Chunk &child : chunks(data, chunk.below, chunk.end)) check(data, child, depth + 1);
}

bool has_child(Bytes data, const Chunk &chunk, std::uint16_t kind) {
    const std::vector<Chunk> children = chunks(data, chunk.below, chunk.end);
    return std::any_of(children.begin(), children.end(), [kind](const Chunk &c) { return c.kind() == kind; });
}

std::string text(Bytes bytes) { return {bytes.begin(), bytes.end()}; }

template <std::size_t N> std::array<double, N> floats(Bytes payload, std::uint64_t at = 0u) {
    std::array<double, N> out{};
    for (std::size_t i = 0; i < N; ++i) out[i] = float_value(load32(payload, at + 4u * i));
    return out;
}

// ------------------------------------------------------------------ maths
// Only add, subtract, multiply, divide and square root, in Python's order.

// a / b; Python stops at a division by zero.
double over(double a, double b) {
    if (b == 0.0) throw Error("division by zero");
    return a / b;
}

// Python stops at the square root of a negative number.
double root(double x) {
    if (x < 0.0) throw Error("square root of a negative number");
    return std::sqrt(x);
}

double dot3(const Vec3 &a, const Vec3 &b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

double dot4(const Quat &a, const Quat &b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]; }

double distance(const Vec3 &a, const Vec3 &b) {
    const double x = a[0] - b[0], y = a[1] - b[1], z = a[2] - b[2];
    return std::sqrt(x * x + y * y + z * z);
}

// atan of 0 <= z <= 1: halve the angle twice, then a fixed series.
double atan01(double z) {
    z = z / (1.0 + std::sqrt(1.0 + z * z));
    z = z / (1.0 + std::sqrt(1.0 + z * z));
    const double q = z * z;
    double series = 1.0 / 17.0;
    for (const double odd : {15.0, 13.0, 11.0, 9.0, 7.0, 5.0, 3.0, 1.0}) series = 1.0 / odd - q * series;
    return 4.0 * z * series;
}

// acos of -1 <= x <= 1.
double acos01(double x) {
    if (x < 0.0) return kPi - acos01(-x);
    if (x >= 1.0) return 0.0;
    return 2.0 * atan01(std::sqrt((1.0 - x) / (1.0 + x)));
}

// sin of 0 <= x <= pi, by a fixed series.
double sin0pi(double x) {
    if (x > kPi / 2.0) x = kPi - x;
    const double q = x * x;
    double series = 1.0;
    for (const double n : {20.0, 18.0, 16.0, 14.0, 12.0, 10.0, 8.0, 6.0, 4.0, 2.0}) {
        series = 1.0 - q * series / (n * (n + 1.0));
    }
    return x * series;
}

Quat q_mul(const Quat &a, const Quat &b) {
    const double ax = a[0], ay = a[1], az = a[2], aw = a[3];
    const double bx = b[0], by = b[1], bz = b[2], bw = b[3];
    return {aw * bx + ax * bw + ay * bz - az * by, aw * by - ax * bz + ay * bw + az * bx,
            aw * bz + ax * by - ay * bx + az * bw, aw * bw - ax * bx - ay * by - az * bz};
}

Quat q_conj(const Quat &q) { return {-q[0], -q[1], -q[2], q[3]}; }

Quat q_norm(const Quat &q) {
    const double n = std::sqrt(dot4(q, q));
    if (n != 0.0) return {q[0] / n, q[1] / n, q[2] / n, q[3] / n};
    return kIdentity;
}

Vec3 q_rot(const Quat &q, const Vec3 &v) {
    const double x = q[0], y = q[1], z = q[2], w = q[3];
    const double vx = v[0], vy = v[1], vz = v[2];
    const double tx = 2.0 * (y * vz - z * vy), ty = 2.0 * (z * vx - x * vz), tz = 2.0 * (x * vy - y * vx);
    return {vx + w * tx + y * tz - z * ty, vy + w * ty + z * tx - x * tz, vz + w * tz + x * ty - y * tx};
}

// The shortest rotation taking unit vector a to unit vector b.
Quat q_arc(const Vec3 &a, const Vec3 &b) {
    const double d = dot3(a, b);
    if (d < -0.999999) return {1.0, 0.0, 0.0, 0.0};
    return q_norm({a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0], 1.0 + d});
}

// The angle between two rotations, in degrees.
double q_angle(const Quat &a, const Quat &b) {
    const double d = std::fabs(dot4(q_norm(a), q_norm(b)));
    return 2.0 * acos01(d < 1.0 ? d : 1.0) * 180.0 / kPi;
}

Quat slerp(const Quat &a, Quat b, double t) {
    double d = dot4(a, b);
    if (d < 0.0) {
        b = {-b[0], -b[1], -b[2], -b[3]};
        d = -d;
    }
    if (d > 0.9995) {
        return q_norm({a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t,
                       a[3] + (b[3] - a[3]) * t});
    }
    const double theta = acos01(d);
    const double s = sin0pi(theta);
    const double wa = over(sin0pi((1.0 - t) * theta), s), wb = over(sin0pi(t * theta), s);
    return {a[0] * wa + b[0] * wb, a[1] * wa + b[1] * wb, a[2] * wa + b[2] * wb, a[3] * wa + b[3] * wb};
}

Vec3 lerp(const Vec3 &a, const Vec3 &b, double t) {
    return {a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t};
}

// A rotation and a vector seen in a mirror across the body's side-to-side
// plane (x = 0).
Quat mirror_q(const Quat &q) { return {q[0], -q[1], -q[2], q[3]}; }

Vec3 mirror_v(const Vec3 &v) { return {-v[0], v[1], v[2]}; }

Vec3 sub(const Vec3 &a, const Vec3 &b) { return {a[0] - b[0], a[1] - b[1], a[2] - b[2]}; }

Vec3 add(const Vec3 &a, const Vec3 &b) { return {a[0] + b[0], a[1] + b[1], a[2] + b[2]}; }

Vec3 unit(const Vec3 &v) {
    const double n = std::sqrt(dot3(v, v));
    if (n != 0.0) return {v[0] / n, v[1] / n, v[2] / n};
    return {0.0, 1.0, 0.0};
}

// The rotation of a matrix given by rows.
Quat matrix_quat(const std::array<Vec3, 3> &m) {
    const double m00 = m[0][0], m01 = m[0][1], m02 = m[0][2];
    const double m10 = m[1][0], m11 = m[1][1], m12 = m[1][2];
    const double m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];
    const double trace = m00 + m11 + m22;
    if (trace > 0.0) {
        const double s = root(trace + 1.0) * 2.0;
        return q_norm({over(m21 - m12, s), over(m02 - m20, s), over(m10 - m01, s), 0.25 * s});
    }
    if (m00 > m11 && m00 > m22) {
        const double s = root(1.0 + m00 - m11 - m22) * 2.0;
        return q_norm({0.25 * s, over(m01 + m10, s), over(m02 + m20, s), over(m21 - m12, s)});
    }
    if (m11 > m22) {
        const double s = root(1.0 + m11 - m00 - m22) * 2.0;
        return q_norm({over(m01 + m10, s), 0.25 * s, over(m12 + m21, s), over(m02 - m20, s)});
    }
    const double s = root(1.0 + m22 - m00 - m11) * 2.0;
    return q_norm({over(m02 + m20, s), over(m12 + m21, s), 0.25 * s, over(m10 - m01, s)});
}

// Where a bone is in the model, from its inverse bind matrix (16 floats,
// column major).
Placement inverse_bind(const std::array<double, 16> &m) {
    const double a = m[0], b = m[4], c = m[8], d = m[1], e = m[5], f = m[9], g = m[2], h = m[6], i = m[10];
    const Vec3 t = {m[12], m[13], m[14]};
    const double det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
    const std::array<Vec3, 3> inv = {{
        {over(e * i - f * h, det), over(c * h - b * i, det), over(b * f - c * e, det)},
        {over(f * g - d * i, det), over(a * i - c * g, det), over(c * d - a * f, det)},
        {over(d * h - e * g, det), over(b * g - a * h, det), over(a * e - b * d, det)},
    }};
    Placement out;
    for (std::size_t k = 0; k < 3u; ++k) out.translate[k] = -(inv[k][0] * t[0] + inv[k][1] * t[1] + inv[k][2] * t[2]);
    out.rotate = matrix_quat(inv);
    return out;
}

// ------------------------------------------------------------------ curves

Curve decode_curve(const Chunk &chunk) {
    if (chunk.args.size() != 16u) throw Error("a curve's header is not 16 bytes");
    const std::uint32_t type = load32(chunk.args, 0u);
    const std::uint64_t dims = load32(chunk.args, 4u);
    const std::uint64_t count = load32(chunk.args, 8u);
    Curve curve;
    curve.interpolation = type & 0xFu;
    if (curve.interpolation >= kPerDim.size()) throw Error("a curve's interpolation is unknown");
    const bool half = (type & kHalfFloats) != 0u;
    const std::uint64_t per = 1u + dims * kPerDim[curve.interpolation];
    const std::uint64_t each = half ? 2u : 4u;
    if (count != 0u && per > chunk.data.size() / each / count) throw Error("a curve is cut short");
    curve.width = static_cast<std::size_t>(per - 1u);
    curve.frames.reserve(static_cast<std::size_t>(count));
    curve.values.reserve(static_cast<std::size_t>(count * (per - 1u)));
    for (std::uint64_t k = 0; k < count * per; ++k) {
        const double value = half ? half_value(load16(chunk.data, k * 2u)) : float_value(load32(chunk.data, k * 4u));
        (k % per == 0u ? curve.frames : curve.values).push_back(value);
    }
    return curve;
}

// The first `want` values of a curve at a frame: the key at or before it
// for a constant curve, else between the keys either side of it.
void sample(const Curve &curve, double frame, double *out, std::size_t want) {
    const std::size_t count = curve.frames.size();
    if (count == 0u) throw Error("a curve has no keys");
    const auto key = [&curve](std::size_t k) { return curve.values.data() + k * curve.width; };
    const auto copy = [&curve, out, want](const double *values) {
        if (curve.width < want) throw Error("a curve has too few values");
        std::copy_n(values, want, out);
    };
    if (count == 1u || frame <= curve.frames.front()) return copy(key(0u));
    if (frame >= curve.frames.back()) return copy(key(count - 1u));
    std::size_t lo = 0u, hi = count - 1u;
    while (hi - lo > 1u) {
        const std::size_t mid = (lo + hi) / 2u;
        if (curve.frames[mid] <= frame) lo = mid;
        else hi = mid;
    }
    if (curve.interpolation == kConstant) return copy(key(lo));
    const double t0 = curve.frames[lo], t1 = curve.frames[hi];
    const double *v0 = key(lo), *v1 = key(hi);
    const double t = over(frame - t0, t1 - t0);
    if (curve.interpolation == kSpherical) {
        if (curve.width < 4u) throw Error("a curve has too few values");
        const Quat q = slerp({v0[0], v0[1], v0[2], v0[3]}, {v1[0], v1[1], v1[2], v1[3]}, t);
        std::copy_n(q.begin(), std::min<std::size_t>(want, 4u), out);
        return;
    }
    if (curve.width < want) throw Error("a curve has too few values");
    for (std::size_t k = 0; k < want; ++k) out[k] = v0[k] + (v1[k] - v0[k]) * t;
}

// A motion's chunks: FrameLoop (start, end), FrameRate, FrameRepeat, then
// Animate commands {bone, command, curve} and the curves.
Motion decode_motion(Bytes data, std::string name, std::uint64_t begin, std::uint64_t end) {
    const std::vector<Chunk> children = chunks(data, begin, end);
    Motion motion;
    motion.name = std::move(name);
    for (const Chunk &c : children) {
        if (c.kind() == kFCurve) motion.curves.push_back(decode_curve(c));
    }
    for (const Chunk &c : children) {
        if (c.kind() == kFrameLoop) {
            motion.loop = floats<2>(c.payload);
        } else if (c.kind() == kFrameRate || c.kind() == kFrameRepeat) {
            (void)load32(c.payload, 0u);
        } else if (c.kind() == kAnimate) {
            if (c.payload.size() != 16u) throw Error("an Animate command is not 16 bytes");
            const std::uint32_t block = load32(c.payload, 0u), command = load32(c.payload, 4u);
            const std::uint32_t curve = load32(c.payload, 12u);
            if (block == kNoRef || curve == kNoRef) throw Error("an Animate command names nothing");
            const std::size_t bone = block & 0xFFFu, index = curve & 0xFFFu;
            if (index >= motion.curves.size()) throw Error("an Animate command names a curve the motion lacks");
            if (motion.tracks.size() <= bone) motion.tracks.resize(bone + 1u);
            if (command == kCommandRotate) motion.tracks[bone].rotate = static_cast<int>(index);
            if (command == kCommandTranslate) motion.tracks[bone].translate = static_cast<int>(index);
        }
    }
    return motion;
}

// ------------------------------------------------------------------ writing

// A name, its zero, and zeros up to a multiple of four bytes.
std::vector<std::uint8_t> name_bytes(std::string_view name) {
    std::vector<std::uint8_t> out(name.begin(), name.end());
    out.push_back(0u);
    pad4(out, 0u);
    return out;
}

void put_half_chunk(std::vector<std::uint8_t> &out, std::uint16_t type, const std::vector<std::uint32_t> &words) {
    store16(out, static_cast<std::uint16_t>(kHalf | type));
    store16(out, 0u);
    store32(out, static_cast<std::uint32_t>(8u + 4u * words.size()));
    for (const std::uint32_t word : words) store32(out, word);
}

// A chunk with a name: its header, then args, data and its chunks.
void put_chunk(std::vector<std::uint8_t> &out, std::uint16_t type, std::string_view name, Bytes args, Bytes data,
               Bytes children) {
    const std::vector<std::uint8_t> raw = name_bytes(name);
    const std::size_t header = 16u + raw.size();
    if (header > 0xFFFFu) throw Error("a chunk's name is too long");
    const std::size_t at = header + args.size();
    const std::size_t child = at + data.size();
    store16(out, type);
    store16(out, static_cast<std::uint16_t>(header));
    store32(out, static_cast<std::uint32_t>(child + children.size()));
    store32(out, static_cast<std::uint32_t>(child));
    store32(out, static_cast<std::uint32_t>(at));
    out.insert(out.end(), raw.begin(), raw.end());
    out.insert(out.end(), args.begin(), args.end());
    out.insert(out.end(), data.begin(), data.end());
    out.insert(out.end(), children.begin(), children.end());
}

// A curve to write: its keys as (frame, values), end to end.
struct NewCurve {
    std::uint32_t interpolation{};
    std::uint32_t dims{};
    std::uint32_t keys{};
    std::vector<double> flat;
};

struct NewTrack {
    std::size_t bone{};
    std::uint32_t command{};
    NewCurve curve;
};

// A curve's chunk; its keys are written as half floats.
void put_curve(std::vector<std::uint8_t> &out, const NewCurve &curve, std::size_t number) {
    std::vector<std::uint8_t> args, body;
    store32(args, curve.interpolation | kHalfFloats);
    store32(args, curve.dims);
    store32(args, curve.keys);
    store32(args, 0u);
    body.reserve(curve.flat.size() * 2u + 2u);
    for (const double value : curve.flat) store16(body, half_bits(value));
    pad4(body, 0u);
    put_chunk(out, kFCurve, "fcurve-" + std::to_string(number), args, body, {});
}

// ------------------------------------------------------------------ rebuilding

// How far apart two values of a track are: degrees for rotations, units
// for translations.
double apart(const Quat &a, const Quat &b) { return q_angle(a, b); }
double apart(const Vec3 &a, const Vec3 &b) { return distance(a, b); }

Quat between(const Quat &a, const Quat &b, double t) { return slerp(a, b, t); }
Vec3 between(const Vec3 &a, const Vec3 &b, double t) { return lerp(a, b, t); }

// gmo_motion.reduce_keys: the frames to keep so that every frame stays
// within `tolerance` of the curve through the kept ones, by stretching each
// span for as long as it fits.
template <class Value> std::vector<std::size_t> reduce_keys(const std::vector<Value> &values, double tolerance) {
    const auto fits = [&values, tolerance](std::size_t i, std::size_t j) {
        for (std::size_t k = i + 1u; k < j; ++k) {
            const double t = static_cast<double>(k - i) / static_cast<double>(j - i);
            if (!(apart(between(values[i], values[j], t), values[k]) <= tolerance)) return false;
        }
        return true;
    };
    std::vector<std::size_t> kept{0u};
    std::size_t i = 0u;
    while (i + 1u < values.size()) {
        std::size_t j = i + 1u;
        while (j + 1u < values.size() && fits(i, j + 1u)) ++j;
        kept.push_back(j);
        i = j;
    }
    return kept;
}

// The curve of a track's values, one per frame: a constant when the kept
// keys are all the same, else spherical (rotations) or linear.
template <class Value> NewCurve track_curve(const std::vector<Value> &values, double tolerance) {
    const std::vector<std::size_t> kept = reduce_keys(values, tolerance);
    const Value &first = values[kept[0]];
    const bool constant = kept.size() == 1u || std::all_of(kept.begin(), kept.end(), [&](std::size_t k) {
        return apart(values[k], first) < 1e-5;
    });
    const bool spherical = first.size() == 4u;
    NewCurve curve;
    curve.dims = static_cast<std::uint32_t>(first.size());
    if (constant) {
        curve.interpolation = kConstant;
        curve.keys = 1u;
        curve.flat.push_back(0.0);
        curve.flat.insert(curve.flat.end(), first.begin(), first.end());
        return curve;
    }
    curve.interpolation = spherical ? kSpherical : kLinear;
    curve.keys = static_cast<std::uint32_t>(kept.size());
    for (const std::size_t k : kept) {
        curve.flat.push_back(static_cast<double>(k));
        curve.flat.insert(curve.flat.end(), values[k].begin(), values[k].end());
    }
    return curve;
}

// The same bone of the other side: l_ for r_ and r_ for l_.
std::string flip(const std::string &name) {
    if (name.starts_with("l_")) return "r_" + name.substr(2u);
    if (name.starts_with("r_")) return "l_" + name.substr(2u);
    return name;
}

std::size_t need(const Skeleton &skeleton, const std::string &name) {
    const std::optional<std::size_t> index = skeleton.find(name);
    if (!index) throw Error("the model has no bone " + name);
    return *index;
}

// A limb bone and the next one along it: the direction the bone points in.
constexpr std::array<std::pair<std::string_view, std::string_view>, 10> kLimbChild = {{
    {"l_femur", "l_tibia"}, {"l_tibia", "l_foot"}, {"l_foot", "l_toe"}, {"r_femur", "r_tibia"},
    {"r_tibia", "r_foot"}, {"r_foot", "r_toe"}, {"l_uparm", "l_foarm"}, {"l_foarm", "l_hand"},
    {"r_uparm", "r_foarm"}, {"r_foarm", "r_hand"},
}};
// World-level bones: their values are copied as they are.
constexpr std::array<std::string_view, 3> kCopied = {"camera_include", "camera2_include", "trans_include"};
// Bartz's star pairs with no bone; it stays this far above his head.
constexpr std::string_view kStar = "star";
constexpr Vec3 kStarOffset = {0.0, 0.42, 0.0};
constexpr double kTolerance = 0.25;  // degrees; a hundredth of it in units for translations
constexpr std::size_t kNone = static_cast<std::size_t>(-1);

} // namespace

// ------------------------------------------------------------------ skeletons

Skeleton::Skeleton(std::span<const std::uint8_t> gmo) : file_(gmo.begin(), gmo.end()) {
    const Bytes data(file_);
    if (data.size() < kMagic.size() || !std::equal(kMagic.begin(), kMagic.end(), data.begin())) {
        throw Error("not a GMO model");
    }
    const std::vector<Chunk> top = chunks(data, kMagic.size(), data.size());
    for (const Chunk &chunk : top) check(data, chunk, 0);
    if (top.size() != 1u || top[0].kind() != kFile) throw Error("a GMO model has one File chunk");

    // A fighter file holds a motion model (bones and motions) and a mesh
    // model (the same bones with the parts and their inverse bind matrices).
    const std::vector<Chunk> models = chunks(data, top[0].below, top[0].end);
    const Chunk *motion_model = nullptr, *mesh = nullptr;
    for (const Chunk &model : models) {
        if (model.kind() != kModel) continue;
        if (motion_model == nullptr && has_child(data, model, kMotion)) motion_model = &model;
        if (mesh == nullptr && has_child(data, model, kPart)) mesh = &model;
    }
    if (motion_model == nullptr) throw Error("the GMO model has no motions");

    for (const Chunk &chunk : chunks(data, motion_model->below, motion_model->end)) {
        if (chunk.kind() == kMotion && !chunk.half()) {
            motions_.push_back({text(chunk.name), static_cast<std::size_t>(chunk.below),
                                static_cast<std::size_t>(chunk.end)});
        }
        if (chunk.kind() != kBone) continue;
        if (chunk.half()) throw Error("a bone has no name");
        Bone bone;
        bone.name = text(chunk.name);
        for (const Chunk &c : chunks(data, chunk.below, chunk.end)) {
            if (c.kind() == kParentBone) {
                const std::uint32_t word = load32(c.payload, 0u);
                bone.parent = word == kNoRef ? -1 : static_cast<int>(word & 0xFFFu);
            } else if (c.kind() == kTranslate) {
                bone.translate = floats<3>(c.payload);
            } else if (c.kind() == kRotateQ) {
                bone.rotate = floats<4>(c.payload);
            }
        }
        bones_.push_back(std::move(bone));
    }

    // Parents before their children.
    const std::size_t count = bones_.size();
    std::vector<bool> placed(count, false);
    std::vector<std::size_t> chain;
    for (std::size_t i = 0; i < count; ++i) {
        chain.clear();
        for (std::size_t at = i; !placed[at];) {
            if (chain.size() > count) throw Error("the bones' parents go in a circle");
            chain.push_back(at);
            const int parent = bones_[at].parent;
            if (parent < 0) break;
            if (static_cast<std::size_t>(parent) >= count) throw Error("a bone's parent is not in the model");
            at = static_cast<std::size_t>(parent);
        }
        for (auto at = chain.rbegin(); at != chain.rend(); ++at) {
            placed[*at] = true;
            order_.push_back(*at);
        }
    }

    std::vector<Placement> local(count);
    for (std::size_t i = 0; i < count; ++i) local[i] = {bones_[i].rotate, bones_[i].translate};
    rest_ = world(local);

    // The bind pose of the bones the mesh is skinned to: a part's first
    // mention of a bone gives it.
    std::vector<bool> skinned(std::max<std::size_t>(0x1000u, count), false);
    bind_.resize(count);
    if (mesh != nullptr) {
        for (const Chunk &chunk : chunks(data, mesh->below, mesh->end)) {
            if (chunk.kind() != kBone) continue;
            const std::vector<Chunk> children = chunks(data, chunk.below, chunk.end);
            const Chunk *blend_bones = nullptr, *offsets = nullptr;
            for (const Chunk &c : children) {
                if (blend_bones == nullptr && c.kind() == kBlendBones) blend_bones = &c;
                if (offsets == nullptr && c.kind() == kBlendOffsets) offsets = &c;
            }
            if (blend_bones == nullptr || offsets == nullptr) continue;
            const std::uint32_t refs = load32(blend_bones->payload, 0u);
            if ((blend_bones->payload.size() - 4u) / 4u < refs) throw Error("a part's bone list is cut short");
            for (std::uint32_t k = 0; k < refs; ++k) {
                const std::uint32_t word = load32(blend_bones->payload, 4u + 4u * std::uint64_t{k});
                const std::size_t index = word & 0xFFFu;
                if (word == kNoRef || skinned[index]) continue;
                const Placement placement = inverse_bind(floats<16>(offsets->payload, 4u + 64u * std::uint64_t{k}));
                skinned[index] = true;
                if (index < count) bind_[index] = placement;
            }
        }
    }
    // Bones no part is skinned to follow their parent's bind pose with
    // their own rest offset.
    for (const std::size_t i : order_) {
        if (skinned[i]) continue;
        const Bone &bone = bones_[i];
        if (bone.parent < 0) {
            bind_[i] = {bone.rotate, bone.translate};
        } else {
            const Placement &parent = bind_[static_cast<std::size_t>(bone.parent)];
            bind_[i] = {q_norm(q_mul(parent.rotate, bone.rotate)),
                        add(parent.translate, q_rot(parent.rotate, bone.translate))};
        }
    }
}

std::optional<std::size_t> Skeleton::find(std::string_view name) const {
    for (std::size_t i = bones_.size(); i-- > 0u;) {
        if (bones_[i].name == name) return i;
    }
    return std::nullopt;
}

bool Skeleton::has_motion(std::string_view name) const {
    return std::any_of(motions_.begin(), motions_.end(), [name](const Stored &m) { return m.name == name; });
}

Motion Skeleton::motion(std::string_view name) const {
    // The last of that name, when several share it.
    for (auto stored = motions_.rbegin(); stored != motions_.rend(); ++stored) {
        if (stored->name == name) return decode_motion(file_, stored->name, stored->begin, stored->end);
    }
    throw Error("the model has no motion " + std::string(name));
}

std::vector<Placement> Skeleton::pose(const Motion &motion, double frame) const {
    std::vector<Placement> out(bones_.size());
    for (std::size_t i = 0; i < bones_.size(); ++i) {
        out[i] = {bones_[i].rotate, bones_[i].translate};
        if (i >= motion.tracks.size()) continue;
        const Motion::Track &track = motion.tracks[i];
        if (track.rotate >= 0) {
            sample(motion.curves[static_cast<std::size_t>(track.rotate)], frame, out[i].rotate.data(), 4u);
        }
        if (track.translate >= 0) {
            sample(motion.curves[static_cast<std::size_t>(track.translate)], frame, out[i].translate.data(), 3u);
        }
    }
    return out;
}

std::vector<Placement> Skeleton::world(const std::vector<Placement> &local) const {
    if (local.size() < bones_.size()) throw Error("a pose has fewer bones than the model");
    std::vector<Placement> out(bones_.size());
    for (const std::size_t i : order_) {
        const int parent = bones_[i].parent;
        if (parent < 0) {
            out[i] = {q_norm(local[i].rotate), local[i].translate};
        } else {
            const Placement &above = out[static_cast<std::size_t>(parent)];
            out[i] = {q_norm(q_mul(above.rotate, local[i].rotate)),
                      add(above.translate, q_rot(above.rotate, local[i].translate))};
        }
    }
    return out;
}

// ------------------------------------------------------------------ retarget

// The fighters' skeletons share bone names (hip, waist, chest, l_uparm,
// r_femur ...), so bones pair by name. The method works in the model's
// space, so different bone axes and rest poses do not matter:
//   delta(b, t)  = source world rotation of b at t * inverse(source bind rotation of b)
//   C(b)         = shortest rotation from the target bone's bind direction to the source bone's (limbs only)
//   world(b, t)  = delta(b, t) * C(b) * target bind rotation of b
//   local(b, t)  = inverse(world(parent, t)) * world(b, t)
// A mirrored move takes the source's opposite bone (l_ <-> r_) and reflects
// its turn across the side-to-side plane. Bones the source lacks keep their
// rest values.
// Translations: `root` (the body) scales by root_scale, `trans_include`
// (the distance the move covers in play) by trans_scale, the camera bones
// are copied, and a moving prop bone (a thrown shield) keeps its path
// relative to its parent.
std::vector<std::uint8_t> retarget(const Skeleton &source, const Skeleton &target, const Retarget &options) {
    const Motion src = source.motion(options.motion);
    if (!src.loop) throw Error("the motion " + options.motion + " has no frame loop");
    if (options.frames < 0) throw Error("an animation cannot end before frame 0");
    const double lo = (*src.loop)[0], hi = (*src.loop)[1];
    const std::vector<Bone> &bones = target.bones();
    const std::size_t count = bones.size();
    const bool mirror = options.mirror;

    // The source bone each target bone follows.
    std::vector<std::size_t> pair(count, kNone);
    std::vector<bool> copied(count, false);
    for (std::size_t i = 0; i < count; ++i) {
        const std::string &name = bones[i].name;
        copied[i] = std::find(kCopied.begin(), kCopied.end(), name) != kCopied.end();
        if (name == kStar) continue;
        if (const std::optional<std::size_t> theirs = source.find(mirror ? flip(name) : name)) pair[i] = *theirs;
    }
    for (const auto &[mine, theirs] : options.follow) pair[need(target, mine)] = need(source, theirs);
    std::vector<const Held *> held(count, nullptr);
    for (const auto &[name, value] : options.held) held[need(target, name)] = &value;
    for (std::size_t i = 0; i < count; ++i) {
        if (held[i] != nullptr) pair[i] = kNone;
    }

    // C(b) of the limbs.
    std::vector<Quat> fix(count, kIdentity);
    for (const auto &[name, child] : kLimbChild) {
        const std::optional<std::size_t> mine = target.find(name), mine_child = target.find(child);
        if (!mine || !mine_child) continue;
        const std::optional<std::size_t> theirs = source.find(mirror ? flip(std::string(name)) : std::string(name));
        const std::optional<std::size_t> theirs_child =
            source.find(mirror ? flip(std::string(child)) : std::string(child));
        if (!theirs || !theirs_child) continue;
        const Vec3 dt = unit(sub(target.bind()[*mine_child].translate, target.bind()[*mine].translate));
        Vec3 ds = unit(sub(source.bind()[*theirs_child].translate, source.bind()[*theirs].translate));
        if (mirror) ds = mirror_v(ds);
        fix[*mine] = q_arc(dt, ds);
    }
    // Whether a source bone's translation changes in the motion.
    const auto moving = [&src](std::size_t bone) {
        if (bone >= src.tracks.size() || src.tracks[bone].translate < 0) return false;
        return src.curves[static_cast<std::size_t>(src.tracks[bone].translate)].frames.size() > 1u;
    };
    const std::optional<std::size_t> root_bone = target.find("root"), head = target.find("head");
    const std::optional<std::size_t> star = target.find(kStar);

    // Every bone's own rotation and translation at every frame.
    const std::size_t steps = static_cast<std::size_t>(options.frames) + 1u;
    std::vector<std::vector<Quat>> rotations(count);
    std::vector<std::vector<Vec3>> translations(count);
    std::vector<Placement> local(count), world(count);
    for (std::size_t k = 0; k < steps; ++k) {
        // The source's frame, kept inside its loop.
        const double wanted = options.start + static_cast<double>(k) * options.speed;
        const double from = wanted > lo ? wanted : lo;
        const double frame = from < hi ? from : hi;
        const std::vector<Placement> local_src = source.pose(src, frame);
        const std::vector<Placement> world_src = source.world(local_src);
        for (const std::size_t i : target.order()) {
            const Bone &bone = bones[i];
            const Quat parent_rot = bone.parent >= 0 ? world[static_cast<std::size_t>(bone.parent)].rotate : kIdentity;
            Quat rot = bone.rotate;
            Vec3 trans = bone.translate;
            const std::size_t s = pair[i];
            if (s != kNone && copied[i]) {
                const Placement &theirs = local_src[s];
                rot = mirror ? mirror_q(theirs.rotate) : theirs.rotate;
                trans = mirror ? mirror_v(theirs.translate) : theirs.translate;
                if (bone.name == "trans_include") {
                    for (double &x : trans) x = x * options.trans_scale;
                }
            } else if (s != kNone) {
                Quat delta = q_mul(world_src[s].rotate, q_conj(source.bind()[s].rotate));
                if (mirror) delta = mirror_q(delta);
                const Quat w = q_norm(q_mul(q_mul(delta, fix[i]), target.bind()[i].rotate));
                rot = q_norm(q_mul(q_conj(parent_rot), w));
                const int source_parent = source.bones()[s].parent;
                if (root_bone && i == *root_bone) {
                    trans = mirror ? mirror_v(local_src[s].translate) : local_src[s].translate;
                    for (double &x : trans) x = x * options.root_scale;
                } else if (moving(s) && source_parent >= 0) {
                    // A prop on the move: its offset from the parent, in
                    // the model's space, scaled like the body.
                    Vec3 off = q_rot(world_src[static_cast<std::size_t>(source_parent)].rotate,
                                     sub(local_src[s].translate, source.bones()[s].translate));
                    if (mirror) off = mirror_v(off);
                    const Vec3 back = q_rot(q_conj(parent_rot), off);
                    for (std::size_t c = 0; c < 3u; ++c) trans[c] = bone.translate[c] + options.root_scale * back[c];
                }
            } else if (held[i] != nullptr) {
                if (held[i]->rotate) rot = *held[i]->rotate;
                if (held[i]->translate) trans = *held[i]->translate;
            }
            local[i] = {rot, trans};
            if (bone.parent < 0) {
                world[i] = {q_norm(rot), trans};
            } else {
                const Placement &above = world[static_cast<std::size_t>(bone.parent)];
                world[i] = {q_norm(q_mul(above.rotate, rot)), add(above.translate, q_rot(above.rotate, trans))};
            }
        }
        if (star && head && bones[*star].parent < 0) local[*star].translate = add(world[*head].translate, kStarOffset);
        for (std::size_t i = 0; i < count; ++i) {
            // The same rotation with the sign of the frame before, so the
            // curve never takes the long way round.
            Quat rot = local[i].rotate;
            if (!rotations[i].empty() && dot4(rotations[i].back(), rot) < 0.0) {
                rot = {-rot[0], -rot[1], -rot[2], -rot[3]};
            }
            rotations[i].push_back(rot);
            translations[i].push_back(local[i].translate);
        }
    }

    // The tracks: a bone's rotation when it follows a source bone or ever
    // leaves its rest value, its translation when it ever leaves it.
    const auto furthest = [](const auto &values, const auto &rest) {
        double most = apart(values[0], rest);
        for (std::size_t k = 1; k < values.size(); ++k) {
            const double far = apart(values[k], rest);
            if (far > most) most = far;
        }
        return most;
    };
    std::vector<NewTrack> tracks;
    for (std::size_t i = 0; i < count; ++i) {
        if (furthest(rotations[i], bones[i].rotate) > 1e-4 || pair[i] != kNone) {
            tracks.push_back({i, kCommandRotate, track_curve(rotations[i], kTolerance)});
        }
        if (furthest(translations[i], bones[i].translate) > 1e-4) {
            tracks.push_back({i, kCommandTranslate, track_curve(translations[i], kTolerance / 100.0)});
        }
    }
    // Scale keys as they are: a weapon that grows in and out.
    for (std::size_t at = 0; at < options.scales.size(); ++at) {
        const std::string &name = options.scales[at].first;
        const auto same = [&name](const auto &entry) { return entry.first == name; };
        if (std::any_of(options.scales.begin(), options.scales.begin() + static_cast<std::ptrdiff_t>(at), same)) {
            continue;
        }
        const auto last = std::find_if(options.scales.rbegin(), options.scales.rend(), same);
        NewCurve curve;
        curve.interpolation = kLinear;
        curve.dims = 3u;
        curve.keys = static_cast<std::uint32_t>(last->second.size());
        for (const ScaleKey &key : last->second) {
            curve.flat.push_back(key.frame);
            curve.flat.insert(curve.flat.end(), key.value.begin(), key.value.end());
        }
        tracks.push_back({need(target, name), kCommandScale, std::move(curve)});
    }

    // The Motion chunk, laid out as the game's own: FrameLoop, FrameRate,
    // FrameRepeat, the Animate commands, the curves.
    std::vector<std::uint8_t> children;
    put_half_chunk(children, kFrameLoop, {float_bits(0.0), float_bits(static_cast<double>(options.frames))});
    put_half_chunk(children, kFrameRate, {float_bits(30.0)});
    put_half_chunk(children, kFrameRepeat, {0u});
    for (std::size_t k = 0; k < tracks.size(); ++k) {
        // References: index, level << 12, chunk type << 16.
        const std::uint32_t block = static_cast<std::uint32_t>(tracks[k].bone & 0xFFFu) | 1u << 12u |
                                    static_cast<std::uint32_t>(kBone) << 16u;
        const std::uint32_t curve = static_cast<std::uint32_t>(k & 0xFFFu) | static_cast<std::uint32_t>(kFCurve) << 16u;
        put_half_chunk(children, kAnimate, {block, tracks[k].command, 0u, curve});
    }
    for (std::size_t k = 0; k < tracks.size(); ++k) put_curve(children, tracks[k].curve, k);
    std::vector<std::uint8_t> out;
    put_chunk(out, kMotion, options.name.empty() ? options.motion : options.name, {}, {}, children);
    return out;
}

} // namespace d012::gmo_motion
