// The mod maker's page. Talks to the local server (modmaker/server.py) with
// the session token; plays and draws the WAV the server decoded with the
// port's FFmpeg, so every sample here is the sample the port counts.
// MIT licence; see port/README.md.
"use strict";

const $ = (id) => document.getElementById(id);
const HASH = new URLSearchParams(location.hash.slice(1));  // token, and optionally mod and track to open
const TOKEN = HASH.get("token") || sessionStorage.getItem("mm-token") || "";
if (TOKEN) { sessionStorage.setItem("mm-token", TOKEN); history.replaceState(null, "", location.pathname); }

const S = {
  state: null, folder: null, model: null, problems: [], dirty: false, saved: null,
  sel: null,            // {s, t}: the selected track
  infos: {},            // file -> info from the server
  audioCache: new Map(),// file -> {channels, mono, rate, n}
  audio: null,          // the selected track's audio
  ctx: null, play: null, cursor: 0, active: "start",
  validateTimer: null, worker: null, candidates: [],
};

// --- Server ---------------------------------------------------------------

async function api(path, body) {
  const res = await fetch(path, {
    method: "POST", headers: { "Content-Type": "application/json", "X-Mod-Maker-Token": TOKEN },
    body: JSON.stringify(body || {}),
  });
  const data = await res.json().catch(() => ({ error: `HTTP ${res.status}` }));
  if (!res.ok) throw new Error(data.error || `HTTP ${res.status}`);
  return data;
}

function status(text, kind) {
  const el = $("status");
  el.textContent = text; el.className = kind || "";
}

// --- Model helpers ----------------------------------------------------------

const track = () => S.sel && S.model ? S.model.series[S.sel.s].tracks[S.sel.t] : null;
const allTrackKeys = (except) => {
  const keys = [];
  for (const s of S.model.series) for (const t of s.tracks) if (t !== except) keys.push(t.key);
  return keys;
};
const loopOf = (t) => t.loop || (t.loop = { mode: "auto" });

function changed() {
  S.dirty = true;
  status("Not saved", "dirty");
  clearTimeout(S.validateTimer);
  S.validateTimer = setTimeout(validate, 350);
  renderPortCommand();
}

async function validate() {
  if (!S.model) return;
  try {
    const r = await api("/api/mod/validate", { folder: S.folder, model: wireModel() });
    S.problems = r.problems;
  } catch (e) { S.problems = [{ level: "error", path: "mod", message: e.message }]; }
  renderProblems();
}

// The model as the server takes it: no private fields.
function wireModel() {
  return JSON.parse(JSON.stringify(S.model, (k, v) => (k.startsWith("_") ? undefined : v)));
}

async function save() {
  if (!S.model) return;
  status("Saving...", "");
  clearTimeout(S.validateTimer);
  try {
    const r = await api("/api/mod/save", { folder: S.folder, model: wireModel() });
    S.problems = r.problems; renderProblems();
    if (!r.ok) { status("Not saved: fix the problems listed", "bad"); return; }
    S.dirty = false;
    for (const s of S.model.series) { s._autoKey = false; for (const t of s.tracks) t._autoKey = false; }
    status(r.unchanged ? "Saved (no change)" : `Saved mod.json${r.backup ? "; the old one is in " + r.backup : ""}`, "ok");
    refreshMods();
  } catch (e) { status("Save failed: " + e.message, "bad"); }
}

// --- Mods -------------------------------------------------------------------

async function refreshMods(select) {
  S.state = await api("/api/state");
  const sel = $("mod-select");
  const current = select || S.folder;
  sel.innerHTML = "";
  const none = new Option(S.state.mods.length ? "Choose a mod..." : "No mods yet", "");
  sel.add(none);
  for (const m of S.state.mods) {
    const label = m.error ? `${m.folder} (${m.error})` : `${m.name || m.folder}  [${m.id}]`;
    sel.add(new Option(label, m.folder));
  }
  sel.value = current || "";
  $("mods-dir-line").textContent = "Mods folder: " + S.state.mods_dir;
  const ff = S.state.ffmpeg;
  $("ffmpeg-line").textContent = `FFmpeg ${ff.version} (${ff.source})`;
}

async function openMod(folder) {
  if (S.dirty && !confirm("Discard the unsaved changes?")) { $("mod-select").value = S.folder || ""; return; }
  stop();
  if (!folder) { S.model = null; S.folder = null; render(); return; }
  try {
    const r = await api("/api/mod/open", { folder });
    S.folder = folder; S.model = r.model; S.problems = r.problems; S.sel = null; S.dirty = false;
    S.model.series.sort((a, b) => (a.order - b.order) || (a.key < b.key ? -1 : a.key > b.key ? 1 : 0));
    S.infos = {};
    if (r.unreadable) status("mod.json could not be read; saving replaces it (a backup is kept)", "bad");
    else status("Opened " + folder, "");
    const first = S.model.series.findIndex((s) => s.tracks.length);
    if (first >= 0) S.sel = { s: first, t: 0 };
    render();
    if (S.sel) selectTrack(S.sel.s, S.sel.t);
    validate();
  } catch (e) { status("Cannot open: " + e.message, "bad"); }
}

function newModDialog() {
  const dlg = $("new-dialog"), form = $("new-form");
  form.reset();
  form.elements.version.value = "1.0.0";
  $("new-problems").innerHTML = "";
  dlg.showModal();
  form.elements.id.focus();
}

async function createMod(ev) {
  ev.preventDefault();
  const f = $("new-form").elements;
  const body = { id: f.id.value.trim(), name: f.name.value.trim(), version: f.version.value.trim(),
                 author: f.author.value.trim(), description: f.description.value.trim(), folder: f.folder.value.trim() };
  try {
    const r = await api("/api/mod/create", body);
    if (!r.ok) {
      $("new-problems").innerHTML = "";
      for (const p of r.problems.filter((p) => p.level === "error")) {
        const li = document.createElement("li"); li.textContent = p.message; $("new-problems").appendChild(li);
      }
      return;
    }
    $("new-dialog").close();
    S.dirty = false;
    await refreshMods(r.folder);
    await openMod(r.folder);
  } catch (e) {
    $("new-problems").innerHTML = `<li></li>`; $("new-problems").firstChild.textContent = e.message;
  }
}

// --- Rendering: the side panel ------------------------------------------------

function render() {
  const has = !!S.model;
  $("welcome").hidden = has; $("mod-panel").hidden = !has; $("series-panel").hidden = !has;
  if (!has) { $("track-editor").hidden = true; $("no-track").hidden = false; renderProblems(); return; }
  for (const k of ["id", "name", "version", "author", "description"]) {
    const el = $("mod-" + k);
    if (document.activeElement !== el) el.value = S.model[k] || "";
  }
  $("mod-folder-line").textContent = `Folder: ${S.state.mods_dir}${S.state.mods_dir.includes("\\") ? "\\" : "/"}${S.folder}`;
  renderSeries();
  renderTrackEditor();
  renderProblems();
  renderPortCommand();
}

function renderSeries() {
  const list = $("series-list");
  list.innerHTML = "";
  S.model.series.forEach((series, si) => {
    const card = document.createElement("div");
    card.className = "series"; card.dataset.s = si;
    card.innerHTML = `
      <div class="series-head">
        <label>Name <input class="s-name" placeholder="FINAL FANTASY XV" data-path="series.${si}.name"></label>
        <label>Order <input class="s-order mono" inputmode="numeric" data-path="series.${si}.order"></label>
      </div>
      <div class="series-meta">
        <label class="key" style="margin:0">Key <input class="s-key mono" spellcheck="false" data-path="series.${si}.key"></label>
        <button type="button" class="small s-upper" title="Upper case, as the game's series names are" hidden>ABC</button>
        <button type="button" class="small s-up" title="Earlier in the game's list">&#9650;</button>
        <button type="button" class="small s-down" title="Later in the game's list">&#9660;</button>
        <button type="button" class="small danger s-del" title="Remove the series">&#10005;</button>
      </div>
      <div class="series-errors errors" style="padding:0 8px"></div>
      <ul class="tracks"></ul>
      <div class="dropzone">Drop audio files here <button type="button" class="small s-add">Add files...</button></div>`;
    const name = card.querySelector(".s-name"), order = card.querySelector(".s-order"), key = card.querySelector(".s-key");
    const upper = card.querySelector(".s-upper");
    name.value = series.name; order.value = series.order; key.value = series.key;
    const showUpper = () => { upper.hidden = series.name === series.name.toUpperCase(); };
    showUpper();
    name.addEventListener("input", async () => {
      series.name = name.value; showUpper(); changed();
      if (series._autoKey) {
        const r = await api("/api/key", { kind: "series", text: series.name, taken: S.model.series.filter((x) => x !== series).map((x) => x.key) });
        series.key = r.key; key.value = r.key; changed();
      }
    });
    upper.addEventListener("click", () => { series.name = series.name.toUpperCase(); name.value = series.name; showUpper(); changed(); renderTrackSeriesSelect(); });
    name.addEventListener("change", renderTrackSeriesSelect);
    order.addEventListener("change", () => {
      const v = Number(order.value);
      series.order = Number.isFinite(v) ? Math.round(v) : 0;
      S.model.series.sort((a, b) => (a.order - b.order) || (a.key < b.key ? -1 : 1));
      fixSelection(); changed(); render();
    });
    key.addEventListener("input", () => { series.key = key.value; series._autoKey = false; changed(); });
    card.querySelector(".s-up").addEventListener("click", () => moveSeries(si, -1));
    card.querySelector(".s-down").addEventListener("click", () => moveSeries(si, 1));
    card.querySelector(".s-del").addEventListener("click", () => {
      if (series.tracks.length && !confirm(`Remove the series "${series.name}" and its ${series.tracks.length} track(s) from the mod? Their files stay in the folder.`)) return;
      S.model.series.splice(si, 1); S.sel = null; changed(); render();
    });
    card.querySelector(".s-add").addEventListener("click", () => { S.addTo = si; $("file-input").click(); });
    card.addEventListener("dragover", (e) => { e.preventDefault(); card.classList.add("drop"); });
    card.addEventListener("dragleave", () => card.classList.remove("drop"));
    card.addEventListener("drop", (e) => { e.preventDefault(); e.stopPropagation(); card.classList.remove("drop"); addFiles(si, e.dataTransfer.files); });

    const ul = card.querySelector(".tracks");
    series.tracks.forEach((t, ti) => {
      const li = document.createElement("li");
      li.className = "track" + (S.sel && S.sel.s === si && S.sel.t === ti ? " selected" : "");
      const mode = loopOf(t).mode;
      const hasError = S.problems.some((p) => p.level === "error" && p.path.startsWith(`series.${si}.tracks.${ti}`));
      li.innerHTML = `${hasError ? '<span class="err-dot" title="Has problems"></span>' : ""}
        <span class="t-title"></span><span class="badge ${mode === "custom" ? "custom" : ""}">${mode === "custom" ? "loop" : mode}</span>
        <span class="tools"><button type="button" class="t-up" title="Earlier">&#9650;</button><button type="button" class="t-down" title="Later">&#9660;</button><button type="button" class="t-del danger" title="Remove from the mod">&#10005;</button></span>`;
      li.querySelector(".t-title").textContent = t.title || "(no title)";
      li.addEventListener("click", (e) => { if (!e.target.closest("button")) selectTrack(si, ti); });
      li.querySelector(".t-up").addEventListener("click", () => moveTrack(si, ti, -1));
      li.querySelector(".t-down").addEventListener("click", () => moveTrack(si, ti, 1));
      li.querySelector(".t-del").addEventListener("click", () => {
        if (!confirm(`Remove "${t.title}" from the mod? Its file stays in the folder.`)) return;
        series.tracks.splice(ti, 1);
        if (S.sel && S.sel.s === si) S.sel = series.tracks.length ? { s: si, t: Math.min(ti, series.tracks.length - 1) } : null;
        changed(); render(); if (S.sel) selectTrack(S.sel.s, S.sel.t);
      });
      ul.appendChild(li);
    });
    if (series._uploads) {
      const up = document.createElement("div"); up.className = "uploading"; up.textContent = series._uploads; card.insertBefore(up, card.querySelector(".dropzone"));
    }
    list.appendChild(card);
  });
  markFieldProblems();
}

function fixSelection() {
  if (!S.sel || !S._selTrack) return;
  S.model.series.forEach((s, si) => s.tracks.forEach((t, ti) => { if (t === S._selTrack) S.sel = { s: si, t: ti }; }));
}

function moveSeries(si, dir) {
  const to = si + dir;
  if (to < 0 || to >= S.model.series.length) return;
  const list = S.model.series;
  [list[si], list[to]] = [list[to], list[si]];
  list.forEach((s, i) => { s.order = i * 10; });  // the game sorts by order
  fixSelection(); changed(); render();
}

function moveTrack(si, ti, dir) {
  const tracks = S.model.series[si].tracks, to = ti + dir;
  if (to < 0 || to >= tracks.length) return;
  [tracks[ti], tracks[to]] = [tracks[to], tracks[ti]];
  fixSelection(); changed(); render();
}

async function addSeries() {
  const order = S.model.series.reduce((m, s) => Math.max(m, s.order + 10), 0);
  const key = (await api("/api/key", { kind: "series", text: "series", taken: S.model.series.map((s) => s.key) })).key;
  S.model.series.push({ key, name: "", order, extra: {}, tracks: [], _autoKey: true });
  changed(); render();
  const cards = document.querySelectorAll(".series .s-name");
  cards[cards.length - 1].focus();
}

async function addFiles(si, files) {
  if (!S.model) return;
  if (si === undefined || si === null || !S.model.series[si]) {
    if (!S.model.series.length) await addSeries();
    si = S.sel ? S.sel.s : 0;
  }
  const series = S.model.series[si];
  const list = Array.from(files || []);
  let lastAdded = null;
  for (let i = 0; i < list.length; i++) {
    const file = list[i];
    series._uploads = `Copying ${file.name} (${i + 1} of ${list.length})...`; renderSeries();
    try {
      const res = await fetch(`/api/track/import?folder=${encodeURIComponent(S.folder)}&name=${encodeURIComponent(file.name)}`,
        { method: "POST", headers: { "X-Mod-Maker-Token": TOKEN, "Content-Type": "application/octet-stream" }, body: file });
      const r = await res.json();
      if (!res.ok) throw new Error(r.error || res.status);
      const key = (await api("/api/key", { kind: "track", text: r.title, taken: allTrackKeys(), mod_id: S.model.id, folder: S.folder })).key;
      const t = { key, title: r.title, file: r.file, extra: {}, loop: { mode: "whole" }, _autoKey: true };
      if (r.info && r.info.loop_tags) t.loop = { mode: "auto" };
      series.tracks.push(t);
      S.infos[r.file] = r.info;
      lastAdded = { s: si, t: series.tracks.length - 1 };
      changed();
    } catch (e) {
      status(`Cannot add ${file.name}: ${e.message}`, "bad");
    }
  }
  series._uploads = null;
  render();
  if (lastAdded) selectTrack(lastAdded.s, lastAdded.t);
}

// --- Problems -----------------------------------------------------------------

function describePath(path) {
  const m = path.match(/^series\.(\d+)(?:\.tracks\.(\d+))?/);
  if (!m || !S.model) return path === "mod" || path.startsWith("mod.") ? "the mod" : path;
  const s = S.model.series[+m[1]];
  if (!s) return path;
  if (m[2] === undefined) return `series ${s.name || s.key}`;
  const t = s.tracks[+m[2]];
  return t ? `${s.name || s.key} / ${t.title || t.key}` : path;
}

function renderProblems() {
  const ul = $("problems");
  ul.innerHTML = "";
  const errors = S.problems.filter((p) => p.level === "error").length, warnings = S.problems.length - errors;
  $("problem-count").textContent = S.problems.length ? `${errors} error(s), ${warnings} warning(s)` : "none";
  // "... is left out" lines repeat what the line before said; keep them out of the way.
  for (const p of S.problems) {
    if (/ is left out$/.test(p.message) && S.problems.some((q) => q !== p && q.path.startsWith(p.path + "."))) continue;
    const li = document.createElement("li");
    li.className = p.level;
    li.textContent = p.message;
    const where = document.createElement("span"); where.className = "where"; where.textContent = describePath(p.path);
    li.appendChild(where);
    li.addEventListener("click", () => {
      const m = p.path.match(/^series\.(\d+)\.tracks\.(\d+)/);
      if (m) selectTrack(+m[1], +m[2]);
    });
    ul.appendChild(li);
  }
  $("save").disabled = !S.model;
  markFieldProblems();
  if (S.model) {
    // The error dots in the track list.
    document.querySelectorAll(".series").forEach((card) => {
      const si = +card.dataset.s;
      card.querySelectorAll(".track").forEach((li, ti) => {
        const has = S.problems.some((p) => p.level === "error" && p.path.startsWith(`series.${si}.tracks.${ti}`));
        const dot = li.querySelector(".err-dot");
        if (has && !dot) { const d = document.createElement("span"); d.className = "err-dot"; d.title = "Has problems"; li.prepend(d); }
        if (!has && dot) dot.remove();
      });
      const own = S.problems.filter((p) => p.level === "error" && (p.path === `series.${si}` || /^series\.\d+\.(name|key|order)$/.test(p.path) && p.path.startsWith(`series.${si}.`)));
      const box = card.querySelector(".series-errors");
      if (box) box.textContent = own.filter((p) => !/ is left out$/.test(p.message)).map((p) => p.message).join(" · ");
    });
  }
  renderTrackErrors();
}

function markFieldProblems() {
  document.querySelectorAll("[data-path]").forEach((el) => {
    const bad = S.problems.some((p) => p.level === "error" && (p.path === el.dataset.path));
    el.classList.toggle("bad", bad);
    el.title = bad ? S.problems.filter((p) => p.path === el.dataset.path).map((p) => p.message).join("\n") : "";
  });
}

function renderTrackErrors() {
  const box = $("track-errors");
  box.innerHTML = "";
  if (!S.sel) return;
  const prefix = `series.${S.sel.s}.tracks.${S.sel.t}`;
  for (const p of S.problems) {
    if (!(p.path === prefix || p.path.startsWith(prefix + "."))) continue;
    if (/ is left out$/.test(p.message)) continue;
    const div = document.createElement("div");
    div.textContent = p.message;
    div.style.color = p.level === "error" ? "var(--error)" : "var(--warn)";
    box.appendChild(div);
  }
  for (const [id, field] of [["track-title", "title"], ["track-key", "key"]]) {
    $(id).classList.toggle("bad", S.problems.some((p) => p.level === "error" && p.path === `${prefix}.${field}`));
  }
  const loopBad = S.problems.some((p) => p.level === "error" && p.path.startsWith(`${prefix}.loop`));
  $("start-samples").classList.toggle("bad", loopBad); $("end-samples").classList.toggle("bad", loopBad);
}

// --- Port command ---------------------------------------------------------------

async function renderPortCommand() {
  if (!S.state) return;
  const t = track();
  const trackId = S.model && t && S.model.id && t.key ? `${S.model.id}:${t.key}` : null;
  try {
    const r = await api("/api/port/commands", { track_id: trackId });
    $("port-command").textContent = r.powershell + (r.cleanup ? `\n# afterwards: ${r.cleanup}` : "");
    $("port-where").textContent = r.where;
  } catch (e) { $("port-command").textContent = e.message; }
}

// --- The track editor -------------------------------------------------------------

function renderTrackSeriesSelect() {
  const sel = $("track-series");
  sel.innerHTML = "";
  if (!S.model) return;
  S.model.series.forEach((s, i) => sel.add(new Option(s.name || s.key || `series ${i + 1}`, String(i))));
  if (S.sel) sel.value = String(S.sel.s);
}

function renderTrackEditor() {
  const t = track();
  $("track-editor").hidden = !t; $("no-track").hidden = !!t;
  if (!t) return;
  if (document.activeElement !== $("track-title")) $("track-title").value = t.title;
  if (document.activeElement !== $("track-key")) $("track-key").value = t.key;
  renderTrackSeriesSelect();
  const info = S.infos[t.file];
  let line = `File: ${t.file}`;
  if (info && !info.error) {
    const ch = info.channels === 1 ? "mono" : info.channels === 2 ? "stereo" : `${info.channels} channels`;
    line += ` · ${info.codec} · ${info.rate} Hz ${ch}`;
    if (info.samples) line += ` · ${formatTime(info.samples, info.rate)} · ${info.samples.toLocaleString()} samples`;
    line += info.rate === 44100 ? " · the port plays it at its own rate" : " · the port resamples it to 44,100 Hz (loop points to the nearest frame)";
  } else if (info && info.error) line += ` · cannot be read: ${info.error}`;
  $("track-file").textContent = line;
  const loop = loopOf(t);
  document.querySelectorAll('input[name="loop-mode"]').forEach((r) => { r.checked = r.value === loop.mode; });
  const tags = info && info.loop_tags;
  $("auto-note").textContent = tags ? `(the file's loop tags: ${tags.start} to ${tags.end ?? "the end"})` : "(no loop tags in the file, so the whole file)";
  renderLoopFields();
  renderTrackErrors();
}

// The loop the editor shows and plays: {mode, start, end} with start/end in samples.
function effectiveLoop() {
  const t = track(), a = S.audio;
  if (!t || !a) return { mode: "none", start: 0, end: 0 };
  const loop = loopOf(t);
  if (loop.mode === "custom") {
    const end = loop.end === null || loop.end === undefined ? a.n : loop.end;
    return { mode: "custom", start: loop.start || 0, end };
  }
  if (loop.mode === "auto") {
    const tags = S.infos[t.file] && S.infos[t.file].loop_tags;
    if (tags) return { mode: "tags", start: tags.start, end: tags.end ?? a.n };
    return { mode: "whole", start: 0, end: a.n };
  }
  if (loop.mode === "whole") return { mode: "whole", start: 0, end: a.n };
  return { mode: "none", start: 0, end: a.n };
}

function bpm() { const v = Number($("bpm").value); return v > 0 && v < 1000 ? v : 120; }

function renderLoopFields() {
  const a = S.audio, eff = effectiveLoop(), custom = eff.mode === "custom";
  for (const which of ["start", "end"]) {
    const box = $("box-" + which);
    box.classList.toggle("off", !custom);
    box.classList.toggle("active", custom && S.active === which);
    box.querySelectorAll("input, button").forEach((el) => { el.disabled = !custom || !a; });
    const v = which === "start" ? eff.start : eff.end;
    if (document.activeElement !== $(which + "-samples")) $(which + "-samples").value = a ? String(v) : "";
    if (document.activeElement !== $(which + "-time")) $(which + "-time").value = a ? formatTime(v, a.rate) : "";
  }
  if (a) {
    const len = eff.end - eff.start;
    const beats = len / a.rate * bpm() / 60;
    $("loop-length").innerHTML = "";
    const lines = eff.mode === "none" ? ["Plays once, no loop."] : [
      `loop ${len.toLocaleString()} samples`, `${formatTime(len, a.rate)} long`,
      `${beats.toFixed(2)} beats at ${bpm()} BPM`, `intro ${formatTime(eff.start, a.rate)}`];
    for (const l of lines) { const d = document.createElement("div"); d.textContent = l; $("loop-length").appendChild(d); }
  }
  S.wave.setLoop(eff);
  S.wave.setActive(S.active);
  renderSeam();
}

function renderSeam() {
  const a = S.audio, eff = effectiveLoop();
  if (!a || eff.mode === "none") { S.seam.draw(null); $("seam-info").textContent = ""; return; }
  const stats = S.seam.draw(a.channels, a.rate, eff.start, eff.end, Number($("seam-width").value));
  if (!stats) return;
  const f = (v) => (v === null ? "-" : v.toFixed(1) + " dB");
  $("seam-info").textContent = `step across the seam ${(stats.step * 100).toFixed(2)}% of full scale (neighbouring samples ${(stats.usual * 100).toFixed(2)}%)` +
    ` · audio before end vs before start: ${f(stats.before)} · after end vs after start: ${stats.after === null ? "(end of file)" : f(stats.after)}` +
    " (lower is closer)";
}

function setLoop(start, end, opts) {
  const t = track(), a = S.audio;
  if (!t || !a) return;
  start = Math.max(0, Math.min(Math.round(start), a.n - 2));
  end = Math.max(start + 2, Math.min(Math.round(end), a.n));
  const loop = loopOf(t);
  loop.mode = "custom"; loop.start = start; loop.end = end;
  delete loop.converted_from_seconds;
  delete loop.start_seconds; delete loop.end_seconds;
  renderLoopFields();
  if (!opts || !opts.quiet) { changed(); updateTrackBadge(); }
}

function updateTrackBadge() {
  const li = document.querySelectorAll(".series")[S.sel.s]?.querySelectorAll(".track")[S.sel.t];
  if (!li) return;
  const mode = loopOf(track()).mode, b = li.querySelector(".badge");
  b.textContent = mode === "custom" ? "loop" : mode; b.className = "badge" + (mode === "custom" ? " custom" : "");
}

function setMarker(which, sample, opts) {
  const eff = effectiveLoop();
  if (eff.mode !== "custom") return;
  S.active = which;
  if (which === "start") setLoop(Math.min(sample, eff.end - 2), eff.end, opts);
  else setLoop(eff.start, Math.max(sample, eff.start + 2), opts);
}

function stepSamples() {
  const a = S.audio, v = $("step").value;
  if (v === "1" || v === "10") return Number(v);
  if (v === "ms1") return Math.round(a.rate / 1000);
  if (v === "ms10") return Math.round(a.rate / 100);
  if (v === "beat") return Math.round(a.rate * 60 / bpm());
  return Math.round(a.rate * 240 / bpm());
}

function nudge(which, dir, times) {
  const eff = effectiveLoop();
  if (eff.mode !== "custom") return;
  const v = (which === "start" ? eff.start : eff.end) + dir * stepSamples() * (times || 1);
  setMarker(which, v);
  S.wave.ensureVisible(which === "start" ? effectiveLoop().start : effectiveLoop().end);
}

// The nearest zero crossing of the summed channels: the first sample after
// the sign changes, preferring the same direction as at the other marker.
function snapZero(which) {
  const a = S.audio, eff = effectiveLoop();
  if (!a || eff.mode !== "custom") return;
  const m = a.mono, at = which === "start" ? eff.start : eff.end, other = which === "start" ? eff.end : eff.start;
  const dirAt = (i) => (i > 0 && i < m.length ? Math.sign(m[i] - m[i - 1]) : 0);
  const want = dirAt(other);
  const crossing = (i) => i > 0 && i < m.length && (m[i - 1] < 0) !== (m[i] < 0);
  const radius = Math.round(a.rate * 0.02);
  let any = null, same = null;
  for (let d = 0; d <= radius && same === null; d++) {
    for (const i of [at - d, at + d]) {
      if (!crossing(i)) continue;
      const j = Math.abs(m[i - 1]) < Math.abs(m[i]) ? i - 1 : i;  // the sample nearer zero starts the marker
      if (any === null) any = j;
      if (same === null && (want === 0 || dirAt(i) === want)) same = j;
    }
  }
  const target = same ?? any;
  if (target === null) { status("No zero crossing within 20 ms", "bad"); return; }
  setMarker(which, target);
}

// --- Audio --------------------------------------------------------------------------

function parseWav(buf) {
  const v = new DataView(buf);
  const tag = (o) => String.fromCharCode(v.getUint8(o), v.getUint8(o + 1), v.getUint8(o + 2), v.getUint8(o + 3));
  if (tag(0) !== "RIFF" || tag(8) !== "WAVE") throw new Error("not a WAV");
  let o = 12, rate = 0, ch = 0, bits = 0;
  while (o + 8 <= buf.byteLength) {
    const id = tag(o), size = v.getUint32(o + 4, true);
    if (id === "fmt ") { ch = v.getUint16(o + 10, true); rate = v.getUint32(o + 12, true); bits = v.getUint16(o + 22, true); }
    if (id === "data") {
      if (bits !== 16) throw new Error("expected 16-bit PCM");
      const len = Math.min(size, buf.byteLength - o - 8);
      const n = Math.floor(len / (2 * ch));
      const pcm = new Int16Array(buf, o + 8, n * ch);
      const channels = [];
      for (let c = 0; c < ch; c++) channels.push(new Float32Array(n));
      for (let i = 0, k = 0; i < n; i++) for (let c = 0; c < ch; c++) channels[c][i] = pcm[k++] / 32768;
      return { rate, channels };
    }
    o += 8 + size + (size & 1);
  }
  throw new Error("no data in the WAV");
}

async function loadAudio(file) {
  if (S.audioCache.has(file)) return S.audioCache.get(file);
  const res = await fetch(`/api/track/audio?folder=${encodeURIComponent(S.folder)}&file=${encodeURIComponent(file)}`,
                          { headers: { "X-Mod-Maker-Token": TOKEN } });
  if (!res.ok) throw new Error((await res.json().catch(() => ({}))).error || `HTTP ${res.status}`);
  const { rate, channels } = parseWav(await res.arrayBuffer());
  const n = channels[0].length;
  let mono = channels[0];
  if (channels.length > 1) {
    mono = new Float32Array(n);
    for (let i = 0; i < n; i++) { let s = 0; for (const c of channels) s += c[i]; mono[i] = s / channels.length; }
  }
  const audio = { file, rate, channels, mono, n, buffer: null };
  S.audioCache.set(file, audio);
  while (S.audioCache.size > 3) S.audioCache.delete(S.audioCache.keys().next().value);
  return audio;
}

async function selectTrack(si, ti) {
  stop();
  S.sel = { s: si, t: ti };
  const t = track();
  S._selTrack = t;
  document.querySelectorAll(".track").forEach((el) => el.classList.remove("selected"));
  const li = document.querySelectorAll(".series")[si]?.querySelectorAll(".track")[ti];
  if (li) li.classList.add("selected");
  S.audio = null; S.wave.clear(); S.candidates = []; renderCandidates(); $("suggest-status").textContent = "";
  renderTrackEditor(); renderPortCommand();
  if (!t) return;
  $("wave-busy").hidden = false;
  try {
    const [info, audio] = await Promise.all([
      S.infos[t.file] && S.infos[t.file].samples ? S.infos[t.file] : api("/api/track/info", { folder: S.folder, file: t.file }),
      loadAudio(t.file)]);
    if (track() !== t) return;
    S.infos[t.file] = info;
    S.audio = audio; S.cursor = 0;
    S.wave.setAudio(audio.channels, audio.rate);
    if (info.samples && info.samples !== audio.n) status(`Note: the decoded length differs (${info.samples} vs ${audio.n})`, "bad");
    renderTrackEditor();
    validate();
  } catch (e) {
    $("track-file").textContent = `File: ${t.file} · cannot be decoded: ${e.message}`;
  } finally { $("wave-busy").hidden = true; }
}

function context(rate) {
  if (S.ctx && S.ctx.sampleRate !== rate && S.ctx._wanted !== rate) { S.ctx.close(); S.ctx = null; }
  if (!S.ctx) {
    try { S.ctx = new AudioContext({ sampleRate: rate }); } catch (e) { S.ctx = new AudioContext(); }
    S.ctx._wanted = rate;
  }
  if (S.ctx.state === "suspended") S.ctx.resume();
  return S.ctx;
}

function bufferOf(a, from, to) {
  const ctx = context(a.rate);
  const len = Math.max(1, to - from);
  const b = ctx.createBuffer(a.channels.length, len, a.rate);
  a.channels.forEach((c, i) => b.copyToChannel(c.subarray(from, to), i));
  return b;
}

function fullBuffer(a) {
  if (!a.buffer || a.buffer._ctx !== S.ctx) { a.buffer = bufferOf(a, 0, a.n); a.buffer._ctx = S.ctx; }
  return a.buffer;
}

function stop() {
  if (S.play) {
    try { S.play.src.stop(); } catch (e) { /* already stopped */ }
    S.play.src.disconnect();
    cancelAnimationFrame(S.play.raf);
    S.play = null;
  }
  $("play").textContent = "Play";
  if (S.wave) S.wave.setPlayhead(null);
}

function currentPosition() {
  if (!S.play) return null;
  return S.play.map(S.ctx.currentTime - S.play.t0);
}

// Plays from `from` as the port would: on a loop, the loop end runs into its start.
function play(from) {
  const a = S.audio;
  if (!a) return;
  stop();
  const ctx = context(a.rate);
  const eff = effectiveLoop();
  const src = ctx.createBufferSource();
  src.buffer = fullBuffer(a);
  from = Math.max(0, Math.min(from, a.n - 1));
  let map;
  if (eff.mode !== "none" && from < eff.end) {
    src.loop = true; src.loopStart = eff.start / a.rate; src.loopEnd = eff.end / a.rate;
    const len = eff.end - eff.start;
    map = (t) => { let p = from + t * a.rate; if (p >= eff.end) p = eff.start + ((p - eff.end) % len); return p; };
  } else {
    map = (t) => Math.min(from + t * a.rate, a.n);
  }
  src.connect(ctx.destination);
  const t0 = ctx.currentTime + 0.03;
  src.start(t0, from / a.rate);
  startPlayhead(src, t0, map, from);
}

// The seam on its own, sample-exact: [end - pre, end) then [start, start + post),
// following the loop if it is shorter than `post`.
function audition(startS, endS) {
  const a = S.audio;
  if (!a) return;
  const eff = effectiveLoop();
  const start = startS ?? eff.start, end = endS ?? eff.end;
  if (eff.mode === "none" && startS === undefined) { status("No loop to audition: pick Custom loop or Whole file", "bad"); return; }
  stop();
  const ctx = context(a.rate);
  const pre = Math.min(Math.round(Number($("pre-roll").value) * a.rate), end);
  const post = Math.round(Number($("post-roll").value) * a.rate);
  const gap = Math.round(0.6 * a.rate);
  const len = pre + post + ($("repeat").checked ? gap : 0);
  const b = ctx.createBuffer(a.channels.length, len, a.rate);
  const segments = [{ at: 0, from: end - pre, count: pre }];
  let at = pre, p = start, left = post;
  while (left > 0) {
    const take = Math.min(left, end - p);
    segments.push({ at, from: p, count: take });
    at += take; left -= take; p = start;
  }
  a.channels.forEach((c, i) => {
    const out = new Float32Array(len);
    for (const s of segments) out.set(c.subarray(s.from, s.from + s.count), s.at);
    b.copyToChannel(out, i);
  });
  const src = ctx.createBufferSource();
  src.buffer = b; src.loop = $("repeat").checked;
  src.connect(ctx.destination);
  const t0 = ctx.currentTime + 0.03;
  src.start(t0);
  const map = (t) => {
    let i = Math.floor(t * a.rate);
    if (src.loop) i %= len;
    for (const s of segments) if (i >= s.at && i < s.at + s.count) return s.from + (i - s.at);
    return i >= pre + post ? null : end;
  };
  startPlayhead(src, t0, map, end - pre, !src.loop);
  S.wave.ensureVisible(end - pre);
}

function startPlayhead(src, t0, map, from, endsByItself) {
  S.play = { src, t0, map, raf: 0 };
  $("play").textContent = "Pause";
  src.onended = () => { if (S.play && S.play.src === src) { stop(); } };
  const tick = () => {
    if (!S.play || S.play.src !== src) return;
    const p = currentPosition();
    if (p !== null) {
      S.wave.setPlayhead(p);
      $("position").textContent = formatTime(Math.round(p), S.audio.rate) + "  ·  " + Math.round(p).toLocaleString();
      const span = S.wave.spp * S.wave.width();
      if (p > S.wave.start + span * 0.97 || p < S.wave.start) { S.wave.start = p - span * 0.05; S.wave.clampView(); }
    }
    S.play.raf = requestAnimationFrame(tick);
  };
  tick();
}

function togglePlay() {
  if (S.play) {
    const p = currentPosition();
    stop();
    if (p !== null) { S.cursor = Math.round(p); S.wave.setCursor(S.cursor); }
    return;
  }
  play(S.cursor);
}

function here() {
  const p = currentPosition();
  return p !== null ? Math.round(p) : S.cursor;
}

// --- Suggestions -------------------------------------------------------------------

function runSuggest(mode) {
  const a = S.audio;
  if (!a) return;
  if (S.worker) S.worker.terminate();
  const worker = new Worker("/static/suggest.js");
  S.worker = worker;
  $("suggest-status").textContent = "Listening for repeats...";
  $("suggest-any").disabled = $("suggest-end").disabled = true;
  const eff = effectiveLoop();
  const mono = a.mono.slice();
  worker.onmessage = (ev) => {
    const m = ev.data;
    if (m.progress !== undefined) { $("suggest-status").textContent = `Searching... ${Math.round(m.progress * 100)}%`; return; }
    worker.terminate(); S.worker = null;
    $("suggest-any").disabled = $("suggest-end").disabled = false;
    if (m.error) { $("suggest-status").textContent = "Failed: " + m.error.split("\n")[0]; return; }
    S.candidates = m.result.candidates;
    $("suggest-status").textContent = S.candidates.length ? `${S.candidates.length} found in ${(m.result.ms / 1000).toFixed(1)} s` : (m.result.note || "No repeat found");
    renderCandidates();
  };
  worker.postMessage({ mono, rate: a.rate, mode, end: mode === "end" ? eff.end : undefined, minLoop: Number($("min-loop").value) || 8 }, [mono.buffer]);
}

function renderCandidates() {
  const ol = $("suggest-list");
  ol.innerHTML = "";
  const a = S.audio;
  if (!a) return;
  S.candidates.forEach((c) => {
    const li = document.createElement("li");
    li.className = "cand" + (c.score < 0.9 ? " weak" : "");
    const what = document.createElement("span"); what.className = "what mono";
    what.textContent = `${formatTime(c.start, a.rate)} → ${formatTime(c.end, a.rate)}   loop ${formatTime(c.length, a.rate)}   (${c.start.toLocaleString()} – ${c.end.toLocaleString()})`;
    const score = document.createElement("span"); score.className = "score mono";
    score.textContent = `match ${(c.score * 100).toFixed(1)}%` + (Math.abs(c.levelDb) > 1 ? `, level ${c.levelDb > 0 ? "+" : ""}${c.levelDb.toFixed(1)} dB` : "");
    const listen = document.createElement("button"); listen.type = "button"; listen.textContent = "Audition"; listen.className = "accent small";
    listen.addEventListener("click", () => audition(c.start, c.end));
    const use = document.createElement("button"); use.type = "button"; use.textContent = "Use"; use.className = "small";
    use.addEventListener("click", () => { setLoop(c.start, c.end); S.wave.zoomAround(c.end, a.rate * 4); });
    const li2 = document.createElement("div"); li2.style.display = "flex"; li2.style.gap = "6px"; li2.append(listen, use);
    li.append(what, score, li2);
    ol.appendChild(li);
  });
}

// --- Wiring ------------------------------------------------------------------------

function commitSamples(which) {
  const v = Number($(which + "-samples").value.replace(/[ ,_]/g, ""));
  if (!Number.isFinite(v)) { renderLoopFields(); return; }
  setMarker(which, v);
}

function commitTime(which) {
  const text = $(which + "-time").value.trim();
  const m = text.match(/^(?:(\d+):)?(?:(\d+):)?(\d+(?:\.\d*)?)$/);
  if (!m || !S.audio) { renderLoopFields(); return; }
  const parts = text.split(":").map(Number);
  let sec = 0;
  for (const p of parts) sec = sec * 60 + p;
  setMarker(which, Math.round(sec * S.audio.rate));
}

function wire() {
  S.wave = new WaveView($("wave"), $("overview"), {
    onCursor: (s) => { S.cursor = s; S.wave.setCursor(s); if (S.play) play(s); $("position").textContent = formatTime(s, S.audio.rate) + "  ·  " + s.toLocaleString(); },
    onSet: (which, s) => setMarker(which, s),
    onSelect: (which) => { S.active = which; renderLoopFields(); },
    onDrag: (which, s, final) => setMarker(which, s, { quiet: !final }),
  });
  S.seam = new SeamView($("seam"));
  new ResizeObserver(() => renderSeam()).observe($("seam"));

  $("mod-select").addEventListener("change", (e) => openMod(e.target.value));
  $("new-mod").addEventListener("click", newModDialog);
  $("new-form").addEventListener("submit", createMod);
  $("new-cancel").addEventListener("click", () => $("new-dialog").close());
  $("save").addEventListener("click", save);
  $("add-series").addEventListener("click", addSeries);
  $("file-input").addEventListener("change", (e) => { addFiles(S.addTo, e.target.files); e.target.value = ""; });
  for (const k of ["id", "name", "version", "author", "description"]) {
    $("mod-" + k).addEventListener("input", (e) => { S.model[k] = e.target.value; changed(); });
  }
  $("track-title").addEventListener("input", async (e) => {
    const t = track(); if (!t) return;
    t.title = e.target.value; changed();
    const li = document.querySelectorAll(".series")[S.sel.s]?.querySelectorAll(".track")[S.sel.t];
    if (li) li.querySelector(".t-title").textContent = t.title || "(no title)";
    if (t._autoKey) {
      const r = await api("/api/key", { kind: "track", text: t.title, taken: allTrackKeys(t), mod_id: S.model.id, folder: S.folder });
      if (track() === t && t._autoKey) { t.key = r.key; $("track-key").value = r.key; changed(); }
    }
  });
  $("track-key").addEventListener("input", (e) => { const t = track(); if (!t) return; t.key = e.target.value; t._autoKey = false; changed(); });
  $("track-series").addEventListener("change", (e) => {
    const t = track(), to = Number(e.target.value);
    if (!t || to === S.sel.s) return;
    S.model.series[S.sel.s].tracks.splice(S.sel.t, 1);
    S.model.series[to].tracks.push(t);
    S.sel = { s: to, t: S.model.series[to].tracks.length - 1 };
    changed(); render();
  });
  document.querySelectorAll('input[name="loop-mode"]').forEach((r) => r.addEventListener("change", () => {
    const t = track(); if (!t) return;
    const loop = loopOf(t);
    if (r.value === "custom" && loop.mode !== "custom") {
      const eff = effectiveLoop();
      t.loop = { mode: "custom", start: loop.start ?? (eff.mode === "tags" ? eff.start : 0), end: loop.end ?? (eff.mode === "tags" ? eff.end : (S.audio ? S.audio.n : null)) };
    } else {
      t.loop = Object.assign({}, loop, { mode: r.value });
    }
    changed(); renderLoopFields(); updateTrackBadge();
  }));
  for (const which of ["start", "end"]) {
    const box = $("box-" + which);
    box.querySelector(".mhead").addEventListener("click", () => { S.active = which; renderLoopFields(); });
    box.addEventListener("focusin", () => { if (S.active !== which) { S.active = which; renderLoopFields(); } });
    $(which + "-samples").addEventListener("change", () => commitSamples(which));
    $(which + "-time").addEventListener("change", () => commitTime(which));
    for (const id of [which + "-samples", which + "-time"]) {
      $(id).addEventListener("keydown", (e) => { if (e.key === "Enter") e.target.blur(); });
    }
    box.querySelectorAll("button").forEach((b) => b.addEventListener("click", () => {
      S.active = which;
      const act = b.dataset.act;
      if (act === "set") setMarker(which, here());
      if (act === "nudge") nudge(which, Number(b.dataset.dir));
      if (act === "zero") snapZero(which);
      if (act === "go") { const eff = effectiveLoop(); S.cursor = which === "start" ? eff.start : eff.end; S.wave.setCursor(S.cursor); S.wave.ensureVisible(S.cursor); }
    }));
  }
  $("bpm").addEventListener("change", renderLoopFields);
  $("seam-width").addEventListener("change", renderSeam);
  $("play").addEventListener("click", togglePlay);
  $("play-start").addEventListener("click", () => play(0));
  $("audition").addEventListener("click", () => audition());
  $("zoom-fit").addEventListener("click", () => S.wave.zoomFit());
  $("zoom-loop").addEventListener("click", () => { const e = effectiveLoop(); const pad = (e.end - e.start) * 0.05; S.wave.zoomTo(e.start - pad, e.end + pad); });
  $("zoom-start").addEventListener("click", () => S.audio && S.wave.zoomAround(effectiveLoop().start, S.audio.rate * 2));
  $("zoom-end").addEventListener("click", () => S.audio && S.wave.zoomAround(effectiveLoop().end, S.audio.rate * 2));
  $("zoom-in").addEventListener("click", () => S.wave.zoomBy(0.5));
  $("zoom-out").addEventListener("click", () => S.wave.zoomBy(2));
  $("suggest-any").addEventListener("click", () => runSuggest("any"));
  $("suggest-end").addEventListener("click", () => runSuggest("end"));
  $("copy-command").addEventListener("click", () => navigator.clipboard.writeText($("port-command").textContent).then(() => status("Copied", "ok")));

  // Files dropped anywhere else go to the selected track's series.
  document.addEventListener("dragover", (e) => e.preventDefault());
  document.addEventListener("drop", (e) => { e.preventDefault(); if (S.model && e.dataTransfer.files.length) addFiles(S.sel ? S.sel.s : null, e.dataTransfer.files); });

  document.addEventListener("keydown", (e) => {
    if ((e.ctrlKey || e.metaKey) && e.key.toLowerCase() === "s") { e.preventDefault(); save(); return; }
    const tag = (e.target.tagName || "").toLowerCase();
    if (tag === "input" || tag === "textarea" || tag === "select" || $("new-dialog").open) return;
    if (!S.audio || e.ctrlKey || e.metaKey || e.altKey) return;
    const k = e.key;
    if (k === " ") { e.preventDefault(); togglePlay(); }
    else if (k === "s" || k === "S") { e.preventDefault(); ensureCustom(); setMarker("start", here()); }
    else if (k === "e" || k === "E") { e.preventDefault(); ensureCustom(); setMarker("end", here()); }
    else if (k === "a" || k === "A") { e.preventDefault(); audition(); }
    else if (k === "z" || k === "Z") { e.preventDefault(); snapZero(S.active); }
    else if (k === "[") { S.active = "start"; renderLoopFields(); }
    else if (k === "]") { S.active = "end"; renderLoopFields(); }
    else if (k === "ArrowLeft" || k === "ArrowRight") { e.preventDefault(); nudge(S.active, k === "ArrowLeft" ? -1 : 1, e.shiftKey ? 10 : 1); }
    else if (k === "+" || k === "=") S.wave.zoomBy(0.5);
    else if (k === "-" || k === "_") S.wave.zoomBy(2);
    else if (k === "f" || k === "F") S.wave.zoomFit();
    else if (k === "Home") { e.preventDefault(); play(0); }
    else if (k === "Escape") stop();
  });
  window.addEventListener("beforeunload", (e) => { if (S.dirty) { e.preventDefault(); e.returnValue = ""; } });
}

// S and E on a track without a custom loop start one.
function ensureCustom() {
  const t = track();
  if (!t || loopOf(t).mode === "custom") return;
  t.loop = { mode: "custom", start: 0, end: S.audio.n };
  renderTrackEditor(); updateTrackBadge();
}

async function main() {
  wire();
  if (!TOKEN) { status("No session token: open the address the mod maker printed", "bad"); return; }
  try {
    await refreshMods();
    render();
    const wanted = HASH.get("mod") || (S.state.mods.length === 1 && !S.state.mods[0].error ? S.state.mods[0].folder : null);
    if (wanted && S.state.mods.some((m) => m.folder === wanted)) {
      $("mod-select").value = wanted;
      await openMod(wanted);
      const key = HASH.get("track");
      if (key) S.model.series.forEach((s, si) => s.tracks.forEach((t, ti) => { if (t.key === key) selectTrack(si, ti); }));
    }
  } catch (e) { status("Cannot reach the mod maker: " + e.message, "bad"); }
}

main();
