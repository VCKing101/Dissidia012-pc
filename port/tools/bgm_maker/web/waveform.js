// Waveform and seam views for the Battle BGM Maker, drawn on canvases.
// MIT licence; see port/README.md.
"use strict";

const WAVE_COLORS = {
  bg: "#0d0f12", grid: "#1c2028", ruler: "#14171c", rulerText: "#7d8594",
  wave: "#5fa8d3", waveDim: "#2f5068", region: "rgba(76,208,125,0.07)", outside: "rgba(0,0,0,0.35)",
  start: "#4cd07d", end: "#ff6b6b", playhead: "#ffffff", cursor: "#e0a84f", center: "#262b34",
  ghost: "rgba(220,220,220,0.28)", ch1: "#5fa8d3", ch2: "#d39a5f",
};
const RULER = 20;

function formatTime(samples, rate) {
  if (!rate) return "-";
  const sign = samples < 0 ? "-" : "";
  let ms = Math.round(Math.abs(samples) * 1000 / rate);
  const h = Math.floor(ms / 3600000); ms -= h * 3600000;
  const m = Math.floor(ms / 60000); ms -= m * 60000;
  const s = Math.floor(ms / 1000); ms -= s * 1000;
  const tail = String(s).padStart(2, "0") + "." + String(ms).padStart(3, "0");
  return sign + (h ? h + ":" + String(m).padStart(2, "0") + ":" + tail : m + ":" + tail);
}

class Peaks {
  // Min/max of each channel in blocks of `block` samples.
  constructor(channels, block, source) {
    this.block = block;
    this.levels = channels.map((data, c) => {
      const count = Math.ceil((source ? source.levels[c].length / 2 * source.block : data.length) / block);
      const out = new Float32Array(count * 2);
      if (source) {
        const ratio = block / source.block, src = source.levels[c];
        for (let i = 0; i < count; i++) {
          let lo = 1, hi = -1;
          const a = i * ratio, b = Math.min(a + ratio, src.length / 2);
          for (let j = a; j < b; j++) { if (src[2 * j] < lo) lo = src[2 * j]; if (src[2 * j + 1] > hi) hi = src[2 * j + 1]; }
          out[2 * i] = lo; out[2 * i + 1] = hi;
        }
      } else {
        for (let i = 0; i < count; i++) {
          let lo = 1, hi = -1;
          const a = i * block, b = Math.min(a + block, data.length);
          for (let j = a; j < b; j++) { const v = data[j]; if (v < lo) lo = v; if (v > hi) hi = v; }
          out[2 * i] = lo; out[2 * i + 1] = hi;
        }
      }
      return out;
    });
  }
}

// Min and max of channel `c` over samples [a, b).
function rangeOf(channels, pyramid, c, a, b) {
  const data = channels[c];
  a = Math.max(0, Math.floor(a)); b = Math.min(data.length, Math.ceil(b));
  if (b <= a) return null;
  let lo = 1, hi = -1;
  let level = null;
  for (const p of pyramid) if (b - a >= p.block * 4) level = p;
  if (!level) {
    for (let j = a; j < b; j++) { const v = data[j]; if (v < lo) lo = v; if (v > hi) hi = v; }
    return [lo, hi];
  }
  const blk = level.block, arr = level.levels[c];
  const first = Math.ceil(a / blk), last = Math.floor(b / blk);
  for (let j = a; j < Math.min(b, first * blk); j++) { const v = data[j]; if (v < lo) lo = v; if (v > hi) hi = v; }
  for (let i = first; i < last; i++) { if (arr[2 * i] < lo) lo = arr[2 * i]; if (arr[2 * i + 1] > hi) hi = arr[2 * i + 1]; }
  for (let j = Math.max(a, last * blk); j < b; j++) { const v = data[j]; if (v < lo) lo = v; if (v > hi) hi = v; }
  return [lo, hi];
}

function niceStep(seconds) {
  const steps = [1e-4, 2e-4, 5e-4, 1e-3, 2e-3, 5e-3, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1, 2, 5, 10, 15, 30, 60, 120, 300, 600];
  for (const s of steps) if (s >= seconds) return s;
  return 1200;
}

class WaveView {
  constructor(canvas, overview, handlers) {
    this.canvas = canvas; this.overview = overview; this.h = handlers || {};
    this.channels = null; this.rate = 0; this.n = 0; this.pyramid = [];
    this.start = 0; this.spp = 1;
    this.loop = { mode: "none", start: 0, end: 0 };
    this.cursor = 0; this.playhead = null; this.active = "start";
    this.drag = null; this.pending = false; this.overviewImage = null;
    this._bind();
    new ResizeObserver(() => { this.overviewImage = null; this.invalidate(); }).observe(canvas);
  }

  setAudio(channels, rate) {
    this.channels = channels; this.rate = rate; this.n = channels[0].length;
    const p1 = new Peaks(channels, 256), p2 = new Peaks(channels, 8192, p1);
    this.pyramid = [p1, p2];
    this.overviewImage = null; this.cursor = 0; this.playhead = null;
    this.zoomFit();
  }

  clear() { this.channels = null; this.n = 0; this.invalidate(); }

  setLoop(loop) { this.loop = Object.assign({}, loop); this.invalidate(); }
  setCursor(s) { this.cursor = s; this.invalidate(); }
  setPlayhead(s) { this.playhead = s; this.invalidate(); }
  setActive(which) { this.active = which; this.invalidate(); }

  width() { return this.canvas.clientWidth || 1; }
  clampView() {
    const w = this.width();
    const maxSpp = Math.max(this.n / w, 1 / 32);
    this.spp = Math.min(Math.max(this.spp, 1 / 32), maxSpp);
    const span = this.spp * w;
    this.start = Math.min(Math.max(this.start, 0), Math.max(0, this.n - span));
  }
  zoomFit() { this.start = 0; this.spp = this.n / this.width(); this.clampView(); this.invalidate(); }
  zoomTo(a, b) { this.spp = Math.max(b - a, 32) / this.width(); this.start = a; this.clampView(); this.invalidate(); }
  zoomAround(center, span) { this.zoomTo(center - span / 2, center + span / 2); }
  zoomBy(factor, anchor) {
    if (anchor === undefined) anchor = this.playhead !== null ? this.playhead : this.cursor;
    const x = (anchor - this.start) / this.spp;
    const inView = x >= 0 && x <= this.width();
    const ax = inView ? x : this.width() / 2;
    const as = inView ? anchor : this.start + this.spp * ax;
    this.spp *= factor; this.clampView();
    this.start = as - ax * this.spp; this.clampView(); this.invalidate();
  }
  ensureVisible(s) {
    const span = this.spp * this.width();
    if (s < this.start || s > this.start + span) { this.start = s - span * 0.1; this.clampView(); this.invalidate(); }
  }
  xOf(s) { return (s - this.start) / this.spp; }
  sampleAt(x) { return Math.round(Math.min(Math.max(this.start + x * this.spp, 0), this.n)); }

  invalidate() {
    if (this.pending) return;
    this.pending = true;
    requestAnimationFrame(() => { this.pending = false; this.draw(); this.drawOverview(); });
  }

  _size(canvas) {
    const dpr = window.devicePixelRatio || 1;
    const w = Math.max(1, Math.round(canvas.clientWidth * dpr)), h = Math.max(1, Math.round(canvas.clientHeight * dpr));
    if (canvas.width !== w || canvas.height !== h) { canvas.width = w; canvas.height = h; }
    const ctx = canvas.getContext("2d");
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    return [ctx, canvas.clientWidth, canvas.clientHeight];
  }

  hasMarkers() { return this.loop.mode === "custom"; }
  region() {
    if (this.loop.mode === "custom" || this.loop.mode === "tags") return [this.loop.start, this.loop.end];
    if (this.loop.mode === "whole") return [0, this.n];
    return null;
  }

  draw() {
    const [ctx, W, H] = this._size(this.canvas);
    ctx.fillStyle = WAVE_COLORS.bg; ctx.fillRect(0, 0, W, H);
    if (!this.channels) return;
    this.clampView();
    const nch = this.channels.length, laneH = (H - RULER) / nch;
    // Region of the loop, and darker outside it.
    const region = this.region();
    if (region) {
      const x0 = this.xOf(region[0]), x1 = this.xOf(region[1]);
      ctx.fillStyle = WAVE_COLORS.outside;
      if (x0 > 0) ctx.fillRect(0, RULER, Math.min(x0, W), H);
      if (x1 < W) ctx.fillRect(Math.max(x1, 0), RULER, W, H);
      ctx.fillStyle = WAVE_COLORS.region;
      ctx.fillRect(Math.max(x0, 0), RULER, Math.min(x1, W) - Math.max(x0, 0), H);
    }
    // Ruler.
    ctx.fillStyle = WAVE_COLORS.ruler; ctx.fillRect(0, 0, W, RULER);
    const step = niceStep(80 * this.spp / this.rate);
    const t0 = Math.floor(this.start / this.rate / step) * step;
    ctx.font = "11px Consolas, monospace"; ctx.textBaseline = "middle";
    for (let t = t0; ; t += step) {
      const x = this.xOf(t * this.rate);
      if (x > W) break;
      if (x < -60) continue;
      ctx.fillStyle = WAVE_COLORS.grid; ctx.fillRect(Math.round(x), RULER, 1, H - RULER);
      ctx.fillStyle = WAVE_COLORS.rulerText; ctx.fillRect(Math.round(x), RULER - 5, 1, 5);
      const label = step < 0.001 ? String(Math.round(t * this.rate)) : formatTime(Math.round(t * this.rate), this.rate);
      ctx.fillText(label, x + 3, RULER / 2);
    }
    // Lanes.
    for (let c = 0; c < nch; c++) {
      const top = RULER + c * laneH, mid = top + laneH / 2, amp = laneH / 2 - 3;
      ctx.fillStyle = WAVE_COLORS.center; ctx.fillRect(0, Math.round(mid), W, 1);
      if (c > 0) { ctx.fillStyle = WAVE_COLORS.grid; ctx.fillRect(0, Math.round(top), W, 1); }
      const data = this.channels[c];
      if (this.spp >= 1) {
        ctx.fillStyle = WAVE_COLORS.wave;
        for (let x = 0; x < W; x++) {
          const a = this.start + x * this.spp, r = rangeOf(this.channels, this.pyramid, c, a, a + this.spp);
          if (!r) continue;
          const y0 = mid - r[1] * amp, y1 = mid - r[0] * amp;
          ctx.fillRect(x, y0, 1, Math.max(1, y1 - y0));
        }
      } else {
        const a = Math.max(0, Math.floor(this.start) - 1), b = Math.min(this.n, Math.ceil(this.start + W * this.spp) + 2);
        ctx.strokeStyle = WAVE_COLORS.wave; ctx.lineWidth = 1.25; ctx.beginPath();
        for (let i = a; i < b; i++) {
          const x = this.xOf(i), y = mid - data[i] * amp;
          if (i === a) ctx.moveTo(x, y); else ctx.lineTo(x, y);
        }
        ctx.stroke();
        if (1 / this.spp >= 6) {
          ctx.fillStyle = WAVE_COLORS.wave;
          for (let i = a; i < b; i++) ctx.fillRect(this.xOf(i) - 1.5, mid - data[i] * amp - 1.5, 3, 3);
        }
      }
    }
    // Markers. A marker sits at the left edge of its sample.
    const marker = (s, color, label, active) => {
      const x = Math.round(this.xOf(s)) + 0.5;
      if (x < -20 || x > W + 20) return;
      ctx.strokeStyle = color; ctx.lineWidth = active ? 2 : 1.25;
      ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, H); ctx.stroke();
      ctx.fillStyle = color;
      const fw = 16;
      if (label === "S") ctx.fillRect(x, 0, fw, RULER - 2); else ctx.fillRect(x - fw, 0, fw, RULER - 2);
      ctx.fillStyle = "#0d0f12"; ctx.font = "bold 11px Segoe UI, sans-serif";
      ctx.fillText(label, label === "S" ? x + 4 : x - fw + 4, RULER / 2);
    };
    if (region && this.loop.mode !== "whole") {
      const editable = this.hasMarkers();
      marker(region[0], editable ? WAVE_COLORS.start : "#3c7a55", "S", editable && this.active === "start");
      marker(region[1], editable ? WAVE_COLORS.end : "#8a4545", "E", editable && this.active === "end");
    }
    // Cursor and playhead.
    const cx = Math.round(this.xOf(this.cursor)) + 0.5;
    if (cx >= 0 && cx <= W) {
      ctx.strokeStyle = WAVE_COLORS.cursor; ctx.setLineDash([4, 3]); ctx.lineWidth = 1;
      ctx.beginPath(); ctx.moveTo(cx, RULER); ctx.lineTo(cx, H); ctx.stroke(); ctx.setLineDash([]);
    }
    if (this.playhead !== null) {
      const px = Math.round(this.xOf(this.playhead)) + 0.5;
      if (px >= 0 && px <= W) {
        ctx.strokeStyle = WAVE_COLORS.playhead; ctx.lineWidth = 1.5;
        ctx.beginPath(); ctx.moveTo(px, 0); ctx.lineTo(px, H); ctx.stroke();
      }
    }
  }

  drawOverview() {
    const [ctx, W, H] = this._size(this.overview);
    ctx.fillStyle = "#0b0c0f"; ctx.fillRect(0, 0, W, H);
    if (!this.channels) return;
    const dpr = window.devicePixelRatio || 1;
    if (!this.overviewImage || this.overviewImage.width !== this.overview.width) {
      const spp = this.n / W, mid = H / 2, amp = H / 2 - 2;
      ctx.fillStyle = WAVE_COLORS.waveDim;
      for (let x = 0; x < W; x++) {
        let lo = 1, hi = -1;
        for (let c = 0; c < this.channels.length; c++) {
          const r = rangeOf(this.channels, this.pyramid, c, x * spp, (x + 1) * spp);
          if (r) { lo = Math.min(lo, r[0]); hi = Math.max(hi, r[1]); }
        }
        if (hi >= lo) ctx.fillRect(x, mid - hi * amp, 1, Math.max(1, (hi - lo) * amp));
      }
      this.overviewImage = ctx.getImageData(0, 0, this.overview.width, this.overview.height);
    } else {
      ctx.putImageData(this.overviewImage, 0, 0);
    }
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    const k = W / this.n;
    const region = this.region();
    if (region) {
      ctx.fillStyle = "rgba(76,208,125,0.12)";
      ctx.fillRect(region[0] * k, 0, (region[1] - region[0]) * k, H);
      if (this.loop.mode !== "whole") {
        ctx.fillStyle = WAVE_COLORS.start; ctx.fillRect(region[0] * k, 0, 1.5, H);
        ctx.fillStyle = WAVE_COLORS.end; ctx.fillRect(region[1] * k - 1, 0, 1.5, H);
      }
    }
    ctx.strokeStyle = "rgba(255,255,255,0.55)"; ctx.lineWidth = 1;
    ctx.strokeRect(this.start * k + 0.5, 0.5, Math.max(2, this.spp * this.width() * k - 1), H - 1);
    if (this.playhead !== null) { ctx.fillStyle = "#fff"; ctx.fillRect(this.playhead * k, 0, 1, H); }
  }

  _hitMarker(x) {
    if (!this.hasMarkers()) return null;
    const ds = Math.abs(x - this.xOf(this.loop.start)), de = Math.abs(x - this.xOf(this.loop.end));
    if (Math.min(ds, de) > 6) return null;
    return ds <= de ? "start" : "end";
  }

  _bind() {
    const c = this.canvas;
    const pos = (e) => { const r = c.getBoundingClientRect(); return e.clientX - r.left; };
    c.addEventListener("contextmenu", (e) => {
      e.preventDefault();
      if (this.channels && this.hasMarkers()) this.h.onSet && this.h.onSet("end", this.sampleAt(pos(e)));
    });
    c.addEventListener("mousedown", (e) => {
      if (!this.channels || e.button !== 0) return;
      c.focus();
      const x = pos(e);
      if (e.shiftKey && this.hasMarkers()) { this.h.onSet && this.h.onSet("start", this.sampleAt(x)); return; }
      if (e.altKey && this.hasMarkers()) { this.h.onSet && this.h.onSet("end", this.sampleAt(x)); return; }
      const hit = this._hitMarker(x);
      if (hit) { this.drag = { kind: "marker", which: hit }; this.h.onSelect && this.h.onSelect(hit); }
      else this.drag = { kind: "pan", x0: x, start0: this.start, moved: false };
      e.preventDefault();
    });
    window.addEventListener("mousemove", (e) => {
      if (!this.channels) return;
      const x = pos(e);
      if (!this.drag) { c.style.cursor = this._hitMarker(x) ? "ew-resize" : "crosshair"; return; }
      if (this.drag.kind === "marker") {
        this.h.onDrag && this.h.onDrag(this.drag.which, this.sampleAt(x), false);
      } else {
        const dx = x - this.drag.x0;
        if (Math.abs(dx) > 3) this.drag.moved = true;
        if (this.drag.moved) { this.start = this.drag.start0 - dx * this.spp; this.clampView(); this.invalidate(); c.style.cursor = "grabbing"; }
      }
    });
    window.addEventListener("mouseup", (e) => {
      if (!this.drag) return;
      const d = this.drag; this.drag = null;
      const x = pos(e);
      if (d.kind === "marker") this.h.onDrag && this.h.onDrag(d.which, this.sampleAt(x), true);
      else if (!d.moved) this.h.onCursor && this.h.onCursor(this.sampleAt(x));
      c.style.cursor = "crosshair";
    });
    c.addEventListener("wheel", (e) => {
      if (!this.channels) return;
      e.preventDefault();
      const horizontal = e.shiftKey || Math.abs(e.deltaX) > Math.abs(e.deltaY);
      if (horizontal) {
        const d = Math.abs(e.deltaX) > Math.abs(e.deltaY) ? e.deltaX : e.deltaY;
        this.start += d * this.spp; this.clampView(); this.invalidate();
      } else {
        this.zoomBy(Math.exp(e.deltaY * 0.0015), this.sampleAt(pos(e)));
      }
    }, { passive: false });
    const ov = this.overview;
    const ovMove = (e) => {
      const r = ov.getBoundingClientRect();
      const s = (e.clientX - r.left) / r.width * this.n;
      this.start = s - this.spp * this.width() / 2; this.clampView(); this.invalidate();
    };
    ov.addEventListener("mousedown", (e) => {
      if (!this.channels) return;
      ovMove(e);
      const move = (ev) => ovMove(ev);
      const up = () => { window.removeEventListener("mousemove", move); window.removeEventListener("mouseup", up); };
      window.addEventListener("mousemove", move); window.addEventListener("mouseup", up);
    });
  }
}

class SeamView {
  constructor(canvas) { this.canvas = canvas; }

  draw(channels, rate, start, end, widthSeconds) {
    const dpr = window.devicePixelRatio || 1, c = this.canvas;
    const W = c.clientWidth, H = c.clientHeight;
    c.width = Math.round(W * dpr); c.height = Math.round(H * dpr);
    const ctx = c.getContext("2d"); ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    ctx.fillStyle = WAVE_COLORS.bg; ctx.fillRect(0, 0, W, H);
    if (!channels || !(end > start)) return null;
    const n = channels[0].length, half = W / 2;
    const w = Math.max(4, Math.round(widthSeconds * rate));
    const nch = channels.length, laneH = H / nch;
    ctx.fillStyle = WAVE_COLORS.center; ctx.fillRect(Math.round(half), 0, 1, H);
    const px = half / w;  // pixels per sample
    for (let ch = 0; ch < nch; ch++) {
      const data = channels[ch], mid = ch * laneH + laneH / 2, amp = laneH / 2 - 3;
      ctx.fillStyle = WAVE_COLORS.grid; ctx.fillRect(0, Math.round(mid), W, 1);
      // A series of samples, the first at x0, as a line (or a min/max band when dense).
      const trace = (from, count, x0, color, width) => {
        ctx.strokeStyle = color; ctx.fillStyle = color; ctx.lineWidth = width;
        if (px >= 1) {
          ctx.beginPath();
          let begun = false;
          for (let i = 0; i < count; i++) {
            const j = from + i;
            if (j < 0 || j >= n) { begun = false; continue; }
            const x = x0 + (i + 0.5) * px, y = mid - data[j] * amp;
            if (!begun) { ctx.moveTo(x, y); begun = true; } else ctx.lineTo(x, y);
          }
          ctx.stroke();
        } else {
          const per = 1 / px;
          for (let x = 0; x < half; x++) {
            let lo = 1, hi = -1;
            for (let j = Math.floor(from + x * per); j < Math.floor(from + (x + 1) * per); j++) {
              if (j < 0 || j >= n) continue;
              const v = data[j]; if (v < lo) lo = v; if (v > hi) hi = v;
            }
            if (hi >= lo) ctx.fillRect(x0 + x, mid - hi * amp, 1, Math.max(1, (hi - lo) * amp));
          }
        }
      };
      // Ghosts: what precedes the loop start (left), what follows the loop end (right).
      trace(start - w, w, 0, WAVE_COLORS.ghost, 1);
      trace(end, w, half, WAVE_COLORS.ghost, 1);
      const color = ch === 0 ? WAVE_COLORS.ch1 : WAVE_COLORS.ch2;
      trace(end - w, w, 0, color, 1.5);
      trace(start, w, half, color, 1.5);
    }
    ctx.fillStyle = WAVE_COLORS.rulerText; ctx.font = "11px Consolas, monospace"; ctx.textBaseline = "top";
    ctx.fillText("... loop end", 6, 4);
    ctx.fillText("loop start ...", half + 6, 4);
    return seamStats(channels, start, end, w);
  }
}

// How well the two sides of a seam match: the step across it against the
// signal's usual step, and the difference of the audio before (and after)
// both points, in dB below the signal.
function seamStats(channels, start, end, w) {
  const n = channels[0].length;
  let step = 0, usual = 0, diffBefore = 0, sigBefore = 0, diffAfter = 0, sigAfter = 0, after = 0;
  for (const data of channels) {
    if (end >= 2 && start < n) {
      step = Math.max(step, Math.abs(data[start] - data[end - 1]));
      usual = Math.max(usual, Math.abs(data[end - 1] - data[end - 2]), start + 1 < n ? Math.abs(data[start + 1] - data[start]) : 0);
    }
    for (let i = 1; i <= w; i++) {
      if (start - i < 0) break;
      const a = data[end - i], b = data[start - i];
      diffBefore += (a - b) * (a - b); sigBefore += a * a;
    }
    for (let i = 0; i < w && end + i < n; i++) {
      const a = data[end + i], b = data[start + i];
      diffAfter += (a - b) * (a - b); sigAfter += b * b; after++;
    }
  }
  const db = (d, s) => s > 0 ? 10 * Math.log10(Math.max(d, 1e-20) / s) : null;
  return { step, usual, before: db(diffBefore, sigBefore), after: after ? db(diffAfter, sigAfter) : null };
}

window.WaveView = WaveView;
window.SeamView = SeamView;
window.formatTime = formatTime;
window.seamStats = seamStats;
