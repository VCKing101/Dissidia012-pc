# Dissidia 012 (ULUS10566 v1.00) — battle camera manual control: static analysis

> Static-analysis report produced by a research subagent on 13 October 2026 and kept as written.
> The hook sites, single-caller claims, action masks and yaw arithmetic were re-checked against the binary afterwards.
> Nothing here has been confirmed at runtime yet.

Scope: static analysis only (Ghidra pseudocode, refs.tsv, `tools/re_query.py` disassembly, raw reads of
`OVL_BATTLE_APP.ELF`, `EBOOT.ELF`, and one read-only scan of the local USA ISO for a data chunk).
Nothing in the repo was modified, nothing was run in an emulator.

All addresses are **battle overlay** unless prefixed `main:`.

---------------------------------------------------------------------------------------------------

## 0. Answer in one paragraph

The D-pad-camera hypothesis is **correct, with one qualification**: the camera does not test raw
D-pad bits, it tests four *logical action bits* (`0x02000000`, `0x04000000`, `0x08000000`,
`0x10000000`, low word of the 64-bit logical pad) which the button-config layer in main maps from
the D-pad (default) or from the analog nub's digital "virtual direction" bits (alternate control
type). Exactly two functions in the whole battle overlay read a pad for camera purposes:
`0x08B3F7F4` (free camera) and `0x08B40290` (lock-on camera). Each has exactly one caller, and the
call sites (`jal` at `0x08B3F450` and `0x08B400B0`) are the recommended hook points.

---------------------------------------------------------------------------------------------------

## 1. VERIFIED FACTS

### 1.1 The pad object the camera reads (main, just enough to follow the input)

`main:0x088729A0(i)` returns `g + 0xC0 + i*0x50` = "logical pad i". Evidence that this is a remapped
view of raw pad i:

* `main:0x08872F90` (PAD_MANAGER init) sets `logical[i]+0x40 = &raw[i]` and copies the 0x1E0-byte
  template at `main:0x089C9888` to `main:0x08B7F380 + i*0x1E0` (bss).
* `main:0x088726B0(logical)` builds the logical 64-bit held mask by running the raw held mask
  through two tables with `main:0x0887263C` (entry = `{u64 src, u64 dst}`, "if (raw & src)==src then
  out |= dst", terminated by src==0): always the default table `main:0x089C9738`, plus the table at
  `logical+0x4C` if non-null. It also copies the raw stick vec4 (`raw+0x20`) to `logical+0x20`.
* `logical+0x4C` is set by `main:0x08872E50(g, cfg)` from the pointer array `main:0x089D5FF8`
  (`cfg 0 -> {0,0}`, `cfg 1 -> {0x08B7F380, 0x08B7F560}`).
* Logical pad layout is the same as the raw pad for the first 0x34 bytes (`+0x00` held u64,
  `+0x08` pressed, `+0x10` released, `+0x18` repeat, `+0x20` stick vec4), `+0x34` = vtable
  (`main:0x089C96E8`).
* Logical pad vtable (`main:0x089C96E8`, entries are `{s16 this_adjust @+N-4.., fn @+N}`):
  `+0x0C` held-any (`main:0x08872740` -> `0x08871CFC`), `+0x14` pressed-any (`0x0887279C`),
  `+0x1C` released-any (`0x088727F8`), `+0x24` repeat-any (`0x08872854`), `+0x2C` get stick vec4
  (`0x088728B0`). Every one of them first calls `main:0x08873084` and returns 0 / neutral if
  `*(u32*)(g+0x264) & 2` (a global input-lock flag).

Battle action template `main:0x089C9888` (raw src -> logical dst), the entries relevant here:

| raw src | logical dst | meaning (see 1.3) |
|---|---|---|
| `0x40` DOWN | `0x0000000002000000` | camera "A" vertical |
| `0x10` UP | `0x0000000004000000` | camera "B" vertical |
| `0x80` LEFT | `0x0000000008000000` | camera yaw + |
| `0x20` RIGHT | `0x0000000010000000` | camera yaw - |
| hi `0x4/0x10/0x20/0x8` (nub up/down/left/right) | `0x20000000`, `0x40000000`, `0x80000000`, hi `0x1` | movement |
| `0x100` L | hi `0x2` | lock toggle (see 1.6) |
| `0x1` SELECT | hi `0x4` | camera reset (see 1.3) |

The runtime copies are **rewritten per player** by `main:0x0887309C(padIdx, moveOnNub, invV, invH, forceCamOnNub)`,
which replaces the *source* mask of the entries whose dst is one of the eight bits above:

* dst `0x02000000`: src = D-pad UP (`0x10`), or DOWN (`0x40`) if `invV`; if `!moveOnNub || forceCamOnNub`:
  src = nub-up (hi `0x4`), or nub-down (hi `0x10`) if `invV`.
* dst `0x04000000`: the opposite vertical direction of the above.
* dst `0x08000000`: src = D-pad RIGHT (`0x20`), or LEFT (`0x80`) if `invH`; nub variants hi `0x8` / hi `0x20`.
* dst `0x10000000`: the opposite horizontal direction.
* dst `0x20000000/0x40000000/0x80000000/hi 0x1` (movement): nub if `moveOnNub`, else D-pad.

Callers: `main:0x088FB3C0` passes, for player i, flags `f = *(u16*)(0x09AFD960 + 4*i)`:
`moveOnNub = (f & 0x20) != 0`, `invV = f & 1`, `invH = (f & 2) != 0`, `forceCamOnNub = (f & 0x40) == 0`.
Two options-menu paths (`main:0x0891468C`, `main:0x0891813C`) call it with values from
`main:0x088F47E0(4/1/2)`.

So the game already supports "camera on the analog nub", but only as **digital** virtual directions.

### 1.2 Camera object model

Constructor `0x08B3C4C8(cam, slot, ownerChar, paramRow, pad)`; allocated (0x310 bytes) and stored by
`0x08B3BC34(mgr, slot, ownerChar, paramRow, pad)` into `mgr + 0x6D0 + slot*4` (8 slots).
`mgr = *(u32*)0x08B3D6B8` (pointer in battle data; written by `main:0x08878DE0`).
The character constructor `0x08B39E5C(char, pad, charData, slot, ?)` does
`char+0x5A0 = pad` and `char+0x5AC = FUN_08B3BC34(mgr, slot, char, *(u8*)(*(char+0x39C)+0xB), pad)`.
**Every character (player, opponent, assists) owns its own camera object.**

Camera fields used below (all verified from the code cited in this report):

| offset | type | meaning |
|---|---|---|
| `+0x00/+0x10/+0x20/+0x30` | vec4 | eye: current / previous / desired / velocity |
| `+0x40/+0x50/+0x60/+0x70` | vec4 | look-at: current / previous / desired / velocity |
| `+0x80/+0x90/+0xA0/+0xB0` | vec4 | up: current / previous / desired / velocity |
| `+0xD8..+0xEC` | 6 floats | spring coefficients copied from the param row |
| `+0xF0` | s32 | camera speed index (three speeds per row in the data; constructor default 1). Read at `0x08B3F85C`, `0x08B402B8`, `0x08B40D7C` |
| `+0xF4` | u8 | auto-follow-behind enable (free mode only; read at `0x08B3FAA4`) |
| `+0xF8` | ptr | owner character |
| `+0xFC` | ptr | override look target (non-zero forces lock-on mode, `0x08B3B0B4`) |
| `+0x100` | s32 | dt source selector (see 1.5) |
| `+0x108` | s32 | **current mode index** (0..3) |
| `+0x110` | s32 | saved mode to return to after mode 2/3 |
| `+0x11C` | s32 | row index into the camera parameter table |
| `+0x120` | ptr | **pad object** (same pointer as `char+0x5A0`) |
| `+0x125` | u8 | "this camera is the one on screen" (`0x08B3B310` setter, `0x08B3B308` getter) |
| `+0x126` | u8 | **manual input enabled** |
| `+0x12C` | ptr[4] | mode object pointers |
| `+0x13C` | mode 0 object | `+0x00` vtable `0x08B3FCD8`, `+0x04` float distance `P`, `+0x08` float yaw |
| `+0x148` | mode 1 object | `+0x00` vtable `0x08B41560`, `+0x04` distance `D`, `+0x08` auto distance offset, `+0x0C` yaw current, `+0x10` yaw target, `+0x14` **manual yaw delta for this frame**, `+0x18` u8 flag, `+0x1C` lock target kind |
| `+0x168` | mode 2 object | vtable `0x08B3F118` |
| `+0x180` | mode 3 object | vtable `0x08B3F1A8` |

Mode vtable slots used: `+0x0C` enter, `+0x14` leave, `+0x1C` **update**, `+0x5C` reset, `+0x64`
"requested mode or -1", `+0x7C` post-update (spring smoothing `0x08B3D72C`).

### 1.3 The two functions that read input

A word search for `lw rX, 0x120(rY)` over the camera module (`0x08B3A000..0x08B43000`) finds only
`0x08B3F84C` and `0x08B402FC`, i.e. these two functions. `+0x126` is read only at `0x08B3F7F8` and `0x08B402D8`.

#### (a) Free camera input — `0x08B3F7F4`, size 1244 (+ delay slot at `0x08B3FCD0`)

Only caller: `jal` at `0x08B3F450` in `0x08B3F40C` (mode 0 update, vtable `0x08B3FCD8+0x1C`).
Args: `a0` = mode 0 object (`cam+0x13C`), `a1` = camera.

```
08B3F7F8: lbu   $a2, 0x126($a1)        ; manual input enabled?
08B3F800: move  $s0, $a0               ; s0 = mode0 object
08B3F824: beqz  $a2, -> epilogue       ; no input -> return, nothing changed
08B3F828: move  $s1, $a1               ; s1 = camera
08B3F82C: lw    $a0, 0x11C($s1)        ; row index
  ...                                  ; s2 = *(0x089D4798) + row*0x94
08B3F84C: lw    $s3, 0x120($s1)        ; s3 = pad object
08B3F854: jal   func_8B3B4FC           ; f0 = dt (see 1.5)
08B3F85C: lw    $a0, 0xF0($s1)         ; speed index
08B3F860: addiu $a1, $s2, 0x14
08B3F864: sll   $a0, $a0, 2
08B3F868: addu  $a0, $a1, $a0
08B3F86C: lw    $a1, 0x34($s3)         ; pad vtable
08B3F870: mov.s $f20, $f0              ; f20 = dt
08B3F874: lwc1  $f22, 0x0($a0)         ; f22 = row[0x14 + 4*idx] = yaw speed, degrees per dt unit
08B3F878: addiu $a1, $a1, 0x8
08B3F87C: lh    $a0, 0x0($a1)          ; this-adjust
08B3F880: lui   $a2, 0x8B4
08B3F884: lw    $a3, -0x28C($a2)       ; hi word of mask @0x08B3FD74 = 0
08B3F888: lw    $a2, -0x290($a2)       ; lo word of mask @0x08B3FD70 = 0x08000000
08B3F88C: lw    $a1, 0x4($a1)          ; vtable+0x0C = "held any"
08B3F890: jalr  $a1
08B3F894: addu  $a0, $s3, $a0
08B3F898: bnez  $v0, -> 08B3F8B0       ; held 0x08000000 ?
  ; --- 0x08000000 held: yaw += (pi/180) * speed * dt
08B3F8B8: lwc1  $f12, -0x298($s5)      ; 0x08B3FD68 = 3.14159 (0x40490FDB)
08B3F8BC: mtc1  $a0, $f13              ; 0x43340000 = 180.0
08B3F8C0: div.s $f12, $f12, $f13
08B3F8C4: lwc1  $f14, 0x8($s0)         ; yaw
08B3F8C8: mul.s $f12, $f12, $f22
08B3F8CC: mul.s $f12, $f12, $f20
08B3F8D0: add.s $f12, $f14, $f12
08B3F8D4: swc1  $f12, 0x8($s0)
  ...                                  ; wrap into (-pi, pi) with fmodf (main:0x089941C0), store at 08B3F948
  ; --- else test 0x10000000 (mask @0x08B3FD78), jalr at 08B3F96C
08B3F974: beqz  $v0, -> 08B3FA14
08B3F994: mul.s $f22, $f12, $f20       ; (pi/180)*speed*dt
08B3F998: sub.s $f22, $f14, $f22       ; yaw -= ...
08B3F99C: swc1  $f22, 0x8($s0)
  ; --- vertical: test 0x02000000 (mask @0x08B3FD80), jalr at 08B3FA38
08B3FA48: lwc1  $f12, 0x8($s2)         ; row[0x08] = distance speed
08B3FA4C: lwc1  $f13, 0x4($s0)         ; P
08B3FA50: mul.s $f12, $f12, $f20
08B3FA54: sub.s $f12, $f13, $f12       ; P -= row[8]*dt
08B3FA5C: swc1  $f12, 0x4($s0)
  ; --- else test 0x04000000 (mask @0x08B3FD88), jalr at 08B3FA80
08B3FA9C: add.s $f12, $f13, $f12       ; P += row[8]*dt
08B3FAA0: swc1  $f12, 0x4($s0)
  ; --- auto-follow (only if cam+0xF4 == 1 and owner flags allow): 08B3FAA4..08B3FC08
  ;     yaw moves toward atan2(-facing.x, -facing.z) by at most row[0x20] rad per frame
  ; --- reset: pressed-any (vtable+0x14) of mask @0x08B3FD90 = {lo 0, hi 0x4}, jalr at 08B3FC30
  ;     then if !(owner+0x5B6 & 0x80): call mode vtable+0x5C (0x08B3F7AC: yaw = atan2(-facing.x,-facing.z))
  ; --- clamp P to [row[0x00], row[0x04]]
08B3FC6C: lwc1  $f12, 0x4($s0)
08B3FC70: lwc1  $f13, 0x0($s2)
08B3FC80: lwc1  $f14, 0x4($s2)
08B3FCA4: swc1  $f12, 0x4($s0)
```

How it becomes motion (`0x08B3F5A8`, called right after from `0x08B3F40C`):
`eye_desired = lookat_desired + RotY(yaw) * (0, 0, P)`, then
`eye_desired.y = lookat_desired.y + row[0x28] + (P - row[0x00])^2 * row[0x0C]`
(`08B3F5F4 sub.s`, `08B3F60C mul.s`, `08B3F624 mul.s $f20,$f13,$f15`, `08B3F6B0 jal 0x089A4124` with `$f12 = yaw`,
`08B3F6E0/E4 add.s`, `08B3F6F4 swc1 $f12,0x24($s0)`).
There is **no independent pitch**: "vertical" input changes one scalar `P` that is both the orbit
distance and (quadratically) the camera height.

#### (b) Lock-on camera input — `0x08B40290`, size 616

Only caller: `jal` at `0x08B400B0` in `0x08B40014` (mode 1 update, vtable `0x08B41560+0x1C`).
Args: `a0` = mode 1 object (`cam+0x148`), `a1` = camera.

```
08B402C0: addiu $s3, $s3, 0x44         ; s3 = row + 0x44
08B402D4: lwc1  $f22, 0x0($a2)         ; f22 = row[0x6C + 4*idx] = yaw speed (deg per dt unit)
08B402D8: lbu   $a2, 0x126($a1)
08B402E0: move  $s1, $a0               ; s1 = mode1 object
08B402F4: beqz  $a2, -> epilogue
08B402F8: move  $s0, $a1               ; s0 = camera
08B402FC: lw    $s2, 0x120($s0)        ; s2 = pad
08B40300: jal   func_8B3B4FC
08B4030C: mov.s $f20, $f0              ; f20 = dt
  ; held 0x08000000 (mask @0x08B415F8)? jalr at 08B40330
08B40338: beqz  $v0, -> 08B40370
08B40340: lwc1  $f12, 0x15F0($a0)      ; 0x08B415F0 = 3.14159
08B4034C: div.s $f12, $f12, $f13       ; / 180.0
08B40350: mul.s $f12, $f12, $f22
08B40354: mul.s $f12, $f12, $f20
08B40358: swc1  $f12, 0x14($s1)        ; manual yaw delta = +(pi/180)*speed*dt
  ; else held 0x10000000 (mask @0x08B41600)? jalr at 08B40394
08B403B8: mul.s $f22, $f12, $f20
08B403C0: neg.s $f22, $f22             ; (delay slot)
08B403C4: mtc1  $zero, $f22            ; neither held -> 0
08B403C8: swc1  $f22, 0x14($s1)        ; manual yaw delta = -(...) or 0
  ; held 0x02000000 (mask @0x08B41608)? jalr at 08B403E8
08B403F8: lwc1  $f12, 0x8($s3)         ; row[0x4C] = distance speed
08B40400: mul.s $f20, $f12, $f20
08B40404: sub.s $f20, $f13, $f20       ; D -= row[0x4C]*dt
08B4040C: lwc1  $f12, 0x0($s3)         ; clamp low  = row[0x44]
08B40424: swc1  $f12, 0x4($s1)
  ; else held 0x04000000 (mask @0x08B41610)? jalr at 08B40448
08B40464: add.s $f20, $f13, $f20       ; D += row[0x4C]*dt
08B4046C: lwc1  $f12, 0x4($s3)         ; clamp high = row[0x48]
08B40480: swc1  $f12, 0x4($s1)
  ; pressed-any mask @0x08B41618 = {0, hi 0x4} -> mode vtable+0x5C (0x08B40BFC reset)
```

How it becomes motion (`0x08B40CDC`, called next from `0x08B40014`; in this function `$f20` = speed,
`$f22` = dt, `$s0` = mode 1 object, `$s3` = `0x08B40000` for the pi constant):

```
08B40FF0: swc1  $f0, 0x10($s0)         ; target yaw = yaw of (owner - target) when the target is out of frame
08B40FFC: lwc1  $f12, 0xC($s0)         ; ...else target yaw = current yaw
08B41000: swc1  $f12, 0x10($s0)
08B41004..08B4107C                     ; frustum-edge auto nudge of target yaw by +/-(pi/180)*speed*dt
08B411B8: lwc1  $f12, 0x10($s0)
08B411BC: lwc1  $f13, 0x14($s0)        ; <-- manual delta consumed here
08B411C4: add.s $f12, $f12, $f13
  ...wrap...
08B41240: swc1  $f12, 0x10($s0)
08B412B4..08B41354                     ; current yaw (+0x0C) slews toward target (+0x10)
08B412DC: mul.s $f13, $f13, $f20       ;   by at most (pi/180)*speed*dt per frame
08B412E4: mul.s $f20, $f13, $f22
08B412E8: add.s $f20, $f14, $f20
08B412EC: swc1  $f20, 0xC($s0)
08B413DC: swc1  $f12, 0xC($s0)         ; wrapped current yaw
```
then `eye_desired = ownerPos + RotY(yaw_current) * (0,0,clamp(D + autoOffset, row[0x44], row[0x48]))`
and the same quadratic height term with `row[0x50]`, `row[0x54]`.

**Lock-on mode therefore already has a signed per-frame yaw rate variable: `cam+0x15C`
(mode1 `+0x14`), written every frame by `0x08B40290` and consumed at `0x08B411BC`.**

### 1.4 Parameter table and real speed values

`*(u32*)0x089D4798` is set by `main:0x08811598(src, size)` to `copy + 0x18`, where `copy` is a heap
copy of the `camr` chunk of `battle.bin` (`main:0x088135D4`; tag bytes at `main:0x089C6EF4`).
Row size 0x94, six rows. The values are **not in either ELF**; I read them from the local USA ISO
(`ARC\x01` container at ISO offset `0x16C821A0`, `camr` entry = offset `0x4520`, size `0x5C0`;
chunk header `43 41 4D 20 20 07 10 20 0A ...`).

Row 0 (rows 1-5 identical for every speed field; only distance limits differ):

| row offset | value | use |
|---|---|---|
| `+0x00`, `+0x04` | 3.0, 6.0 | free: `P` min / max (row2: 4/7, row3-4: 5/8, row5: 6/6) |
| `+0x08` | 0.08 | free: `P` change per dt unit |
| `+0x0C` | 0.9 | free: height = `row[0x28] + (P-min)^2 * 0.9` |
| `+0x14`, `+0x18`, `+0x1C` | **2.0, 3.0, 4.5** | free: yaw speed in **degrees per dt unit**, index = `cam+0xF0` |
| `+0x20` | 0.05 | free: auto-follow max yaw step (radians per frame) |
| `+0x28` | 0.7 | free: base height |
| `+0x44`, `+0x48` | 2.0, 5.0 | lock-on: `D` min / max (row1: 2.4/5.2, row2: 3/6, row3: 4/7, row4: 6/10) |
| `+0x4C` | 0.1 | lock-on: `D` change per dt unit |
| `+0x6C`, `+0x70`, `+0x74` | **2.0, 3.0, 4.5** | lock-on: yaw speed in degrees per dt unit |

Constants in the overlay: pi at `0x08B3FD68` and `0x08B415F0` (`0x40490FDB`); 180.0 is an immediate
(`lui 0x4334`).

Per-frame steps: yaw `= (pi/180) * {2,3,4.5} * dt` rad; distance `= 0.08*dt` (free) or `0.1*dt` (lock-on).

### 1.5 dt and the per-frame update path

`0x08B3B4FC(cam)`: `cam+0x100 == 0` -> `*(float*)(*(u32*)0x089D69E0 + 0x18)`; `== 1` ->
`*(float*)0x089D5EAC`; `== 2` -> `cam+0x104` (constructor sets 2.0); otherwise 0.
`0x08B0E200` (battle start, both combatants `*(0x089D6A10)+0x38/+0x3C`) sets `cam+0x100 = 0`,
`cam+0x126 = 1`; `0x08B0E1BC` sets `cam+0x100 = 1`, `cam+0x126 = 0`; `0x08B09D88` (battle end) clears
`+0x126` on both. Constructor leaves `+0x126 = 0`.

Update path, once per frame:

```
task "OBJ_CAMERA_MANAGER" 0x08B3C41C -> 0x08B3CA24(mgr)
  for slot 0..7: 0x08B3C760(cam)
      mode transitions (0x08B3B3EC)
      mode->vtable[+0x1C](mode, cam)     ; mode 0: 0x08B3F40C   mode 1: 0x08B40014
                                         ; mode 2: 0x08B3E6D0   mode 3: 0x08B3ECA8
          mode 0: 0x08B3F7F4 input -> 0x08B3F500 look-at -> 0x08B3F5A8 eye -> 0x08B3DE68 collision
          mode 1: 0x08B40290 input -> 0x08B40CDC eye -> 0x08B404F8 look-at -> 0x08B3DE68 -> 0x08B40644
      mode->vtable[+0x7C](mode, cam)     ; 0x08B3D72C: spring current -> desired
  the camera whose +0x125 == 1 is copied into the view (0x08B3CA24, then draw task 0x08B3C0D8)
```
`0x08B3C760` is also called directly twice from `0x08A3F448`.

### 1.6 Camera modes

* Mode 0 (vtable `0x08B3FCD8`): free orbit around the owner. Manual yaw + distance.
* Mode 1 (vtable `0x08B41560`): lock-on. `0x08B3C760` drops to mode 0 if the owner's target
  (character vtable `+0x134`) is null or flagged dead. Manual yaw *offset* + distance.
* Mode 2 (vtable `0x08B3F118`): entered automatically while `*(u8*)(owner+0x191) & 1`
  (`main:0x088378D8`); no pad access.
* Mode 3 (vtable `0x08B3F1A8`): scripted/animated camera started by `0x08B3AE14` / `0x08B3AEB4`; no pad access.
* `0x08B3AFD4(cam)` toggles mode 1 <-> 0 (or 2 <-> 1). It is called from the character handler
  `0x08B3526C` when `0x08B31DA4` is true, which tests *released* (pad vtable `+0x1C`) of mask
  `0x08B3AC58 = 0x0000000200000000` (L in the battle template).
* Battle start: mode 1 is selected if `*(u16*)(0x09AFD960 + 4*i) & 8`.
* Per-player settings pushed into the camera by `main:0x0888BA0C` via the 7-instruction function at
  `0x08B3B1B8`: `cam+0xF0 = (s8)*(0x09AFD962 + 4*i)` (speed index), `cam+0xF4 = (flags & 4) != 0`,
  `cam+0xF5 = (flags & 0x10) != 0`.

### 1.7 Who owns which pad

`main:0x0888BA0C` creates combatant 0 with `main:0x088729A0(0)` and combatant 1 with
`main:0x088729A0(1)`; when `*(ctx+0x4C) == 6`, combatant 0 gets logical pad **1** instead.
Assists are created by `main:0x08888354` with `main:0x08872B48(0)` (`g+0x160`). `0x08B43374` creates
a character on logical pad 1 and immediately sets its camera to `+0x126 = 0`, row 5.
Both combatants' cameras get `+0x126 = 1` at battle start, so **the opponent's camera also runs the
input functions every frame**, reading *its own* pad object.

---------------------------------------------------------------------------------------------------

## 2. RECOMMENDED HOOK POINTS

### 2.1 Primary: replace the two `jal`s (one instruction each)

| | free camera | lock-on camera |
|---|---|---|
| patch address | `0x08B3F450` | `0x08B400B0` |
| original word | `0E2CFDFD` (`jal 0x08B3F7F4`) | `0E2D00A4` (`jal 0x08B40290`) |
| delay slot (keep) | `0x08B3F454: move $a1,$s1` | `0x08B400B4: move $a1,$s0` |
| args at entry | `a0` = mode0 (`cam+0x13C`), `a1` = cam | `a0` = mode1 (`cam+0x148`), `a1` = cam |
| caller's live regs | `s0` = row ptr, `s1` = cam, `s2` = mode0 | `s0` = cam, `s1` = mode1, `s2` = row+0x78, `s3` = cam+0xE0, `s4` = 1 |
| float regs | nothing live across the call | nothing live across the call |

Because these are ordinary call boundaries, the hook is a plain o32 function: it may clobber all
caller-saved integer and float registers, must preserve `s0-s7`, `$f20+`, `ra`, `sp`.

What the hook computes (`rx`, `ry` in [-1, 1] after deadzone, already sign-corrected for the user's
inversion preference):

```
hook_free(mode, cam):
    call 0x08B3F7F4(mode, cam)                       ; keep D-pad / reset / auto-follow / clamp
    if cam.u8[0x126] == 0: return
    if cam.ptr[0x120] != *(u32*)0x089D6008 + 0xC0: return   ; only the local player's logical pad 0
    if *(u32*)(*(u32*)0x089D6008 + 0x264) & 2: return       ; same lock the pad vtable applies
    row = *(u32*)0x089D4798 + cam.s32[0x11C] * 0x94
    dt  = call 0x08B3B4FC(cam)                        ; returns in $f0
    yaw = mode.f32[0x08] + rx * (pi/180) * row.f32[0x14 + 4*cam.s32[0xF0]] * dt
    wrap yaw into (-pi, pi)                           ; the game uses fmodf at main:0x089941C0
    mode.f32[0x08] = yaw
    mode.f32[0x04] = clamp(mode.f32[0x04] + ry * row.f32[0x08] * dt, row.f32[0x00], row.f32[0x04])

hook_lock(mode, cam):
    call 0x08B40290(mode, cam)                        ; rewrites mode+0x14 every enabled frame
    same three gates
    row = ...; dt = ...
    mode.f32[0x14] += rx * (pi/180) * row.f32[0x6C + 4*cam.s32[0xF0]] * dt
    mode.f32[0x04]  = clamp(mode.f32[0x04] + ry * row.f32[0x4C] * dt, row.f32[0x44], row.f32[0x48])
```

Sign reference from the D-pad code: logical `0x08000000` -> yaw **+**, `0x10000000` -> yaw **-**,
`0x02000000` -> distance **-**, `0x04000000` -> distance **+**. With both inversion flags clear the
rewritten table maps D-pad RIGHT -> `0x08000000` and D-pad UP -> `0x02000000`; the hook can read
`*(u16*)0x09AFD960` masks `0x1` (`invV`) and `0x2` (`invH`) to follow the user's setting.

This is the point I recommend: in lock-on mode it writes the game's own signed rate variable before
it is applied; in free mode no such variable exists (each direction branch stores immediately), so
running after the original and before `0x08B3F500/0x08B3F5A8` is the equivalent.

### 2.2 Alternative inline points (if a wrapper is not wanted)

Free camera, inside `0x08B3F7F4` — live there: `s0` mode0, `s1` cam, `s2` row, `s3` pad, `f20` dt:
* `0x08B3F874 lwc1 $f22,0($a0)` — yaw speed load; scale `$f22` by |rx|.
* Direction decisions are the `$v0` results at `0x08B3F898` (yaw+), `0x08B3F974` (yaw-),
  `0x08B3FA40` (dist-), `0x08B3FA88` (dist+).
* Vertical step is formed at `0x08B3FA50` / `0x08B3FA98` (`mul.s $f12,$f12,$f20`).

Lock-on, inside `0x08B40290` — live: `s0` cam, `s1` mode1, `s2` pad, `s3` row+0x44, `f20` dt, `f22` speed:
* `0x08B40358 swc1 $f12,0x14($s1)` and `0x08B403C8 swc1 $f22,0x14($s1)` — the two stores of the
  signed yaw delta; `0x08B403CC` is the first instruction after both paths have stored it.

### 2.3 Zero-overlay-patch fallback (digital only)

OR-ing `0x08000000 / 0x10000000 / 0x02000000 / 0x04000000` into logical pad 0's held word
(`g+0xC0`) in main after `main:0x088725A0` makes the right stick behave exactly like the D-pad
camera with no battle patch. No proportional speed.

---------------------------------------------------------------------------------------------------

## 3. FRAGILITY

1. **Two code paths.** Free and lock-on are separate functions with separate state; both need the hook.
   Modes 2 and 3 ignore input entirely (by design).
2. **Lock-on rate cap.** `0x08B412B4..0x08B41354` limits the visible yaw change to
   `(pi/180)*speed*dt` per frame regardless of `+0x14`. Analog 0..1 scales cleanly; anything
   faster than the stock D-pad speed needs that slew patched as well (it uses the same row field).
3. **Lock-on re-centres.** When the target leaves the frame (`0x08B40FE8..0x08B40FF0`) the target
   yaw is recomputed from positions, discarding the accumulated manual offset. Stock behaviour.
4. **Every character's camera runs this code.** Without the `cam+0x120 == logical pad 0` gate a
   global right-stick value would also rotate the opponent's (off-screen) camera. `cam+0x125`
   is an alternative gate ("camera on screen").
5. **Pad 0 is not always the viewed combatant.** `main:0x0888BA0C` gives combatant 0 logical pad 1
   when `*(ctx+0x4C) == 6`.
6. **Button config / control type.** The D-pad may not be the camera at all (nub can be), and
   inversion is applied in the remap table, not in the camera. A hook that bypasses the table
   must apply inversion itself.
7. **Input lock.** D-pad camera input is suppressed by `g+0x264 & 2` inside the pad vtable; the hook
   must replicate that or the camera will move when the game intends input to be dead.
8. **Shared file data.** Speeds come from `battle.bin/camr`, not code. A different region/revision
   may differ.
9. **Unlisted functions.** Ghidra's function list misses small functions only called from main
   (e.g. `0x08B3B1B8`); do not treat gaps between listed functions as free space.

---------------------------------------------------------------------------------------------------

## 4. INFERENCES (not proven statically)

* **dt unit is 1/60 s, so dt is about 2.0 at 30 fps.** Basis: `main:0x08871BD0` decrements the pad
  repeat timer by `max(2.0, *(float*)0x089D5EAC)` with reload values 30.0 and 4.0. If so, stock yaw
  speed is 120 / 180 / 270 degrees per second for speed index 0 / 1 / 2.
* **Speed index 0/1/2 is the user's camera-speed option**; I saw where it is copied from
  (`0x09AFD962 + 4*i`) but not the option UI.
* **Flag bit meanings at `0x09AFD960`** (`0x20` move-on-nub, `0x40` clear = camera forced to nub,
  `0x4` auto-follow, `0x8` start locked-on) are read off the call arguments, not from the options screen.
* **Mode 2 = chase/special-state camera, mode 3 = attack/cutscene scripted camera** — named from structure only.
* **Camera state is gameplay state.** `0x08B36D2C` builds a view basis from a character's camera
  (`0x08B3B31C` on `char+0x5AC`) and passes it with the pad (`char+0x5A0`) to `0x08B38A0C`, which
  looks like camera-relative movement. If replays or ad-hoc play re-simulate from recorded/transmitted
  pad state, an analog camera input that is not part of that state would desynchronise them.
* **The spring smoothing in `0x08B3D72C` / `0x08B3B99C` does not use dt** (the pseudocode of
  `0x08B3B99C` has no dt term), which would make camera lag frame-rate dependent. Relevant to the
  frame-rate goal; not verified in disassembly.
* `main:0x08935524` is assumed to be a plain memcpy (the `camr` values are used unmodified).

---------------------------------------------------------------------------------------------------

## 5. NOT DETERMINED — needs runtime confirmation

* Actual dt values in battle (`*(float*)(*(u32*)0x089D69E0 + 0x18)`), and whether it is 0 while paused.
* On-screen direction of yaw + (clockwise or counter-clockwise) and the default state of the two
  inversion flags; i.e. the correct default sign for `rx` / `ry`.
* Whether `cam+0xF0` is always within 0..2.
* What feeds logical pad 1 for a CPU opponent, a replay, or ad-hoc play, and whether those sources
  ever carry the four camera bits (decides how much the opponent's camera moves on its own).
* What `*(ctx+0x4C) == 6` is (replay? spectate?) and which camera is on screen then.
* Whether replays/ad-hoc are actually sensitive to camera yaw (the determinism concern above).
* Meaning of `cam+0xF5` (within the camera module it is read only at `0x08B406A0` in `0x08B40644`,
  the lock-on look-at height adjustment).
* Which physical nub direction each virtual bit (hi `0x4/0x8/0x10/0x20`) represents; the
  up/down/left/right labels in 1.1 assume the usual PSP axis convention (x+ right, y+ down).
* Where injected code can live (code cave vs. separate module) and where the pad module will expose
  the right-stick values — out of scope here; the hook only needs two floats for the local player.
* The ISO offset quoted for `camr` is specific to the local dump; the in-memory address is
  `*(u32*)0x089D4798` (rows) / `*(u32*)0x089D4790` (chunk copy).
