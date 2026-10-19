"""Summarise a PortableKit display-list dump (<PREFIX>_TRACE_GE_LIST) draw by draw.

    python tools/port/ge_list_summary.py LOG [--run N] [--detail]

Reads the "[ge-list] 0xADDR 0xWORD" lines of one list run and prints, for every
primitive and block transfer, the state it uses: frame buffer and depth buffer
addresses and widths, pixel format, clear mode, texture address, size and
format, vertex type (through mode or transformed), and the enables that matter
for battles (depth test, stencil, alpha blend). Consecutive draws with the same
state are folded into one line with a count. Command numbers are the GE's as
published in the PSP SDK documentation; the decoding is only what is printed.

--detail adds the raw words of the registers around the stencil effects: depth
function (0xDE), blend mode and fixed colours (0xDF-0xE1), texture function
(0xC9), alpha test (0x22, 0xDB), colour test (0x27, 0xD8-0xDA), logic op (0x28,
0xE6), dither (0x20) and the other enables the renderer ignores (0x1C, 0x25,
0x26), each printed only when its enable (if any) is on.
"""

import argparse
import re
import sys

PRIMS = ["points", "lines", "linestrip", "tris", "strip", "fan", "sprites"]
TEX_FORMATS = ["565", "5551", "4444", "8888", "clut4", "clut8", "clut16", "clut32", "dxt1", "dxt3", "dxt5"]
FB_FORMATS = ["565", "5551", "4444", "8888"]


def runs(path):
    current, lines = None, []
    pattern = re.compile(r"\[ge-list\] run (\d+)")
    word = re.compile(r"\[ge-list\] 0x([0-9A-Fa-f]{8}) 0x([0-9A-Fa-f]{8})")
    with open(path, encoding="utf-8", errors="replace") as log:
        for line in log:
            m = pattern.search(line)
            if m:
                if current is not None:
                    yield current, lines
                current, lines = int(m.group(1)), []
                continue
            m = word.search(line)
            if m and current is not None:
                lines.append((int(m.group(1), 16), int(m.group(2), 16)))
    if current is not None:
        yield current, lines


DETAIL = [0x1D, 0x9B, 0xDE, 0xDF, 0xE0, 0xE1, 0xC9, 0x22, 0xDB, 0x27, 0xD8, 0xD9, 0xDA, 0x28, 0xE6, 0x20, 0x1C, 0x25, 0x26]


def detail(s):
    r = s["raw"]
    on = lambda c: r.get(c, 0) & 1
    out = [f"ztf={r.get(0xDE, 0):X}"] + ([f"cull={r.get(0x9B, 0):X}"] if on(0x1D) else [])
    if s["abe"]:
        out.append(f"bm={r.get(0xDF, 0):06X}")
        if (r.get(0xDF, 0) & 0xF) == 10 or ((r.get(0xDF, 0) >> 4) & 0xF) == 10:
            out.append(f"fix={r.get(0xE0, 0):06X}/{r.get(0xE1, 0):06X}")
    if s["tme"]:
        out.append(f"tfx={r.get(0xC9, 0):06X}")
    if on(0x22):
        out.append(f"atest={r.get(0xDB, 0):06X}")
    if on(0x27):
        out.append(f"ctest={r.get(0xD8, 0):X},{r.get(0xD9, 0):06X},{r.get(0xDA, 0):06X}")
    if on(0x28):
        out.append(f"logic={r.get(0xE6, 0):X}")
    for c, name in ((0x20, "dither"), (0x1C, "clip"), (0x25, "e25"), (0x26, "e26")):
        if on(c):
            out.append(name)
    return " ".join(out)


def summarise(words, with_detail=False):
    s = dict(fbp=0, fbw=0, zbp=0, zbw=0, fpsm=0, tbp=0, tbw=0, tsize=0, tpsm=0, clear=0, vtype=0,
             zte=0, ste=0, abe=0, tme=0, region=0, scissor=(0, 0), stst=0, sop=0, zmsk=0, pmsk=(0, 0),
             cbp=0, offset=(0, 0), viewport=(0, 0, 0, 0), raw={})
    trx = dict(sbp=0, sbw=0, dbp=0, dbw=0, size=0, spos=0, dpos=0)
    out = []
    for address, word in words:
        cmd, data = word >> 24, word & 0xFFFFFF
        if cmd in DETAIL: s["raw"][cmd] = data
        if cmd == 0x9C: s["fbp"] = (s["fbp"] & 0xFF000000) | data
        elif cmd == 0x9D: s["fbw"] = data & 0xFFFF; s["fbp"] = (s["fbp"] & 0x00FFFFFF) | ((data >> 16) << 24)
        elif cmd == 0x9E: s["zbp"] = (s["zbp"] & 0xFF000000) | data
        elif cmd == 0x9F: s["zbw"] = data & 0xFFFF; s["zbp"] = (s["zbp"] & 0x00FFFFFF) | ((data >> 16) << 24)
        elif cmd == 0xD2: s["fpsm"] = data & 3
        elif cmd == 0xA0: s["tbp"] = (s["tbp"] & 0xFF000000) | data
        elif cmd == 0xA8: s["tbw"] = data & 0xFFFF; s["tbp"] = (s["tbp"] & 0x00FFFFFF) | ((data >> 16) << 24)
        elif cmd == 0xB8: s["tsize"] = data
        elif cmd == 0xC3: s["tpsm"] = data & 0xF
        elif cmd == 0xD3: s["clear"] = data
        elif cmd == 0x12: s["vtype"] = data
        elif cmd == 0x23: s["zte"] = data & 1
        elif cmd == 0x24: s["ste"] = data & 1
        elif cmd == 0x21: s["abe"] = data & 1
        elif cmd == 0x1E: s["tme"] = data & 1
        elif cmd == 0xDC: s["stst"] = data
        elif cmd == 0xDD: s["sop"] = data
        elif cmd == 0xE7: s["zmsk"] = data & 1
        elif cmd == 0xE8: s["pmsk"] = (data, s["pmsk"][1])
        elif cmd == 0xE9: s["pmsk"] = (s["pmsk"][0], data)
        elif cmd == 0xD4: s["scissor"] = (data, s["scissor"][1])
        elif cmd == 0xD5: s["scissor"] = (s["scissor"][0], data)
        elif cmd == 0x4C: s["offset"] = (data >> 4, s["offset"][1])
        elif cmd == 0x4D: s["offset"] = (s["offset"][0], data >> 4)
        elif cmd == 0xB2: trx["sbp"] = (trx["sbp"] & 0xFF000000) | data
        elif cmd == 0xB3: trx["sbw"] = data & 0xFFFF; trx["sbp"] = (trx["sbp"] & 0x00FFFFFF) | ((data >> 16) << 24)
        elif cmd == 0xB4: trx["dbp"] = (trx["dbp"] & 0xFF000000) | data
        elif cmd == 0xB5: trx["dbw"] = data & 0xFFFF; trx["dbp"] = (trx["dbp"] & 0x00FFFFFF) | ((data >> 16) << 24)
        elif cmd == 0xEB: trx["spos"] = data
        elif cmd == 0xEC: trx["dpos"] = data
        elif cmd == 0xEE: trx["size"] = data
        elif cmd == 0xEA:
            w, h = (trx["size"] & 0x3FF) + 1, ((trx["size"] >> 10) & 0x3FF) + 1
            out.append(f"TRANSFER {trx['sbp']:08X}/{trx['sbw']} ({trx['spos'] & 0x3FF},{trx['spos'] >> 10}) -> "
                       f"{trx['dbp']:08X}/{trx['dbw']} ({trx['dpos'] & 0x3FF},{trx['dpos'] >> 10}) {w}x{h} "
                       f"{'32' if data & 1 else '16'}bpp")
        elif cmd == 0x04:
            prim, count = (data >> 16) & 7, data & 0xFFFF
            through = (s["vtype"] >> 23) & 1
            tw, th = 1 << (s["tsize"] & 0xF), 1 << ((s["tsize"] >> 8) & 0xF)
            tex = (f"tex {s['tbp']:08X}/{s['tbw']} {tw}x{th} {TEX_FORMATS[s['tpsm']] if s['tpsm'] < 11 else s['tpsm']}"
                   if s["tme"] else "notex")
            sx1, sy1 = s["scissor"][0] & 0x3FF, (s["scissor"][0] >> 10) & 0x3FF
            sx2, sy2 = s["scissor"][1] & 0x3FF, (s["scissor"][1] >> 10) & 0x3FF
            flags = []
            if s["clear"] & 1: flags.append(f"CLEAR{s['clear'] >> 8:x}")
            if s["zte"]: flags.append("z")
            if s["zmsk"]: flags.append("zmask")
            if s["ste"]: flags.append(f"stencil({s['stst']:06X},{s['sop']:06X})")
            if s["abe"]: flags.append("blend")
            if s["pmsk"] != (0, 0): flags.append(f"pmask({s['pmsk'][0]:06X},{s['pmsk'][1]:06X})")
            out.append(f"{PRIMS[prim]:9} n={count:<5} fb {s['fbp']:08X}/{s['fbw']} {FB_FORMATS[s['fpsm']]} "
                       f"z {s['zbp']:08X}/{s['zbw']} {'through' if through else '3d'} {tex} "
                       f"sc({sx1},{sy1})-({sx2},{sy2}) {' '.join(flags)}"
                       + (f" | {detail(s)}" if with_detail else ""))
    return out


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("log")
    parser.add_argument("--run", type=int)
    parser.add_argument("--detail", action="store_true")
    options = parser.parse_args()
    for run, words in runs(options.log):
        if options.run is not None and run != options.run:
            continue
        print(f"== run {run}: {len(words)} words")
        folded, previous, count = [], None, 0
        for line in summarise(words, options.detail):
            key = re.sub(r"n=\d+\s*", "", line)
            if key == previous:
                count += 1
                continue
            if previous is not None:
                folded.append(f"{count:4}x {last}")
            previous, last, count = key, line, 1
        if previous is not None:
            folded.append(f"{count:4}x {last}")
        print("\n".join(folded))
    return 0


if __name__ == "__main__":
    sys.exit(main())
