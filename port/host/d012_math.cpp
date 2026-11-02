// The game's library maths, the same on every machine.
//
// The recompiled game reaches five float functions of the C runtime, through
// the VFPU's vsin, vcos, vrot, vasin, vlog2 and vexp2: sinf, cosf, asinf,
// log2f and exp2f. Each C runtime has its own, and they round the last bit
// differently for some inputs: Microsoft's on Windows and Wine's under Proton
// part two copies of a network battle sooner or later (docs/port/
// communication-mode.md, section 13). Defined here, in the program itself,
// they take the place of the C runtime's for all of its code, and give the
// same bits wherever the program runs. The installer's game code is compiled
// apart from the program, into libraries that would take the C runtime's:
// --compile-game builds this file into each of them too (sdk/d012_math.cpp).
//
// Each works in double precision with nothing but IEEE additions,
// multiplications, divisions and square roots, which every machine rounds
// alike: the argument brought into a small range by a whole number of known
// steps (quarter turns, octaves, binary exponents), then a short series, then
// one rounding to float. They are not correctly rounded in every case, only
// to well within a float's last bit, and always the same.
//
// MIT licence; see port/README.md.

#include <cstdint>
#include <cstring>

namespace {

constexpr double kHalfPi = 1.57079632679489661923;
constexpr double kTwoOverPi = 0.63661977236758134308;
// pi/2 in three parts, the first two short enough that a quarter-turn count of
// up to 2^20 times them is exact.
constexpr double kHalfPi1 = 1.57079632673412561417e+00;
constexpr double kHalfPi2 = 6.07710050630396597660e-11;
constexpr double kHalfPi3 = 2.02226624879595063154e-21;
constexpr double kLn2 = 0.693147180559945309417;
constexpr double kSqrt2 = 1.41421356237309504880;

std::uint32_t bits_of(float f) {
    std::uint32_t b;
    std::memcpy(&b, &f, 4);
    return b;
}

float float_of(std::uint32_t b) {
    float f;
    std::memcpy(&f, &b, 4);
    return f;
}

double double_of(std::uint64_t b) {
    double d;
    std::memcpy(&d, &b, 8);
    return d;
}

bool is_nan_or_inf(float x) { return (bits_of(x) & 0x7F800000u) == 0x7F800000u; }

// The nearest whole number, halves away from zero, for |x| < 2^52.
double nearest(double x) {
    const double whole = static_cast<double>(static_cast<std::int64_t>(x));
    const double rest = x - whole;
    if (rest >= 0.5) return whole + 1.0;
    if (rest <= -0.5) return whole - 1.0;
    return whole;
}

// sin and cos of |r| <= pi/4 (and a little more), to about 1e-16.
double sin_small(double r) {
    const double z = r * r;
    // r - r^3/3! + r^5/5! - ... + r^17/17!
    double t = 1.0 / 355687428096000.0;
    t = t * z - 1.0 / 1307674368000.0;
    t = t * z + 1.0 / 6227020800.0;
    t = t * z - 1.0 / 39916800.0;
    t = t * z + 1.0 / 362880.0;
    t = t * z - 1.0 / 5040.0;
    t = t * z + 1.0 / 120.0;
    t = t * z - 1.0 / 6.0;
    return r + r * z * t;
}

double cos_small(double r) {
    const double z = r * r;
    // 1 - r^2/2! + r^4/4! - ... + r^18/18!
    double t = -1.0 / 6402373705728000.0;
    t = t * z + 1.0 / 20922789888000.0;
    t = t * z - 1.0 / 87178291200.0;
    t = t * z + 1.0 / 479001600.0;
    t = t * z - 1.0 / 3628800.0;
    t = t * z + 1.0 / 40320.0;
    t = t * z - 1.0 / 720.0;
    t = t * z + 1.0 / 24.0;
    t = t * z - 0.5;
    return 1.0 + z * t;
}

// x as a whole number of quarter turns plus a remainder within about pi/4.
// Up to 2^20 quarter turns the three-part pi/2 keeps the remainder exact to
// double precision; past that a float's own step is already a sizeable part
// of a turn, and the plain product does.
void quarter_turns(double x, std::int64_t &quadrant, double &rest) {
    const double k = nearest(x * kTwoOverPi);
    if (k <= 1048576.0 && k >= -1048576.0)
        rest = ((x - k * kHalfPi1) - k * kHalfPi2) - k * kHalfPi3;
    else
        rest = x - k * kHalfPi;
    quadrant = static_cast<std::int64_t>(k);
}

double sin_of(double x, int shift) {
    // Past 1e15 there is not a single bit of a turn left in a float: zero turns.
    if (!(x < 1e15 && x > -1e15)) x = 0.0;
    std::int64_t quadrant = 0;
    double rest = 0.0;
    quarter_turns(x, quadrant, rest);
    switch ((quadrant + shift) & 3) {
    case 0: return sin_small(rest);
    case 1: return cos_small(rest);
    case 2: return -sin_small(rest);
    default: return -cos_small(rest);
    }
}

// asin of |z| <= 0.5, by its series, to about 1e-16.
double asin_small(double z) {
    const double z2 = z * z;
    double term = z;
    double sum = z;
    // term(n+1) = term(n) * z^2 * (2n+1)^2 / ((2n+2)(2n+3))
    for (int n = 0; n < 40; ++n) {
        const double a = 2.0 * n + 1.0;
        term = term * z2 * a * a / ((a + 1.0) * (a + 2.0));
        sum += term;
    }
    return sum;
}

// ln of m in [sqrt(2)/2, sqrt(2)], by atanh's series, to about 1e-16.
double ln_mantissa(double m) {
    const double s = (m - 1.0) / (m + 1.0);
    const double s2 = s * s;
    double power = s;
    double sum = s;
    for (int n = 3; n < 60; n += 2) {
        power *= s2;
        sum += power / n;
    }
    return 2.0 * sum;
}

// 2^n for a whole n with 2^n a normal double.
double power_of_two(int n) { return double_of(static_cast<std::uint64_t>(n + 1023) << 52u); }

} // namespace

// Microsoft's compiler does not let a program define these (sinf is one of its
// intrinsics, and its callers reach exp2f straight through ucrtbase.dll), so
// an MSVC build keeps the C runtime's: setup.ps1 and the installer build with
// clang, and a battle refuses a copy built with the other compiler
// (d012_comm_battle.cpp).
#if !defined(_MSC_VER)
extern "C" {

float sinf(float x) {
    if (is_nan_or_inf(x)) return x - x;
    return static_cast<float>(sin_of(static_cast<double>(x), 0));
}

float cosf(float x) {
    if (is_nan_or_inf(x)) return x - x;
    return static_cast<float>(sin_of(static_cast<double>(x), 1));
}

float asinf(float x) {
    const double d = static_cast<double>(x);
    if (!(d >= -1.0 && d <= 1.0)) return (x - x) / (x - x);  // NaN, also for a NaN
    const double a = d < 0.0 ? -d : d;
    double r;
    if (a <= 0.5) {
        r = asin_small(a);
    } else {
        // asin(a) = pi/2 - 2 asin(sqrt((1 - a) / 2))
        double h = (1.0 - a) * 0.5;
        // A square root by Newton's method from a fixed start, so that it needs
        // nothing from the C runtime either; h is in [0, 0.25].
        double s = 0.0;
        if (h > 0.0) {
            s = h < 0.0625 ? 0.125 : 0.375;
            for (int i = 0; i < 60; ++i) s = 0.5 * (s + h / s);
        }
        r = kHalfPi - 2.0 * asin_small(s);
    }
    return static_cast<float>(d < 0.0 ? -r : r);
}

float log2f(float x) {
    const std::uint32_t b = bits_of(x);
    if ((b & 0x7FFFFFFFu) == 0u) return float_of(0xFF800000u);   // -inf
    if ((b & 0x80000000u) != 0u) return (x - x) / (x - x);         // NaN
    if ((b & 0x7F800000u) == 0x7F800000u) return x;                // +inf or NaN
    int exponent = static_cast<int>((b >> 23u) & 0xFFu);
    std::uint32_t mantissa = b & 0x007FFFFFu;
    if (exponent == 0) {
        // Subnormal: normalise.
        exponent = 1;
        while ((mantissa & 0x00800000u) == 0u) {
            mantissa <<= 1u;
            --exponent;
        }
        mantissa &= 0x007FFFFFu;
    }
    int e = exponent - 127;
    double m = 1.0 + static_cast<double>(mantissa) / 8388608.0;
    if (m > kSqrt2) {
        m *= 0.5;
        ++e;
    }
    return static_cast<float>(static_cast<double>(e) + ln_mantissa(m) / kLn2);
}

float exp2f(float x) {
    const std::uint32_t b = bits_of(x);
    if ((b & 0x7F800000u) == 0x7F800000u) {
        if ((b & 0x007FFFFFu) != 0u) return x;           // NaN
        return (b & 0x80000000u) != 0u ? 0.0f : x;       // 2^-inf, 2^inf
    }
    const double d = static_cast<double>(x);
    if (d >= 128.0) return float_of(0x7F800000u);
    if (d < -151.0) return 0.0f;
    const double n = nearest(d);
    const double f = (d - n) * kLn2;  // |f| <= ln(2)/2
    // e^f by its series
    double term = 1.0, sum = 1.0;
    for (int k = 1; k < 20; ++k) {
        term = term * f / k;
        sum += term;
    }
    return static_cast<float>(sum * power_of_two(static_cast<int>(n)));
}

} // extern "C"
#endif
