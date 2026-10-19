# Bounded presentation update experiments

Target: main `func_08873F80`, address `0x08873F80`, 348 native bytes.
All candidates use unchanged SNC `-O2 -g` and the existing private
`main_presentation_process_view.h`. No production, inventory or match-manifest
changes were made. None of these candidates receives source credit.

| Isolated source stem | Compiled bytes | Exact |
| --- | ---: | --- |
| presentation_review_process_labels | 396 | No |
| presentation_review_process_labels_index | 396 | No |
| presentation_review_process_find_reference | 344 | No |
| presentation_review_process_find_outputs | 344 | No |
| presentation_review_process_common_advance | 360 | No |
| presentation_review_process_interpolate | 352 | No |
| presentation_review_process_first_member | 352 | No |
| presentation_review_process_vector_cursor | 348 | No |

Raw objects, compiler logs and comparison JSON remain in
`build/main-source-cluster/<source-stem>/`.

The earlier guarded C form remains closest: 348 bytes with 28 differing
instruction words. The isolated vector-cursor form produces the same mismatch.
Both preserve the native callback/int argument interface and guarded interval
use. C++ interpolation encapsulation changes the empty-interval/marker branch
layout in the native direction, but still emits an extra argument move and
displaces subsequent instructions. Explicit labels inhibit the native loop
strength reduction and substantially enlarge the routine.

The central remaining register difference is the following-record pointer:
native retains it in `$a2`, while the candidates use `$a1`/`$a0` and move it
to `$a2` before interpolation. Loop counter/limit allocation and the placement
of the empty-interval branch also differ. No inference about a different
original compiler profile is justified by these bounded results.

Reproduction command, replacing the source stem as appropriate:

```powershell
.\.venv\Scripts\python.exe tools/probes/main_cluster.py tools/probes/presentation_review_process_interpolate.cpp --function func_08873F80:0x08873F80:348 --symbol D_089D69E0=0x089D69E0 --symbol D_089D5EAC=0x089D5EAC --symbol func_089A39EC=0x089A39EC
```
