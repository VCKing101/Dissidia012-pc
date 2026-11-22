"""Compare two D012_SYNC_TRACE files read by read, keeping the last run of each read.

    python tools/port/sync_last_run.py A.bin B.bin

A run with rollback (D012_ROLLBACK_TEST, or online) writes a record each time it runs a pad read again, so its
records no longer pair one for one with a plain run's, which sync_compare.py expects. This keeps the last record of
each read number in both traces (the run that stood) and compares those: how many reads both have, how many in a
versus battle, how many records were runs again, and which 64 KiB pages differ and from which read.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sync_compare  # noqa: E402


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    a, b = sync_compare.load(sys.argv[1]), sync_compare.load(sys.argv[2])
    last_a = {r["read"]: r for r in a["records"]}
    last_b = {r["read"]: r for r in b["records"]}
    reruns = len(a["records"]) - len(last_a) + len(b["records"]) - len(last_b)
    common = sorted(set(last_a) & set(last_b))
    battle = [n for n in common if last_a[n]["flags"] & 3 == 3]
    print(f"A: {len(last_a)} reads; B: {len(last_b)} reads; {reruns} records of reads run again; "
          f"{len(common)} in both, {len(battle)} of them in a versus battle")
    input_diff = [n for n in common if (last_a[n]["p1"], last_a[n]["p2"]) != (last_b[n]["p1"], last_b[n]["p2"])]
    print(f"reads whose inputs differ: {len(input_diff)}" + (f" (first {input_diff[0]})" if input_diff else ""))
    pages = {}
    differ = []
    for n in common:
        ha, hb = last_a[n]["hashes"], last_b[n]["hashes"]
        if ha == hb:
            continue
        differ.append(n)
        for p, (x, y) in enumerate(zip(ha, hb)):
            if x != y:
                pages.setdefault(p, []).append(n)
    print(f"reads whose memory differs: {len(differ)} of {len(common)}" + (f" (first at read {differ[0]})" if differ else ""))
    for p, reads in sorted(pages.items(), key=lambda kv: kv[1][0])[:30]:
        print(f"  {sync_compare.page_range(a, p)}: {len(reads)} reads, first {reads[0]}")


if __name__ == "__main__":
    main()
