"""Control-flow hazards for static recompilation.

For every module (inventory function ranges):
  * jr through a non-$ra register: match the SNC switch idiom
    (sltiu bound / sll / lui / addu / lw / jr) and read the jump table;
  * jalr: classify the shape of the target computation (C++ virtual call via
    vtable, function-pointer field/global, constant address, other);
  * code pointers in data: words equal to a function entry, runs of them;
  * code addresses materialised in code (lui + addiu/ori pairs);
  * functions with no direct jal/j caller;
  * cross-module calls (overlay -> main, main -> overlay window);
  * $ra used as a general-purpose register (hand-written code);
  * unknown executable bytes from the inventory, with a rough content guess.

Writes build/scratch/port/control_flow.json.
"""

import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402
from common import op, rs, rt, rd, imm, simm, h  # noqa: E402

OVERLAY_WINDOW = (0x089DD3C0, 0x08B63170)  # gap between main's two load segments
MAIN_CODE = (0x08804040, 0x089C60B0)


def last_def(module, address, reg, owner, limit=16):
    """Find the closest earlier instruction in the same function writing reg."""
    for back in range(1, limit + 1):
        a = address - back * 4
        if module.owner_of(a) != owner:
            return None, None
        w = module.word(a)
        o = op(w)
        if o == 0:
            fn = w & 0x3F
            if fn in (0x08, 0x09, 0x0C, 0x0D, 0x18, 0x19, 0x1A, 0x1B):
                if fn == 0x09 and rd(w) == reg:
                    return a, w
                continue
            if rd(w) == reg:
                return a, w
        elif o in (0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17, 0x01):
            if o == 0x03 and reg in (2, 3, 31):
                return a, w
            continue
        elif o >= 0x28 and o != 0x30:
            continue  # stores, cache, FPU/VFPU loads/stores and VFPU ops
        elif o in (0x18, 0x19, 0x1B):
            continue  # VFPU arithmetic
        elif o == 0x1F:
            if ((rd(w) if (w & 0x3F) == 0x20 else rt(w))) == reg:
                return a, w
            continue
        elif o == 0x11 or o == 0x12 or o == 0x10:
            if o == 0x11 and (w >> 21) & 31 == 0 and rt(w) == reg:  # mfc1
                return a, w
            continue
        elif rt(w) == reg:
            return a, w
    return None, None


def is_lw(w):
    return op(w) == 0x23


def jump_tables(module, name):
    results = []
    other = []
    for start, end, owner in module.func_ranges:
        for offset, w in enumerate(module.words(start, end)):
            a = start + offset * 4
            if not (common.is_jr(w) and rs(w) != 31):
                continue
            reg = rs(w)
            da, dw = last_def(module, a, reg, owner)
            table = None
            bound = None
            if dw is not None and is_lw(dw):
                base = rs(dw)
                ba, bw = last_def(module, da, base, owner)
                if bw is not None and op(bw) == 0 and (bw & 0x3F) == 0x21:  # addu
                    for src in (rs(bw), rt(bw)):
                        la, lw_ = last_def(module, ba, src, owner)
                        if lw_ is not None and op(lw_) == 0x0F:
                            table = ((imm(lw_) << 16) + simm(dw)) & 0xFFFFFFFF
                            break
                # bound: look for sltiu within 12 instructions before
                for back in range(1, 13):
                    x = module.word(a - back * 4)
                    if x is not None and op(x) == 0x0B:
                        bound = imm(x)
                        break
            if table is None:
                kind = "register"
                if dw is not None and is_lw(dw):
                    kind = "loaded pointer (tail call or computed goto)"
                other.append({"site": h(a), "function": owner, "reg": common.REG[reg], "kind": kind})
                continue
            entries = []
            count = bound if bound else 0
            limit = count if count else 4096
            for index in range(limit):
                target = module.word(table + index * 4)
                if target is None:
                    tgt_mod = None
                    break
                if not count and module.owner_of(target) != owner:
                    break
                entries.append(target)
            inside = sum(1 for t in entries if module.owner_of(t) == owner)
            results.append({"site": h(a), "function": owner, "table": h(table), "bound": bound,
                            "entries": len(entries), "targets_inside_function": inside,
                            "table_in_rodata": not module.owner_of(table)})
    return results, other


def classify_jalr(module):
    shapes = Counter()
    examples = defaultdict(list)
    for start, end, owner in module.func_ranges:
        for offset, w in enumerate(module.words(start, end)):
            a = start + offset * 4
            if not common.is_jalr(w):
                continue
            reg = rs(w)
            da, dw = last_def(module, a, reg, owner)
            shape = "unknown (def not found in 16 instrs)"
            if dw is not None:
                if is_lw(dw):
                    base = rs(dw)
                    if base == 29:
                        shape = "pointer spilled on stack"
                    elif base == 28:
                        shape = "gp-relative global"
                    else:
                        ba, bw = last_def(module, da, base, owner)
                        # SNC vtable slot: {s16 this-delta, s16 pad, fn*}; the call site loads
                        # lh delta, -4(slot) next to lw fn, 0(slot) and adds delta to this.
                        delta = False
                        for back in range(1, 12):
                            x = module.word(da - back * 4)
                            if x is not None and op(x) == 0x21 and rs(x) == base and simm(x) == simm(dw) - 4:
                                delta = True
                                break
                        if delta:
                            shape = "virtual call (SNC vtable slot: lh delta + lw fn)"
                        elif bw is not None and is_lw(bw):
                            # object->vtable->slot : vtable pointer loaded from an object
                            shape = "virtual call (load vtable, load slot)"
                        elif bw is not None and op(bw) == 0x0F:
                            shape = "global function pointer (lui + lw)"
                        elif bw is not None and op(bw) == 0 and (bw & 0x3F) == 0x21:
                            shape = "indexed function-pointer table"
                        else:
                            shape = "function pointer field of object/argument"
                elif op(dw) in (0x09, 0x0D):
                    shape = "constant address (lui + addiu/ori)"
                elif op(dw) == 0 and (dw & 0x3F) in (0x21, 0x25) and rt(dw) == 0:
                    shape = "register copy (function pointer argument)"
                else:
                    shape = f"computed ({common.decode(dw)})"
            shapes[shape] += 1
            if len(examples[shape]) < 5:
                examples[shape].append(h(a))
    return dict(shapes.most_common()), dict(examples)


def data_words(module):
    """Yield (address, word) for initialized words that are not decoded instructions."""
    code = sorted(module.iter_ranges("instruction_ranges"))
    for start, data, executable in module.chunks:
        end = start + len(data)
        a = (start + 3) & ~3
        ci = 0
        while a + 4 <= end:
            while ci < len(code) and code[ci][1] <= a:
                ci += 1
            if ci < len(code) and code[ci][0] <= a < code[ci][1]:
                a = code[ci][1]
                continue
            yield a, struct.unpack_from("<I", data, a - start)[0]
            a += 4


def main():
    modules = common.load_all()
    report = {"modules": {}}
    starts = {name: m.func_starts for name, m in modules.items()}
    direct_targets = defaultdict(set)  # (module, address) targets of jal/j
    materialised = defaultdict(set)
    cross = Counter()
    cross_targets = defaultdict(Counter)
    ra_gpr = defaultdict(set)
    for name, module in modules.items():
        for start, end, owner in module.func_ranges:
            words = module.words(start, end)
            lui = {}
            for offset, w in enumerate(words):
                a = start + offset * 4
                o = op(w)
                if o in (2, 3):
                    t = common.jal_target(w, a)
                    in_main = MAIN_CODE[0] <= t < MAIN_CODE[1]
                    in_ovl = OVERLAY_WINDOW[0] <= t < OVERLAY_WINDOW[1]
                    if name == "main":
                        if in_ovl:
                            cross["main->overlay " + ("jal" if o == 3 else "j")] += 1
                            cross_targets["main->overlay"][h(t)] += 1
                            for ov in ("menu", "battle", "extra"):
                                if t in starts[ov]:
                                    direct_targets[(ov, t)].add(h(a))
                        else:
                            direct_targets[("main", t)].add(h(a))
                    else:
                        if in_main:
                            cross[f"{name}->main " + ("jal" if o == 3 else "j")] += 1
                            direct_targets[("main", t)].add(h(a))
                        else:
                            direct_targets[(name, t)].add(h(a))
                if o == 0x0F:
                    lui[rt(w)] = imm(w) << 16
                elif o in (0x09, 0x0D) and rs(w) in lui:
                    value = (lui[rs(w)] + simm(w)) & 0xFFFFFFFF if o == 0x09 else lui[rs(w)] | imm(w)
                    for mod_name in ([name, "main"] if name != "main" else ["main", "menu", "battle", "extra"]):
                        if value in starts[mod_name] and value != start:
                            materialised[(mod_name, value)].add(h(a))
                # $ra written by something other than jal/jalr/lw-from-stack
                writes_ra = False
                if o == 0 and rd(w) == 31 and (w & 0x3F) not in (0x08, 0x09, 0x18, 0x19, 0x1A, 0x1B, 0x0C, 0x0D):
                    writes_ra = True
                elif o in (0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F) and rt(w) == 31:
                    writes_ra = True
                elif o == 0x23 and rt(w) == 31 and rs(w) != 29:
                    writes_ra = True
                if writes_ra:
                    ra_gpr[name].add(owner)
    for name, module in modules.items():
        tables, other_jr = jump_tables(module, name)
        jalr_shapes, jalr_examples = classify_jalr(module)
        # code pointers in data
        ptrs = []
        own_starts = starts[name]
        for a, w in data_words(module):
            if w in own_starts or (name != "main" and w in starts["main"]):
                ptrs.append((a, w))
        runs = []
        run = []
        for a, w in ptrs:
            if run and a == run[-1][0] + 4:
                run.append((a, w))
            else:
                if len(run) >= 2:
                    runs.append(run)
                run = [(a, w)]
        if len(run) >= 2:
            runs.append(run)
        jt_targets = set()
        for t in tables:
            pass
        data_ptr_targets = {w for _, w in ptrs}
        functions = [int(f["address"], 16) for f in module.functions]
        imports = set()
        if name == "main":
            imports = {int(f["address"], 16) for f in module.functions if f.get("primary_owner") == "platform_imports"}
        no_direct = [f for f in functions if (name, f) not in direct_targets and f not in imports]
        only_data = [f for f in no_direct if f in data_ptr_targets or any(f == w for _, w in ptrs)]
        only_mat = [f for f in no_direct if (name, f) in materialised and f not in data_ptr_targets]
        neither = [f for f in no_direct if f not in data_ptr_targets and (name, f) not in materialised]
        # unknown bytes
        unknown = list(module.iter_ranges("unclassified_executable_ranges"))
        sizes = [e - s for s, e in unknown]
        zero_words = valid_words = ptr_words = total_words = 0
        for s, e in unknown:
            for a in range((s + 3) & ~3, e - 3, 4):
                w = module.word(a)
                if w is None:
                    continue
                total_words += 1
                if w == 0:
                    zero_words += 1
                elif 0x08800000 <= w < 0x0A000000:
                    ptr_words += 1
        # code pointers in this module's own data that land in unknown bytes on
        # something that looks like a function entry (stack-frame prologue or a
        # bare `jr ra`): functions the inventory has not found yet.
        unknown_sorted = sorted(unknown)

        def in_unknown(value):
            lo, hi = 0, len(unknown_sorted)
            while lo < hi:
                mid = (lo + hi) // 2
                if unknown_sorted[mid][0] <= value:
                    lo = mid + 1
                else:
                    hi = mid
            return lo > 0 and unknown_sorted[lo - 1][0] <= value < unknown_sorted[lo - 1][1]

        def entry_like(value):
            x = module.word(value)
            return x is not None and (((x >> 16) == 0x27BD and x & 0x8000) or x == 0x03E00008)

        hidden = sorted({w for _, w in data_words(module) if w % 4 == 0 and in_unknown(w) and entry_like(w)})
        size_hist = Counter()
        for size in sizes:
            bucket = "<=16" if size <= 16 else "<=64" if size <= 64 else "<=256" if size <= 256 else "<=4K" if size <= 4096 else ">4K"
            size_hist[bucket] += 1
        report["modules"][name] = {
            "jump_tables": {"count": len(tables), "entries": sum(t["entries"] for t in tables),
                            "with_bound": sum(1 for t in tables if t["bound"]),
                            "all_targets_inside_function": sum(1 for t in tables if t["targets_inside_function"] == t["entries"]),
                            "tables": tables},
            "jr_other": {"count": len(other_jr), "sites": other_jr},
            "jalr": {"total": sum(jalr_shapes.values()), "shapes": jalr_shapes, "examples": jalr_examples},
            "code_pointers_in_data": {"words": len(ptrs), "distinct_targets": len(data_ptr_targets),
                                       "runs_of_2_or_more": len(runs),
                                       "words_in_runs": sum(len(r) for r in runs),
                                       "largest_runs": sorted(([h(r[0][0]), len(r)] for r in runs), key=lambda x: -x[1])[:15],
                                       "pointing_into_main": sum(1 for _, w in ptrs if name != "main" and w in starts["main"])},
            "functions": len(functions),
            "functions_without_direct_caller": len(no_direct),
            "of_which_address_in_data": len(only_data),
            "of_which_address_built_in_code_only": len(only_mat),
            "of_which_no_reference_found": len(neither),
            "no_reference_examples": [h(f) for f in neither[:40]],
            "functions_using_ra_as_gpr": sorted(ra_gpr[name]),
            "likely_functions_in_unknown_bytes_referenced_from_data": len(hidden),
            "likely_hidden_function_examples": [h(v) for v in hidden[:40]],
            "unknown_executable_bytes": {"bytes": sum(sizes), "ranges": len(unknown),
                                         "size_histogram": dict(size_hist), "words": total_words,
                                         "zero_words": zero_words, "words_that_look_like_ram_pointers": ptr_words,
                                         "largest": sorted(([h(s), e - s] for s, e in unknown), key=lambda x: -x[1])[:15]},
        }
    report["cross_module"] = {"counts": dict(cross),
                              "main_to_overlay_targets": dict(cross_targets["main->overlay"])}
    # which overlays define each main->overlay target
    defined = {}
    for t in cross_targets["main->overlay"]:
        v = int(t, 16)
        defined[t] = [ov for ov in ("menu", "battle", "extra") if v in starts[ov]]
    report["cross_module"]["main_to_overlay_defined_in"] = defined
    unresolved = {}
    for t, owners in defined.items():
        if owners:
            continue
        v = int(t, 16)
        where = {}
        for ov in ("menu", "battle", "extra"):
            m = modules[ov]
            if not m.contains(v):
                continue
            if m.owner_of(v):
                where[ov] = "inside function " + m.owner_of(v)
            elif any(s <= v < e for s, e in m.iter_ranges("unclassified_executable_ranges")):
                where[ov] = "unknown bytes"
            else:
                where[ov] = "decoded, outside functions"
        unresolved[t] = where
    report["cross_module"]["main_to_overlay_unresolved"] = unresolved
    report["cross_module"]["main_to_overlay_unresolved_summary"] = dict(
        Counter(f"{ov}: {kind.split(' 0x')[0]}" for where in unresolved.values() for ov, kind in where.items()))
    # main data words that hold overlay-window code addresses (e.g. main vtables
    # whose slots are implemented in an overlay)
    main_data_ovl = Counter()
    for a, w in data_words(modules["main"]):
        if OVERLAY_WINDOW[0] <= w < OVERLAY_WINDOW[1]:
            owners = tuple(ov for ov in ("menu", "battle", "extra") if w in starts[ov])
            if owners:
                main_data_ovl["+".join(owners)] += 1
    report["cross_module"]["main_data_words_naming_overlay_functions"] = dict(main_data_ovl)
    # vtable-like runs in data: pairs {small signed delta word, code pointer}
    for name, module in modules.items():
        code = module.func_starts | (starts["main"] if name != "main" else set())
        words = list(data_words(module))
        runs = slots = longest = 0
        i = 0
        while i + 1 < len(words):
            k, count = i, 0
            while (k + 1 < len(words) and words[k + 1][0] == words[k][0] + 4
                   and (words[k][1] & 0xFFFF0000) in (0, 0xFFFF0000) and words[k + 1][1] in code):
                count += 1
                k += 2
            if count >= 2:
                runs += 1
                slots += count
                longest = max(longest, count)
                i = k
            else:
                i += 1
        report["modules"][name]["vtable_like_runs"] = {"runs": runs, "slots": slots, "longest": longest}
    path = common.write_json("control_flow.json", report)
    for name, r in report["modules"].items():
        jt = r["jump_tables"]
        print(f"{name:7s} jump tables {jt['count']:4d} ({jt['entries']} entries, bounded {jt['with_bound']}, "
              f"all-inside {jt['all_targets_inside_function']}) jr-other {r['jr_other']['count']} "
              f"jalr {r['jalr']['total']} data code-ptrs {r['code_pointers_in_data']['words']} "
              f"(runs {r['code_pointers_in_data']['runs_of_2_or_more']}) no-direct-caller {r['functions_without_direct_caller']} "
              f"[data {r['of_which_address_in_data']}, code-addr {r['of_which_address_built_in_code_only']}, none {r['of_which_no_reference_found']}] "
              f"ra-as-gpr fns {len(r['functions_using_ra_as_gpr'])}")
        print("   jalr shapes:", r["jalr"]["shapes"])
        print("   unknown:", {k: v for k, v in r["unknown_executable_bytes"].items() if k != "largest"})
    print("cross:", report["cross_module"]["counts"])
    print("main->overlay targets:", len(defined), Counter(tuple(v) for v in defined.values()))
    print(f"-> {path}")


if __name__ == "__main__":
    main()
