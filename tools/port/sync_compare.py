"""Compare two D012_SYNC_TRACE files (port/host/d012_sync.cpp): online play, phase 0.

    python tools/port/sync_compare.py A.bin B.bin [--align battle|boot] [--pages N] [--ignore ADDR-ADDR ...]

Each trace has one record per game pad read: the inputs both players' pads gave the game and a hash of every
64 KiB page of guest RAM. The two runs are lined up at their first read with a battle loaded and versus on
(--align battle, the default) or at their first read (--align boot). The report:

- where each run's battle starts, and how many reads each has from there;
- the first aligned read whose inputs differ (if the inputs differ, the runs were not fed the same thing, and
  any later state difference may only be that);
- the first aligned read whose memory differs, and which pages differ there;
- for every page that ever differs, how many aligned reads it differs in and when it first does, so that pages
  that always differ (time stamps, audio, host-side buffers) stand apart from a divergence that starts and spreads.

--ignore leaves address ranges out of the "first read whose memory differs" (e.g. pages found to hold only
time stamps), so the next difference shows.
"""

import argparse
import struct
import sys

RECORD_HEAD = struct.Struct("<QII4BIII")


def load(path):
    with open(path, "rb") as f:
        data = f.read()
    if data[:8] != b"D012SYNC":
        sys.exit(f"{path}: not a sync trace")
    version, pages, page_size, first = struct.unpack_from("<IIII", data, 8)
    if version != 1:
        sys.exit(f"{path}: version {version}")
    offset = 24
    size = RECORD_HEAD.size + 8 * pages
    records = []
    while offset + size <= len(data):
        read, flags, b1, lx, ly, rx, ry, stamp, b2, stick2 = RECORD_HEAD.unpack_from(data, offset)
        hashes = struct.unpack_from(f"<{pages}Q", data, offset + RECORD_HEAD.size)
        records.append({
            "read": read,
            "flags": flags,
            "p1": (b1, lx, ly, rx, ry),
            "stamp": stamp,
            "p2": (b2, stick2 & 0xFFFF),
            "hashes": hashes,
        })
        offset += size
    return {"pages": pages, "page_size": page_size, "first": first, "records": records}


def battle_start(trace):
    for i, r in enumerate(trace["records"]):
        if r["flags"] & 3 == 3:
            return i
    return None


def page_range(trace, page):
    start = trace["first"] + page * trace["page_size"]
    return f"0x{start:08X}-0x{start + trace['page_size'] - 1:08X}"


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("a")
    parser.add_argument("b")
    parser.add_argument("--align", choices=("battle", "boot"), default="battle")
    parser.add_argument("--pages", type=int, default=40, help="most pages to list")
    parser.add_argument("--ignore", action="append", default=[], metavar="ADDR-ADDR")
    options = parser.parse_args()

    a, b = load(options.a), load(options.b)
    if (a["pages"], a["page_size"], a["first"]) != (b["pages"], b["page_size"], b["first"]):
        sys.exit("the traces cover different memory")
    ignore = []
    for item in options.ignore:
        lo, _, hi = item.partition("-")
        ignore.append((int(lo, 16), int(hi, 16)))

    def ignored(page):
        start = a["first"] + page * a["page_size"]
        return any(lo <= start <= hi for lo, hi in ignore)

    print(f"A: {len(a['records'])} reads; B: {len(b['records'])} reads")
    if options.align == "battle":
        sa, sb = battle_start(a), battle_start(b)
        print(f"battle starts at read {sa} in A and {sb} in B")
        if sa is None or sb is None:
            sys.exit("no battle in one of the runs")
    else:
        sa = sb = 0
    ra, rb = a["records"][sa:], b["records"][sb:]
    n = min(len(ra), len(rb))
    print(f"comparing {n} aligned reads")

    first_input = next((k for k in range(n) if ra[k]["p1"] != rb[k]["p1"] or ra[k]["p2"] != rb[k]["p2"]), None)
    if first_input is None:
        print("inputs: the same at every aligned read")
    else:
        print(f"inputs: first differ at aligned read {first_input}: A p1 {ra[first_input]['p1']} p2 "
              f"{ra[first_input]['p2']}, B p1 {rb[first_input]['p1']} p2 {rb[first_input]['p2']}")

    pages = a["pages"]
    counts = [0] * pages
    first_seen = [None] * pages
    first_diff = None
    for k in range(n):
        ha, hb = ra[k]["hashes"], rb[k]["hashes"]
        for p in range(pages):
            if ha[p] != hb[p]:
                counts[p] += 1
                if first_seen[p] is None:
                    first_seen[p] = k
                if first_diff is None and not ignored(p):
                    first_diff = k
    if first_diff is None:
        print("memory: the same at every aligned read" + (" (outside the ignored ranges)" if ignore else ""))
    else:
        there = [p for p in range(pages) if ra[first_diff]["hashes"][p] != rb[first_diff]["hashes"][p]
                 and not ignored(p)]
        print(f"memory: first differs at aligned read {first_diff} (A read {sa + first_diff}, B read "
              f"{sb + first_diff}) in {len(there)} pages: " + ", ".join(page_range(a, p) for p in there[:12]))
    differing = sorted((p for p in range(pages) if counts[p]), key=lambda p: (first_seen[p], p))
    print(f"{len(differing)} of {pages} pages differ at some aligned read:")
    for p in differing[:options.pages]:
        print(f"  {page_range(a, p)}: differs in {counts[p]} of {n} reads, first at {first_seen[p]}"
              + ("  (ignored)" if ignored(p) else ""))
    if len(differing) > options.pages:
        print(f"  ... and {len(differing) - options.pages} more")


if __name__ == "__main__":
    main()
