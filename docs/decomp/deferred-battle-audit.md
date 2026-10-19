# Deferred battle setup and callback registration

11 October 2026. The deferred setup path establishes **battle:0x089DD4D0** as a called entry. Fresh analysis adds **24 battle function candidates / 1,580 body bytes**, including that 100-byte prefix. Twenty-two additions have checked local call paths; two unrelated automatic discoveries remain explicitly untraced and provisional. No matching C credit is added.

## Callback chain

Main `0x08889694` prepares three callback pointers and calls `0x0886503C` with a callback block at controller offset `+4`. The setter stores them at block offsets `+0x18`, `+0x1C` and `+0x20`. Thus their offsets in the enclosing controller are `+0x1C`, `+0x20` and `+0x24`.

| Phase | Registered wrapper | Main handler | Callback runner |
| --- | --- | --- | --- |
| Setup | `0x08889098` | `0x0888C4C8` | `0x08864F78` |
| Update | `0x088890B8` | `0x0888DFA4` | `0x08864FA0` |
| Teardown | `0x088890D8` | `0x08888F90` | `0x08864FC8` |

Each wrapper loads the controller from main global `0x089D6A10` and directly calls its handler. Startup calls controller constructor `0x088881FC` and stores the resulting controller pointer at that global. The controller constructor builds the callback block using main `0x08864FF0`, which installs member table `0x089C9348` at block offset `+0x30`.

Main dispatcher `0x08865278` uses the table's records at `+8`, `+16` and `+24`. Each is an eight-byte `{s16 adjustment, s16 flags, u32 callback}` record. Their adjustment and flags are zero; their callback pointers identify the three runners above. Each runner loads its registered callback slot, checks for null and invokes it indirectly. The validator checks the pointer materialization, stores, member records, dispatch instructions and indirect calls.

The phase names describe this observed state-machine order. Original source names and detailed gameplay ownership remain unknown.

## The battle mode barrier

Setup handler `main:0x0888C4C8` begins with a branch-free stack/register prologue. Its first direct call, at `0x0888C4FC`, invokes `main:0x08933C28`; the delay slot loads the overlay-manager pointer from main global `0x089D85E8`.

The 44-byte helper reads the manager's signed mode byte. It returns when mode **1** is active; otherwise it calls the previously reviewed loader `main:0x08933ACC` with mode **1**. The whole helper, including branch target, call argument and return delay slot, is checked against the pinned binary. The main filename dispatch audit binds mode 1 to `ovl_battle_app.elf`.

Setup subsequently calls overlay address `0x089DD4D0` at `0x0888D0B0`. This is the **battle** prefix, separate from menu and extra functions at the same address. The setup body contains **60 direct overlay call sites / 58 distinct targets** after its ensure call. No other direct call into the reviewed overlay-manager transition interval occurs in this body.

This is a static audit. It does not establish that every branch runs in a tested gameplay scenario, nor prove effects of all intervening main helpers or indirect calls. Update and teardown registrations are recorded but do not provide additional battle roots: their overlay-state effects remain unverified.

## Prefix and newly recovered functions

`battle:0x089DD4D0` occupies `0x089DD4D0..0x089DD534` (end exclusive). It calls seven local helpers with pointers to subobjects at offsets `+0x10`, `+0x18`, `+0x1DC`, `+0xA44`, `+0xA8C`, `+0x1C6C` and `+0x1D04`, calls one more helper with the original object pointer, and returns that pointer. The final `jr ra` / stack-restoring delay slot immediately precede the next existing function. This supports a constructor-like interpretation; field types and original class names are unresolved.

Fresh seeded analysis uses a separate Ghidra project, preserving previous projects and annotations. Existing function ranges are unchanged. The new inventory has **6,267 battle candidates**, and unknown battle section bytes decrease by **1,812**. Most body boundaries remain automatic and provisional.

Two 56-byte candidates, `battle:0x089E3F08` and `battle:0x089E44C0`, install a member-table pointer at object offset `+0x30`, call a local helper, and return the object pointer. They were found during automatic analysis, but this audit found no direct `J`/`JAL` caller to them or exact pointer materialization in main/battle executable bytes. They are retained as provisional candidates with **no dispatch reachability claim**. Their function-like bodies do not prove runtime entry or full provenance.

## Reproduce and validate

`config/deferred-battle.json` contains addresses and callback offsets only. `tools/audit_deferred_battle.py` reads the pinned main reference; `tools/prepare_dispatch_entries.py` adds the checked setup targets to battle's seed metadata. `tools/build_inventory.py` validates the deferred path before recording roots and local call reachability.

```powershell
.\.venv\Scripts\python.exe tools/prepare_dispatch_entries.py
.\.venv\Scripts\python.exe tools/build_inventory.py
.\.venv\Scripts\python.exe -m unittest discover -s tools -p test_deferred_battle.py
```

The twelve targeted checks cover the real path and reject an aliased callback, wrong setter slot, missing null guard, changed member record/table, wrong ensure mode, a branch around the barrier, wrong manager argument, wrong module binding, a second direct overlay transition and relabeled phases. Existing inventory/build/progress checks should also pass before reporting project-wide totals.

The mapping audit itself adds no C credit. The subsequent [battle build milestone](battle-build-milestone.md) integrates ten pilot C functions, bringing the project to 34 exact functions / 2,228 bytes. Gameplay ownership and mode percentages remain incomplete. The comparison report keeps the two untraced candidates separate; use `tools/report_dispatch_changes.py --baseline-name dispatch-baseline-v5 --output docs/decomp/deferred-battle-changes.json --allow-untraced` to refresh it.
