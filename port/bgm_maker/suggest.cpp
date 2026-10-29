// Loop suggestions; see suggest.hpp. A line-for-line port of
// port/tools/bgm_maker/web/suggest.js: where it keeps a value in a
// Float32Array, so does this (float), and every other value is a double, as a
// JavaScript number is; js_round() is Math.round. MIT licence; see
// port/README.md.

#include "suggest.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>
#include <optional>

namespace d012::bgm_maker {
namespace {

constexpr int kBands = 32;
constexpr int kFrame = 2048;
constexpr double kPi = 3.14159265358979323846;

// JavaScript's Math.round: halves go up.
double js_round(double x) { return std::floor(x + 0.5); }
std::int64_t round_int(double x) { return static_cast<std::int64_t>(js_round(x)); }

void fft(std::vector<double> &re, std::vector<double> &im) {
    const std::size_t n = re.size();
    for (std::size_t i = 1, j = 0; i < n; ++i) {
        std::size_t bit = n >> 1u;
        for (; (j & bit) != 0u; bit >>= 1u) j ^= bit;
        j ^= bit;
        if (i < j) {
            std::swap(re[i], re[j]);
            std::swap(im[i], im[j]);
        }
    }
    for (std::size_t len = 2; len <= n; len <<= 1u) {
        const double ang = -2.0 * kPi / static_cast<double>(len);
        const double wr = std::cos(ang);
        const double wi = std::sin(ang);
        for (std::size_t i = 0; i < n; i += len) {
            double cr = 1.0;
            double ci = 0.0;
            for (std::size_t k = 0; k < len / 2u; ++k) {
                const std::size_t a = i + k;
                const std::size_t b = a + len / 2u;
                const double xr = re[b] * cr - im[b] * ci;
                const double xi = re[b] * ci + im[b] * cr;
                re[b] = re[a] - xr;
                im[b] = im[a] - xi;
                re[a] += xr;
                im[a] += xi;
                const double t = cr * wr - ci * wi;
                ci = cr * wi + ci * wr;
                cr = t;
            }
        }
    }
}

std::vector<float> decimate(const std::vector<float> &x, int d) {
    if (d <= 1) return x;
    std::vector<float> out(x.size() / static_cast<std::size_t>(d));
    for (std::size_t i = 0; i < out.size(); ++i) {
        double s = 0.0;
        for (int j = 0; j < d; ++j) s += x[i * static_cast<std::size_t>(d) + static_cast<std::size_t>(j)];
        out[i] = static_cast<float>(s / d);
    }
    return out;
}

struct Prints {
    std::vector<float> f;    // t * kBands + b
    std::int64_t frames{};
    std::vector<float> rms;
    std::vector<float> n2;
};

Prints fingerprints(const std::vector<float> &y, double ar, std::int64_t hop, const std::function<void(double)> &progress,
                    const std::atomic<bool> *cancel) {
    Prints p;
    const std::int64_t length = static_cast<std::int64_t>(y.size());
    p.frames = std::max<std::int64_t>(0, static_cast<std::int64_t>(std::floor(static_cast<double>(length - kFrame) / hop)) + 1);
    const std::int64_t T = p.frames;
    p.f.assign(static_cast<std::size_t>(T * kBands), 0.0f);
    p.rms.assign(static_cast<std::size_t>(T), 0.0f);
    std::vector<float> win(kFrame);
    for (int i = 0; i < kFrame; ++i) win[i] = static_cast<float>(0.5 - 0.5 * std::cos(2.0 * kPi * i / kFrame));
    const double lo = 50.0;
    const double hi = std::min(5000.0, ar / 2.0 * 0.95);
    std::vector<std::int64_t> edges;
    for (int b = 0; b <= kBands; ++b)
        edges.push_back(std::max<std::int64_t>(1, round_int(lo * std::pow(hi / lo, static_cast<double>(b) / kBands) / ar * kFrame)));
    std::vector<double> re(kFrame);
    std::vector<double> im(kFrame);
    for (std::int64_t t = 0; t < T; ++t) {
        if (cancel != nullptr && cancel->load()) return p;
        double e = 0.0;
        for (int i = 0; i < kFrame; ++i) {
            const double v = y[static_cast<std::size_t>(t * hop + i)];
            re[i] = v * win[i];
            im[i] = 0.0;
            e += v * v;
        }
        p.rms[static_cast<std::size_t>(t)] = static_cast<float>(std::sqrt(e / kFrame));
        fft(re, im);
        for (int b = 0; b < kBands; ++b) {
            double s = 0.0;
            const std::int64_t k1 = std::max(edges[b + 1], edges[b] + 1);
            for (std::int64_t k = edges[b]; k < k1; ++k)
                s += re[static_cast<std::size_t>(k)] * re[static_cast<std::size_t>(k)] +
                     im[static_cast<std::size_t>(k)] * im[static_cast<std::size_t>(k)];
            p.f[static_cast<std::size_t>(t * kBands + b)] =
                static_cast<float>(std::log(1e-9 + s / static_cast<double>(k1 - edges[b])));
        }
        if (t % 256 == 0) progress(0.3 * static_cast<double>(t) / static_cast<double>(T));
    }
    // Centred on the file's mean; silent frames become zero (they match nothing).
    std::vector<double> mean(kBands, 0.0);
    for (std::int64_t t = 0; t < T; ++t)
        for (int b = 0; b < kBands; ++b) mean[b] += p.f[static_cast<std::size_t>(t * kBands + b)] / static_cast<double>(T);
    double peak = 0.0;
    for (std::int64_t t = 0; t < T; ++t) peak = std::max(peak, static_cast<double>(p.rms[static_cast<std::size_t>(t)]));
    p.n2.assign(static_cast<std::size_t>(T), 0.0f);
    for (std::int64_t t = 0; t < T; ++t) {
        const bool silent = p.rms[static_cast<std::size_t>(t)] < peak * 1e-3;
        double norm = 0.0;
        for (int b = 0; b < kBands; ++b) {
            const std::size_t at = static_cast<std::size_t>(t * kBands + b);
            const double v = silent ? 0.0 : p.f[at] - mean[b];
            p.f[at] = static_cast<float>(v);
            norm += v * v;
        }
        p.n2[static_cast<std::size_t>(t)] = static_cast<float>(norm);
    }
    return p;
}

// How alike two frames are, 1 for the same: 1 - |a-b|^2 / (|a|^2 + |b|^2).
// Unlike a cosine it also sees a change of level, so a fade-out stops matching.
double sim(const Prints &p, std::int64_t a, std::int64_t b) {
    const double d = static_cast<double>(p.n2[static_cast<std::size_t>(a)]) + p.n2[static_cast<std::size_t>(b)];
    if (d <= 0.0) return 0.0;
    double s = 0.0;
    const float *fa = &p.f[static_cast<std::size_t>(a * kBands)];
    const float *fb = &p.f[static_cast<std::size_t>(b * kBands)];
    for (int k = 0; k < kBands; ++k) s += static_cast<double>(fa[k]) * fb[k];
    return 2.0 * s / d;
}

// Normalised cross-correlation of x[e-N, e) with x[e-L-N, e-L), every `stride`th sample.
double ncc(const std::vector<float> &x, std::int64_t e, std::int64_t L, std::int64_t N, std::int64_t stride) {
    double sab = 0.0, saa = 0.0, sbb = 0.0;
    const std::int64_t a0 = e - N;
    const std::int64_t b0 = e - L - N;
    if (b0 < 0 || e > static_cast<std::int64_t>(x.size())) return -1.0;
    for (std::int64_t i = 0; i < N; i += stride) {
        const double a = x[static_cast<std::size_t>(a0 + i)];
        const double b = x[static_cast<std::size_t>(b0 + i)];
        sab += a * b;
        saa += a * a;
        sbb += b * b;
    }
    return saa > 0.0 && sbb > 0.0 ? sab / std::sqrt(saa * sbb) : -1.0;
}

std::pair<std::int64_t, double> best_lag(const std::vector<float> &x, std::int64_t e, std::int64_t center, std::int64_t radius,
                                         std::int64_t N, std::int64_t stride, std::int64_t step) {
    std::int64_t best = center;
    double score = -2.0;
    for (std::int64_t L = std::max<std::int64_t>(1, center - radius); L <= center + radius; L += step) {
        const double s = ncc(x, e, L, std::min(N, e - L), stride);
        if (s > score) {
            score = s;
            best = L;
        }
    }
    return {best, score};
}

// Where to put the end, near `e`, so the samples around the seam match best.
std::int64_t place_seam(const std::vector<float> &m, std::int64_t e, std::int64_t L, std::int64_t radius) {
    const std::int64_t n = static_cast<std::int64_t>(m.size());
    constexpr std::int64_t J = 48;
    std::int64_t best = e;
    double err = std::numeric_limits<double>::infinity();
    for (std::int64_t c = std::max(L + J, e - radius); c <= std::min(n, e + radius); ++c) {
        double d = 0.0;
        int count = 0;
        for (std::int64_t j = -J; j < J; ++j) {
            if (c + j >= n) break;
            const double a = m[static_cast<std::size_t>(c + j)];
            const double b = m[static_cast<std::size_t>(c - L + j)];
            d += (a - b) * (a - b);
            ++count;
        }
        d /= count;
        // A small preference for the original place, so equal matches stay put.
        d += 1e-9 * static_cast<double>(std::abs(c - e));
        if (d < err) {
            err = d;
            best = c;
        }
    }
    return best;
}

struct Coarse {
    std::int64_t end{};
    std::int64_t length{};
    std::int64_t radius{};
    double coarse{};
};

LoopCandidate refine(const std::vector<float> &m, const std::vector<float> &y, int d, int rate, const Coarse &cand,
                     bool fixed_end) {
    const double ar = static_cast<double>(rate) / d;
    std::int64_t e = std::min(cand.end, static_cast<std::int64_t>(m.size()));
    // Coarse, on the decimated signal.
    const std::int64_t eD = static_cast<std::int64_t>(std::floor(static_cast<double>(e) / d));
    const std::int64_t LD = round_int(static_cast<double>(cand.length) / d);
    const std::int64_t N = std::min(round_int(1.5 * ar), eD - 1);
    const std::int64_t radius = round_int(static_cast<double>(cand.radius) / d);
    const auto [l1, s1] = best_lag(y, eD, LD, radius, N, 3, 1);
    const auto [l2, s2] = best_lag(y, eD, l1, 3, N, 1, 1);
    // Fine, at the file's rate.
    const std::int64_t Nf = std::min(round_int(0.35 * rate), e - 1);
    auto [L, score] = best_lag(m, e, l2 * d, 2 * d + 2, Nf, 1, 1);
    if (!fixed_end) {
        e = place_seam(m, e, L, round_int(0.25 * rate));
        std::tie(L, score) = best_lag(m, e, L, 3, Nf, 1, 1);
    }
    // The level of the audio before each side, in dB.
    double ea = 0.0, eb = 0.0;
    for (std::int64_t i = 1; i <= Nf; ++i) {
        if (e - L - i < 0) break;
        const double a = m[static_cast<std::size_t>(e - i)];
        const double b = m[static_cast<std::size_t>(e - L - i)];
        ea += a * a;
        eb += b * b;
    }
    const double level_db = ea > 0.0 && eb > 0.0 ? 10.0 * std::log10(ea / eb) : 0.0;
    return {e - L, e, L, score, level_db, cand.coarse};
}

struct Raw {
    std::int64_t lag{};
    double score{};
    std::int64_t end{};
};

} // namespace

std::vector<float> mono_signal(const DecodedTrack &track) {
    const DecodedAudio &audio = track.audio;
    std::vector<float> mono(static_cast<std::size_t>(audio.frames));
    for (std::int64_t i = 0; i < audio.frames; ++i) {
        double s = 0.0;
        for (int c = 0; c < audio.channels; ++c) s += static_cast<float>(track.sample(c, i) / 32768.0);
        mono[static_cast<std::size_t>(i)] = static_cast<float>(s / audio.channels);
    }
    return mono;
}

SuggestResult suggest_loops(const std::vector<float> &mono, int rate, std::int64_t end, double min_loop_seconds,
                            const std::function<void(double)> &progress, const std::atomic<bool> *cancel) {
    SuggestResult result;
    const bool end_mode = end >= 0;
    const std::int64_t n = static_cast<std::int64_t>(mono.size());
    const int d = static_cast<int>(std::max<std::int64_t>(1, round_int(rate / 11025.0)));
    const std::vector<float> y = decimate(mono, d);
    const double ar = static_cast<double>(rate) / d;
    const std::int64_t hop = std::max<std::int64_t>(1024, static_cast<std::int64_t>(std::ceil(y.size() / 4000.0)));
    const double hop_s = static_cast<double>(hop) / ar;
    result.hop_seconds = hop_s;
    const Prints prints = fingerprints(y, ar, hop, progress, cancel);
    const std::int64_t T = prints.frames;
    if (cancel != nullptr && cancel->load()) return result;
    if (T < 8) {
        result.note = "the file is too short to search";
        return result;
    }
    const std::int64_t min_lag = std::max<std::int64_t>(2, round_int((min_loop_seconds > 0.0 ? min_loop_seconds : 8.0) / hop_s));
    // The end of the music: the last frame above -50 dB of the loudest.
    double peak_rms = 0.0;
    for (std::int64_t t = 0; t < T; ++t) peak_rms = std::max(peak_rms, static_cast<double>(prints.rms[static_cast<std::size_t>(t)]));
    std::int64_t last_loud = T - 1;
    while (last_loud > 0 && prints.rms[static_cast<std::size_t>(last_loud)] < peak_rms * 0.003) --last_loud;
    std::int64_t audio_end = n;
    {
        const double threshold = peak_rms * 0.003;
        std::int64_t i = n - 1;
        while (i > 0 && std::fabs(mono[static_cast<std::size_t>(i)]) < threshold) --i;
        audio_end = std::min(n, i + 1 + round_int(0.02 * rate));
    }
    std::vector<Raw> raw;
    if (end_mode) {
        const std::int64_t e = std::min(end > 0 ? end : n, n);
        const std::int64_t eF =
            std::min(static_cast<std::int64_t>(std::floor((static_cast<double>(e) / d - kFrame) / hop)), T - 1);
        const std::int64_t W = round_int(10.0 / hop_s);
        for (std::int64_t L = min_lag; L <= eF; ++L) {
            if (cancel != nullptr && cancel->load()) return result;
            double s = 0.0;
            int c = 0;
            for (std::int64_t t = std::max(L, eF - W); t <= eF; ++t) {
                s += sim(prints, t, t - L);
                ++c;
            }
            if (c >= 4) raw.push_back({L, s / c, e});
            if (L % 200 == 0) progress(0.3 + 0.5 * static_cast<double>(L) / static_cast<double>(eF));
        }
    } else {
        const std::int64_t smooth = std::max<std::int64_t>(3, round_int(2.0 / hop_s));
        const double theta = 0.7;
        std::vector<float> S(static_cast<std::size_t>(T)), D(static_cast<std::size_t>(T)), log_rms(static_cast<std::size_t>(T));
        for (std::int64_t t = 0; t < T; ++t)
            log_rms[static_cast<std::size_t>(t)] =
                static_cast<float>(std::log(static_cast<double>(prints.rms[static_cast<std::size_t>(t)]) + peak_rms * 1e-3));
        const double level_limit = std::log(std::pow(10.0, 2.0 / 20.0));  // 2 dB
        for (std::int64_t L = min_lag; L < T - smooth; ++L) {
            if (cancel != nullptr && cancel->load()) return result;
            for (std::int64_t t = L; t < T; ++t) {
                S[static_cast<std::size_t>(t)] = static_cast<float>(sim(prints, t, t - L));
                D[static_cast<std::size_t>(t)] = static_cast<float>(
                    std::fabs(static_cast<double>(log_rms[static_cast<std::size_t>(t)]) - log_rms[static_cast<std::size_t>(t - L)]));
            }
            // Box-smoothed runs above theta, at the same level (a fade is not a repeat).
            double sum = 0.0, dsum = 0.0, run_sum = 0.0;
            std::int64_t run_start = -1;
            struct Run {
                std::int64_t a, b;
                double gain;
            };
            std::optional<Run> best_run;
            for (std::int64_t t = L; t < T; ++t) {
                sum += S[static_cast<std::size_t>(t)];
                dsum += D[static_cast<std::size_t>(t)];
                if (t - smooth >= L) {
                    sum -= S[static_cast<std::size_t>(t - smooth)];
                    dsum -= D[static_cast<std::size_t>(t - smooth)];
                }
                const std::int64_t count = std::min(smooth, t - L + 1);
                const double v = sum / static_cast<double>(count);
                const double level = dsum / static_cast<double>(count);
                const std::int64_t tc = t - count / 2;
                if (v >= theta && level < level_limit && tc <= last_loud) {
                    if (run_start < 0) {
                        run_start = tc;
                        run_sum = 0.0;
                    }
                    run_sum += (v - theta) / (1.0 - theta);
                }
                if ((v < theta || level >= level_limit || t == T - 1 || tc > last_loud) && run_start >= 0) {
                    const Run run{run_start, tc, run_sum * hop_s};
                    if (!best_run || run.gain > best_run->gain) best_run = run;
                    run_start = -1;
                }
            }
            if (best_run && best_run->gain > 0.5) {
                // The end: inside the run, a second before it stops matching, or the end of the music.
                const bool reaches_end = best_run->b >= last_loud - smooth;
                std::int64_t at = audio_end;
                if (!reaches_end) {
                    const std::int64_t end_frame = std::max(best_run->a + 1, best_run->b - round_int(1.0 / hop_s));
                    at = round_int(static_cast<double>(end_frame * hop + kFrame / 2) * d);
                }
                raw.push_back({L, best_run->gain, at});
            }
            if (L % 100 == 0) progress(0.3 + 0.5 * static_cast<double>(L) / static_cast<double>(T));
        }
    }
    std::stable_sort(raw.begin(), raw.end(), [](const Raw &a, const Raw &b) { return a.score > b.score; });
    // One candidate per family of lengths.
    std::vector<Raw> picked;
    for (const Raw &r : raw) {
        if (picked.size() >= 6u) break;
        if (std::any_of(picked.begin(), picked.end(), [&](const Raw &p) { return std::abs(p.lag - r.lag) <= 3; })) continue;
        picked.push_back(r);
    }
    std::vector<LoopCandidate> out;
    for (std::size_t i = 0; i < picked.size(); ++i) {
        if (cancel != nullptr && cancel->load()) return result;
        progress(0.8 + 0.2 * static_cast<double>(i) / static_cast<double>(picked.size()));
        const Raw &p = picked[i];
        const Coarse cand{p.end, p.lag * hop * d, 2 * hop * d, p.score};
        if (cand.end - cand.length < 0) continue;
        const LoopCandidate r = refine(mono, y, d, rate, cand, end_mode);
        if (r.start >= 0 && r.end > r.start + 1) out.push_back(r);
    }
    // Best match first (a difference of level counts against it); near
    // duplicates (the same start and end within 10 ms) once.
    const auto rank = [](const LoopCandidate &c) { return c.score - 0.02 * std::max(0.0, std::fabs(c.level_db) - 1.0); };
    std::stable_sort(out.begin(), out.end(), [&](const LoopCandidate &a, const LoopCandidate &b) { return rank(a) > rank(b); });
    for (const LoopCandidate &c : out) {
        const bool near = std::any_of(result.candidates.begin(), result.candidates.end(), [&](const LoopCandidate &u) {
            return std::fabs(static_cast<double>(u.start - c.start)) < rate * 0.01 &&
                   std::fabs(static_cast<double>(u.end - c.end)) < rate * 0.01;
        });
        if (!near) result.candidates.push_back(c);
    }
    if (result.candidates.size() > 5u) result.candidates.resize(5u);
    return result;
}

Suggester::~Suggester() { cancel(); }

void Suggester::cancel() {
    cancel_ = true;
    if (worker_.joinable()) worker_.join();
    cancel_ = false;
    busy_ = false;
}

void Suggester::start(std::shared_ptr<const DecodedTrack> track, std::int64_t end, double min_loop_seconds) {
    cancel();
    {
        const std::lock_guard lock(mutex_);
        done_.reset();
    }
    busy_ = true;
    progress_ = 0.0;
    worker_ = std::thread([this, track = std::move(track), end, min_loop_seconds] {
        const auto started = std::chrono::steady_clock::now();
        const std::vector<float> mono = mono_signal(*track);
        SuggestResult result = suggest_loops(mono, track->audio.rate, end, min_loop_seconds,
                                             [this](double f) { progress_ = f; }, &cancel_);
        if (!cancel_.load()) {
            const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count();
            const std::lock_guard lock(mutex_);
            done_ = Done{track->file, end >= 0, std::move(result), seconds};
        }
        busy_ = false;
    });
}

std::optional<Suggester::Done> Suggester::take() {
    const std::lock_guard lock(mutex_);
    auto done = std::move(done_);
    done_.reset();
    return done;
}

} // namespace d012::bgm_maker
