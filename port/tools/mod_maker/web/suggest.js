// Loop suggestions, run in a worker. Only suggests: the modder chooses.
//
// 1. Fingerprints: the file (mono, decimated to about 11 kHz) is cut into
//    frames; each frame's spectrum is summed into 32 log-spaced bands, logged,
//    centred on the file's mean and normalised.
// 2. Matches: for every lag L (a loop length), frame t is compared with frame
//    t - L. A real loop shows up as a long run of frames that match their
//    copy one loop earlier; musical repeats give shorter, weaker runs.
//    "any" ranks lags by how long and how well they match; "end" keeps the
//    modder's loop end and ranks lags by how well the seconds before it match
//    the seconds before end - L.
// 3. Refinement: the loop length is fixed to the sample by normalised
//    cross-correlation of the waveform (decimated, then at full rate), and
//    the end is moved, within a few hundred ms, to where the samples on both
//    sides of the seam match best.
// MIT licence; see port/README.md.
"use strict";

const BANDS = 32;
const FRAME = 2048;

function fft(re, im) {
  const n = re.length;
  for (let i = 1, j = 0; i < n; i++) {
    let bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) { let t = re[i]; re[i] = re[j]; re[j] = t; t = im[i]; im[i] = im[j]; im[j] = t; }
  }
  for (let len = 2; len <= n; len <<= 1) {
    const ang = -2 * Math.PI / len, wr = Math.cos(ang), wi = Math.sin(ang);
    for (let i = 0; i < n; i += len) {
      let cr = 1, ci = 0;
      for (let k = 0; k < len / 2; k++) {
        const a = i + k, b = a + len / 2;
        const xr = re[b] * cr - im[b] * ci, xi = re[b] * ci + im[b] * cr;
        re[b] = re[a] - xr; im[b] = im[a] - xi; re[a] += xr; im[a] += xi;
        const t = cr * wr - ci * wi; ci = cr * wi + ci * wr; cr = t;
      }
    }
  }
}

function decimate(x, d) {
  if (d <= 1) return x;
  const out = new Float32Array(Math.floor(x.length / d));
  for (let i = 0; i < out.length; i++) {
    let s = 0;
    for (let j = 0; j < d; j++) s += x[i * d + j];
    out[i] = s / d;
  }
  return out;
}

function fingerprints(y, ar, hop, progress) {
  const T = Math.max(0, Math.floor((y.length - FRAME) / hop) + 1);
  const F = new Float32Array(T * BANDS), rms = new Float32Array(T);
  const win = new Float32Array(FRAME);
  for (let i = 0; i < FRAME; i++) win[i] = 0.5 - 0.5 * Math.cos(2 * Math.PI * i / FRAME);
  const lo = 50, hi = Math.min(5000, ar / 2 * 0.95);
  const edges = [];
  for (let b = 0; b <= BANDS; b++) edges.push(Math.max(1, Math.round(lo * Math.pow(hi / lo, b / BANDS) / ar * FRAME)));
  const re = new Float64Array(FRAME), im = new Float64Array(FRAME);
  for (let t = 0; t < T; t++) {
    let e = 0;
    for (let i = 0; i < FRAME; i++) { const v = y[t * hop + i]; re[i] = v * win[i]; im[i] = 0; e += v * v; }
    rms[t] = Math.sqrt(e / FRAME);
    fft(re, im);
    for (let b = 0; b < BANDS; b++) {
      let s = 0;
      const k1 = Math.max(edges[b + 1], edges[b] + 1);
      for (let k = edges[b]; k < k1; k++) s += re[k] * re[k] + im[k] * im[k];
      F[t * BANDS + b] = Math.log(1e-9 + s / (k1 - edges[b]));
    }
    if (progress && t % 256 === 0) progress(0.3 * t / T);
  }
  // Centre on the file's mean; silent frames become zero (they match nothing).
  const mean = new Float64Array(BANDS);
  for (let t = 0; t < T; t++) for (let b = 0; b < BANDS; b++) mean[b] += F[t * BANDS + b] / T;
  let peak = 0;
  for (let t = 0; t < T; t++) peak = Math.max(peak, rms[t]);
  const N2 = new Float32Array(T);
  for (let t = 0; t < T; t++) {
    const silent = rms[t] < peak * 1e-3;
    let norm = 0;
    for (let b = 0; b < BANDS; b++) {
      const v = silent ? 0 : F[t * BANDS + b] - mean[b];
      F[t * BANDS + b] = v; norm += v * v;
    }
    N2[t] = norm;
  }
  return { F, T, rms, N2 };
}

// How alike two frames are, 1 for the same: 1 - |a-b|^2 / (|a|^2 + |b|^2).
// Unlike a cosine it also sees a change of level, so a fade-out stops matching.
function sim(F, N2, a, b) {
  const d = N2[a] + N2[b];
  if (d <= 0) return 0;
  let s = 0;
  const ia = a * BANDS, ib = b * BANDS;
  for (let k = 0; k < BANDS; k++) s += F[ia + k] * F[ib + k];
  return 2 * s / d;
}

// Normalised cross-correlation of x[e-N, e) with x[e-L-N, e-L), every `stride`th sample.
function ncc(x, e, L, N, stride) {
  let sab = 0, saa = 0, sbb = 0;
  const a0 = e - N, b0 = e - L - N;
  if (b0 < 0 || e > x.length) return -1;
  for (let i = 0; i < N; i += stride) { const a = x[a0 + i], b = x[b0 + i]; sab += a * b; saa += a * a; sbb += b * b; }
  return saa > 0 && sbb > 0 ? sab / Math.sqrt(saa * sbb) : -1;
}

function bestLag(x, e, center, radius, N, stride, step) {
  let best = center, score = -2;
  for (let L = Math.max(1, center - radius); L <= center + radius; L += step) {
    const s = ncc(x, e, L, Math.min(N, e - L), stride);
    if (s > score) { score = s; best = L; }
  }
  return [best, score];
}

// Where to put the end, near `e`, so the samples around the seam match best.
function placeSeam(m, e, L, radius) {
  const n = m.length, J = 48;
  let best = e, err = Infinity;
  for (let c = Math.max(L + J, e - radius); c <= Math.min(n, e + radius); c++) {
    let d = 0, cnt = 0;
    for (let j = -J; j < J; j++) {
      if (c + j >= n) break;
      const a = m[c + j], b = m[c - L + j];
      d += (a - b) * (a - b); cnt++;
    }
    d /= cnt;
    // A small preference for the original place, so equal matches stay put.
    d += 1e-9 * Math.abs(c - e);
    if (d < err) { err = d; best = c; }
  }
  return best;
}

function refine(m, y, d, rate, cand, fixedEnd) {
  const ar = rate / d;
  let e = Math.min(cand.end, m.length);
  // Coarse, on the decimated signal.
  const eD = Math.floor(e / d), LD = Math.round(cand.length / d);
  const N = Math.min(Math.round(1.5 * ar), eD - 1);
  const radius = Math.round(cand.radius / d);
  const [l1] = bestLag(y, eD, LD, radius, N, 3, 1);
  const [l2] = bestLag(y, eD, l1, 3, N, 1, 1);
  // Fine, at the file's rate.
  const Nf = Math.min(Math.round(0.35 * rate), e - 1);
  let [L, score] = bestLag(m, e, l2 * d, 2 * d + 2, Nf, 1, 1);
  if (!fixedEnd) {
    e = placeSeam(m, e, L, Math.round(0.25 * rate));
    [L, score] = bestLag(m, e, L, 3, Nf, 1, 1);
  }
  // Level of the audio before each side, in dB.
  let ea = 0, eb = 0;
  for (let i = 1; i <= Nf; i++) { if (e - L - i < 0) break; ea += m[e - i] * m[e - i]; eb += m[e - L - i] * m[e - L - i]; }
  const levelDb = ea > 0 && eb > 0 ? 10 * Math.log10(ea / eb) : 0;
  return { start: e - L, end: e, length: L, score, levelDb, coarse: cand.coarse };
}

function suggest(msg, progress) {
  const { mono, rate, mode } = msg;
  const n = mono.length;
  const d = Math.max(1, Math.round(rate / 11025));
  const y = decimate(mono, d), ar = rate / d;
  const hop = Math.max(1024, Math.ceil(y.length / 4000));
  const hopS = hop / ar;
  const { F, T, rms, N2 } = fingerprints(y, ar, hop, progress);
  if (T < 8) return { candidates: [], note: "the file is too short to search" };
  const minL = Math.max(2, Math.round((msg.minLoop || 8) / hopS));
  // The end of the music: the last frame above -50 dB of the loudest.
  let peakRms = 0;
  for (let t = 0; t < T; t++) peakRms = Math.max(peakRms, rms[t]);
  let lastLoud = T - 1;
  while (lastLoud > 0 && rms[lastLoud] < peakRms * 0.003) lastLoud--;
  let audioEnd = n;
  {
    const thr = peakRms * 0.003;
    let i = n - 1;
    while (i > 0 && Math.abs(mono[i]) < thr) i--;
    audioEnd = Math.min(n, i + 1 + Math.round(0.02 * rate));
  }
  const raw = [];
  if (mode === "end") {
    const e = Math.min(msg.end || n, n);
    const eF = Math.min(Math.floor((e / d - FRAME) / hop), T - 1);
    const W = Math.round(10 / hopS);
    for (let L = minL; L <= eF; L++) {
      let s = 0, c = 0;
      for (let t = Math.max(L, eF - W); t <= eF; t++) { s += sim(F, N2, t, t - L); c++; }
      if (c >= 4) raw.push({ L, score: s / c, end: e });
      if (L % 200 === 0) progress(0.3 + 0.5 * L / eF);
    }
    raw.sort((a, b) => b.score - a.score);
  } else {
    const smooth = Math.max(3, Math.round(2 / hopS));
    const theta = 0.7;
    const S = new Float32Array(T), D = new Float32Array(T), logRms = new Float32Array(T);
    for (let t = 0; t < T; t++) logRms[t] = Math.log(rms[t] + peakRms * 1e-3);
    const LEVEL = Math.log(Math.pow(10, 2 / 20));  // 2 dB
    for (let L = minL; L < T - smooth; L++) {
      for (let t = L; t < T; t++) { S[t] = sim(F, N2, t, t - L); D[t] = Math.abs(logRms[t] - logRms[t - L]); }
      // Box-smoothed runs above theta, at the same level (a fade is not a repeat).
      let sum = 0, dsum = 0, runStart = -1, runSum = 0, bestRun = null;
      for (let t = L; t < T; t++) {
        sum += S[t]; dsum += D[t];
        if (t - smooth >= L) { sum -= S[t - smooth]; dsum -= D[t - smooth]; }
        const count = Math.min(smooth, t - L + 1);
        const v = sum / count, level = dsum / count;
        const tc = t - Math.floor(count / 2);
        if (v >= theta && level < LEVEL && tc <= lastLoud) {
          if (runStart < 0) { runStart = tc; runSum = 0; }
          runSum += (v - theta) / (1 - theta);
        }
        if ((v < theta || level >= LEVEL || t === T - 1 || tc > lastLoud) && runStart >= 0) {
          const run = { a: runStart, b: tc, gain: runSum * hopS };
          if (!bestRun || run.gain > bestRun.gain) bestRun = run;
          runStart = -1;
        }
      }
      if (bestRun && bestRun.gain > 0.5) {
        // The end: inside the run, a second before it stops matching, or the end of the music.
        const reachesEnd = bestRun.b >= lastLoud - smooth;
        const endFrame = reachesEnd ? null : Math.max(bestRun.a + 1, bestRun.b - Math.round(1 / hopS));
        const end = reachesEnd ? audioEnd : Math.round((endFrame * hop + FRAME / 2) * d);
        raw.push({ L, score: bestRun.gain, end, run: (bestRun.b - bestRun.a) * hopS });
      }
      if (L % 100 === 0) progress(0.3 + 0.5 * L / T);
    }
    raw.sort((a, b) => b.score - a.score);
  }
  // One candidate per family of lengths.
  const picked = [];
  for (const r of raw) {
    if (picked.length >= 6) break;
    if (picked.some((p) => Math.abs(p.L - r.L) <= 3)) continue;
    picked.push(r);
  }
  const m = mono;
  const out = [];
  picked.forEach((p, i) => {
    progress(0.8 + 0.2 * i / picked.length);
    const cand = { end: p.end, length: p.L * hop * d, radius: 2 * hop * d, coarse: p.score };
    if (cand.end - cand.length < 0) return;
    const r = refine(m, y, d, rate, cand, mode === "end");
    if (r.start >= 0 && r.end > r.start + 1) out.push(r);
  });
  // Best match first (a difference of level counts against it); near-duplicates
  // (same start and end within 10 ms) once.
  const rank = (c) => c.score - 0.02 * Math.max(0, Math.abs(c.levelDb) - 1);
  out.sort((a, b) => rank(b) - rank(a));
  const unique = [];
  for (const c of out) {
    if (unique.some((u) => Math.abs(u.start - c.start) < rate * 0.01 && Math.abs(u.end - c.end) < rate * 0.01)) continue;
    unique.push(c);
  }
  return { candidates: unique.slice(0, 5), hopSeconds: hopS };
}

if (typeof self !== "undefined" && typeof window === "undefined") {
  self.onmessage = (event) => {
    const started = performance.now();
    try {
      const result = suggest(event.data, (f) => self.postMessage({ progress: f }));
      result.ms = Math.round(performance.now() - started);
      self.postMessage({ done: true, result });
    } catch (error) {
      self.postMessage({ done: true, error: String(error && error.stack || error) });
    }
  };
}
