# Isolated task-queue source experiment

`task_queue_review_unlink_sizeof_offset.cpp` matches native
`func_088828BC` at `0x088828BC`: all 148 bytes, after normal relocation
resolution with the unchanged SNC `-O2 -g` profile. It is not integrated or
credited by this experiment.

SNC preserves the separate address additions when a slot address is formed
by integer address arithmetic. A direct member-address round trip compiles
to 140 bytes; deriving an offset by subtracting addresses of a real local
object compiles to 168 bytes. `__builtin_offsetof` is unsupported by this SNC
compiler. These failed candidates and their compile/raw-comparison evidence
remain alongside the successful candidate.

The successful candidate uses `__typeof__(sizeof(0))` and checks that this
integer is pointer-width. Offsets come from the known contiguous link suffix
and `sizeof(MainTask)`, rather than hardcoded PSP offsets. Integer/pointer
conversion and arithmetic rely on the flat-address representation of the
PSP32 target and the tested host compiler. This is an explicit target ABI
assumption, not a claim of universal pointer-arithmetic portability.

Both slot helpers are invoked only in nonnull predecessor/successor branches.
No null-member expression is evaluated, no dummy address is dereferenced, and
no object lifetime is invented. Unlink itself requires an existing nonnull
task; changing that precondition would change native behavior.

The host fixture verifies the computed offsets against actual `offsetof`,
empty-list boundaries, single/head/tail/interior removal, pending-cursor
advance, count/link updates before finalization, and preserved auxiliary
links. Eight removal cases pass AddressSanitizer and UndefinedBehaviorSanitizer.

Reproduce the raw comparison from the project directory:

```powershell
.\.venv\Scripts\python.exe tools/probes/main_cluster.py tools/probes/task_queue_review_unlink_sizeof_offset.cpp --function func_088828BC:0x088828BC:148
```

Host fixture build/run:

```powershell
wsl -d Ubuntu-24.04 --cd /mnt/x/path/to/Decomp-Dissidia -- g++ -std=c++17 -O2 '-fsanitize=address,undefined' -fno-sanitize-recover=all -Iinclude tools/probes/task_queue_review_unlink_fixture.cpp -o build/main-source-cluster/task_queue_review_unlink_sizeof_offset/host_fixture
wsl -d Ubuntu-24.04 --cd /mnt/x/path/to/Decomp-Dissidia -- build/main-source-cluster/task_queue_review_unlink_sizeof_offset/host_fixture
```

The exact candidate emits these unused weak inline definitions, which need
the normal separate audit if the source is integrated:

```text
__0fYMainTaskUnlinkOperationsNprevious_slotP6IMainTaskT                       8
__0fYMainTaskUnlinkOperationsOfollowing_slotP6IMainTaskT                      8
__0fYMainTaskUnlinkOperationsGunlinkP6NMainTaskOwnerP6IMainTaskT              92
__0fRMainTaskIterationEnextP6NMainTaskOwnerP6IMainTaskT                       24
```

Registration remains unmatched. Integer slots preserve its native address
additions but yield 304/300 bytes. A nested tail block also yields 304 bytes;
conditional tail retrieval yields 316; unconditional retrieval yields 308.
Remaining differences include tail-load scheduling, allocation of count
temporaries, and the consequent branch displacement. No registration source
credit or production changes are proposed.
