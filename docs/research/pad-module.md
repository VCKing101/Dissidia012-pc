# Dissidia 012 (ULUS10566 v1.00) - controller-input ("pad") module map

> Static-analysis report produced by a research subagent on 13 October 2026 and kept as written.
> Open item 4 is since answered: PPSSPP 1.20.4 delivers L3 = 0x0002, R3 = 0x0004, L2 = 0x0400, R2 = 0x0800 in `Buttons`
> (`Core/HLE/sceCtrl.h`), and they are inside `CTRL_MASK_USER`.

Static analysis only. Sources: `analysis/ULUS10566/main/EBOOT.ELF` disassembly (rabbitizer via `tools/re_query.py`),
Ghidra `pseudocode.c` / `refs.tsv` for main, battle, menu, raw instruction scans of all four modules (main, menu, battle, extra).
Nothing in the repo was modified; nothing was run in an emulator.

Conventions: "V:" = verified fact (address + evidence given). "I:" = inference. All 64-bit button masks are written `{lo, hi}`.
"Raw" bits are the bits stored in a raw pad; "logical" bits are the bits stored in a g+0xC0 pad after remapping.

---

## 0. Headline findings (things that change the mod plan)

1. V: The pad manager is a **static object at fixed address 0x08C15B90** (not heap). `0x089D6008` is just a pointer to it,
   written once by the static-init function 0x088769CC (`lui s4,0x8C1; addiu s4,s4,0x5B90` at 0x08876E34/0x08876E54, `sw s4,0x6008(0x89D)` at 0x08876F00).
   Its destructor record is `{0, 0x08C15B90, 0x08872C38}` at 0x089D658C.
2. V: The module proper is **0x08871B40..0x08873497**. 0x08871818..0x08871B3F is an unrelated async file loader
   (strings "package.bin"/"package_info.bin" at 0x089C9690); 0x08873498 onward is projection/camera parameter code
   (0x08873498 writes {50.0, 1.0, 1000.0, 1.7647 = 480/272}).
3. V: There **is** a remap layer: raw pad -> logical pad through two `{phys mask, logical mask}` tables (section 3).
   Raw pads are reachable from outside only through 0x08872088: 19 menu and 4 main sites, every one of them
   `raw[0]->isPressed(mask)`, plus the two netplay injectors.
4. V: Overlays (menu/battle/extra) contain **no** load of 0x089D6008 and no absolute reference into 0x08C15B90..0x08C15DFF.
   They reach pads only through the accessor functions and then (almost exclusively) through the pad vtable at +0x34.
5. V: Replays store **15 raw bits + 1 flag per run-length record**, and ad-hoc netplay transmits **exactly 16 bits per frame**
   (12 buttons + 4 analog-direction bits). Both formats are full. The analog stick is reduced to 8 directions in both.
   Any right-stick state that is not expressed through those existing bits is neither recorded nor transmitted.
6. V: The battle camera is driven by four *logical* bits (0x02000000/0x04000000/0x08000000/0x10000000) tested with
   `isHeld` in battle 0x08B3F7F4 and 0x08B40290, i.e. it is digital and it sits behind the remap layer.
7. V: The dead zone is 100.0 out of 127.5 per axis (rodata 0x089C9720), so the left stick is close to digital already.

---

## 1. Layout of the pad manager object `g` (0x08C15B90)

### 1.1 Size

- V: lower bound 0x268 (last field written is the u32 at +0x264, e.g. 0x08872C1C).
- V: upper bound 0x270: the next symbol is the static-init guard word at 0x08C15E00 (`lw/sw 0x5E00(0x8C1)` at 0x08876EA4/0x08876EB0),
  guarding the next object at 0x08C15E80. g's own guard is at 0x08C15B80 (0x08876ED0/0x08876EE4).
- I: sizeof = 0x270 (16-byte alignment because of the float[4] members).

### 1.2 Struct declarations

```c
typedef struct { u32 lo, hi; } Mask64;          /* always passed in a2:a3 / returned in v0:v1 */

/* vtable entry format (verified at every virtual call site, e.g. 0x08872F34..0x08872F58):
   lw vt,0x34(obj); lh adj,slot+0(vt); lw fn,slot+4(vt); a0 = obj+adj; jalr fn                */
typedef struct { s16 thisAdjust; s16 pad; void *fn; } VtEntry;   /* 8 bytes */

struct PadBase {                 /* size 0x40, ctor 0x08871CB8, vtable 0x089C96B0 */
/*00*/ Mask64 held;              /* w: 0x08871BD0, 0x08871B40, 0x08872540/0x088725A0 (logical only); r: 0x08871BC4, 0x08871CFC, 0x0887263C */
/*08*/ Mask64 pressed;           /* w: 0x08871BD0, 0x08871B40; r: 0x08871D18 */
/*10*/ Mask64 released;          /* w: 0x08871BD0, 0x08871B40; r: 0x08871D34 */
/*18*/ Mask64 repeat;            /* w: 0x08871BD0, 0x08871B40; r: 0x08871D50 */
/*20*/ float  stick[4];          /* {dirX, dirY, 0, magnitude<=1}. w: 0x08872138 (via 0x08871E08), 0x08871B40 (zero),
                                    0x088726B0 (copy raw->logical), 0x08872B04 (zero vec); r: 0x08871D6C */
/*30*/ float  repeatTimer;       /* w/r: 0x08871BD0; w: 0x08871B40 */
/*34*/ const VtEntry *vtable;    /* NOT free. w: ctors/dtors; r: every virtual call */
/*38*/ u8     _pad38[8];         /* no accessor found anywhere (see 1.4) */
};

struct RawPad {                  /* size 0x60, ctor 0x0887205C, base vtable 0x089C96B0 */
/*00*/ struct PadBase base;
/*40*/ struct {                  /* an embedded SceCtrlData used when mode==1 */
/*40*/     u32 timeStamp;        /* w: net only (0x0890DE48, 0x0890E5AC) = 0; never read */
/*44*/     u32 buttons;          /* w: 0x08871DAC(=0), 0x08872304, net (0x0890DE50, 0x0890E5B4); r: 0x08871ED4 */
/*48*/     u8  lx;               /* w: 0x08871DAC(=127), 0x08872304, net; r: 0x08871EAC */
/*49*/     u8  ly;               /* w: same;                            r: 0x08871EB4 */
/*4A*/     u8  rsrv[6];          /* w: net only, as part of the word copies at 0x0890DE54/58 and 0x0890E5B8/BC,
                                    with UNINITIALISED STACK bytes (0x0890D090 fills only +0,+4,+8,+9); never read */
       } inject;
/*50*/ u32    mode;              /* 0 = hardware, 1 = use `inject`. w: 0x0887205C(=0), 0x088720B4; r: 0x08871E5C */
/*54*/ u8     _pad54[12];        /* no accessor found anywhere */
};

struct MapEntry { Mask64 phys; Mask64 logical; };   /* 16 bytes; table terminated by phys == {0,0} */

struct LogicalPad {              /* size 0x50, ctor 0x088724CC, vtable 0x089C96E8 */
/*00*/ struct PadBase base;
/*40*/ struct RawPad *src;       /* w: 0x08873028 (init: &g->raw[i]); r: 0x088726B0, 0x08872970, and externally 0x088CA680 (replay recorder) */
/*44*/ u32    flags;             /* bit0 = disabled, bit1 = "was disabled, swallow edges on next update" (0x08872510/40/94, 0x088725A0) */
/*48*/ u32    index;             /* w: 0x0887302C (= i); never read in module */
/*4C*/ const struct MapEntry *actionMap;  /* w: 0x08872EC0; r: 0x088726E0. NULL or 0x08B7F380 + i*0x1E0 */
};

struct PadManager {              /* static @ 0x08C15B90, size 0x270 */
/*000*/ struct RawPad     raw[2];        /* 0x08C15B90, 0x08C15BF0. Accessor 0x08872088(i) */
/*0C0*/ struct LogicalPad logical[2];    /* 0x08C15C50, 0x08C15CA0. Accessor 0x088729A0(i) */
/*160*/ struct PadBase    null[2];       /* 0x08C15CF0, 0x08C15D30. Accessor 0x08872B48(i). ctor 0x08872ADC */
/*1E0*/ struct RawPad     system;        /* 0x08C15D70. Accessor 0x088720A4(). Receives the hardware during replay playback */
/*240*/ s32   mapSet;                    /* 0x08C15DD0. -1 (ctor), 0 (init), then 1 (0x08933764). w: 0x08872BF4, 0x0887304C, 0x08872ED8; never read */
/*244*/ u8    _unused244[16];            /* 0x08C15DD4..0x08C15DE3: no accessor found anywhere */
/*254*/ u32   _zero254;                  /* 0x08C15DE4. written 0 at 0x08872BF8 and 0x08873050; never read */
/*258*/ float deadZone;                  /* 0x08C15DE8 = 100.0 (0x089C9720). r: 0x088721CC, 0x0887221C, 0x08872264 */
/*25C*/ float repeatDelay;               /* 0x08C15DEC = 30.0 (0x089C9724). r: 0x08871C3C */
/*260*/ float repeatInterval;            /* 0x08C15DF0 = 4.0 (0x089C9728). r: 0x08871CA0 */
/*264*/ u32   flags;                     /* 0x08C15DF4, see below */
/*268*/ u8    _tail[8];                  /* 0x08C15DF8..0x08C15DFF */
};
```

`g->flags` bits (every reader/writer found, main only; overlays have none):

| bit | meaning | set / cleared | read |
|---|---|---|---|
| 0x1 | replay playback: hardware goes to `system`, `logical[]`/`null[]` are not auto-updated | set 0x088CA9A8 (state 0); cleared 0x088C9938 | 0x08872CFC, 0x08872D94, 0x08872DF8 |
| 0x2 | mute: every LogicalPad virtual returns 0 / neutral stick | **no writer found** | 0x08873084 (used by 0x08872740..0x088728B0) |
| 0x4 | `raw[0].isHeld({0x10000,0})` (PSP HOME bit) | set/cleared every frame at 0x08872F64..0x08872F7C | no reader found |
| 0x8 | with bit0: reset `system` instead of polling it | set/cleared in 0x08875540 (I: a sceUtility status poller, around an error dialog) | 0x08872D18 |

### 1.3 What the three pad arrays are

- V: `raw[i]` - physical state. Low word of the masks = the `Buttons` word of SceCtrlData **unfiltered** (0x08871F3C `lw s0,0x14(sp)`),
  high word = virtual bits (section 3.3). Only `raw[0]` (and `system`) ever read the hardware; `raw[1]` is neutral unless injected.
- V: `logical[i]` - remapped state; this is what 0x088729A0 hands out (jal sites: 38 main in 15 functions, 587 menu in 185,
  19 battle in 18, 17 extra in 7). Battle characters keep their pad pointer at character+0x5A0.
- V: `null[i]` - a PadBase that always has no buttons and a zero stick (0x08872B04 copies the global zero vector 0x09AF84C0 and
  calls setButtons(0)). Handed to non-player controllers: 0x088883E4 (extra battle objects created with player index i+2; I: assists), 0x0888DE34
  (replay viewer when replay flag 0xBDE8&2 is set), battle 0x08B20388.
- V: `system` - a third RawPad. Polled from hardware only while `flags&1` (0x08872D3C). Read by the replay UI in battle
  (0x08B1DE74, 0x08B1E370, 0x08B1F590, 0x08B2D010) and passed to the replay viewer at 0x0888DE1C.

### 1.4 Are the suspected "padding" ranges really unused?

Method: (a) every load/store in all 52 module functions was enumerated by offset; (b) all four modules were scanned for loads of
0x089D6008 and for absolute addresses inside g (only the module itself, 0x08875540, 0x088C9938, 0x088CA9A8, 0x0893354C and the
static-init function reference it); (c) every `jal` to the four accessors was followed with a simple register tracker to see
which offsets are applied to the returned pointer. Result of (c): only `lw 0x34` (vtable) is ever applied directly; the pointer
is also passed on or stored into other objects, where it can no longer be tracked. (d) The 177 pseudocode lines containing a
`+ 0x5a0)` dereference (the character's pad pointer) were grepped: none index a field of the pad other than +0x34.

| Range (RawPad) | Verdict | Touched by |
|---|---|---|
| +0x34..+0x37 | **In use** - vtable pointer | 0x08871CB8, 0x08872498, 0x08872B60, 0x08872A94, 0x08872C38, every virtual call |
| +0x38..+0x3F | unused - high confidence | nothing found |
| +0x40..+0x43 | written, never read | 0x0890D664 (0x0890DE48), 0x0890DF58 (0x0890E5AC): netplay writes 0 |
| +0x44..+0x49 | **In use** (inject.buttons, lx, ly) | 0x08871DAC, 0x08871E24, 0x08872304, net |
| +0x4A..+0x4F | never read, but **clobbered with stack garbage every synced netplay frame** on raw[0]/raw[1] | 0x0890DE54/58, 0x0890E5B8/BC |
| +0x50..+0x53 | **In use** (mode) | 0x0887205C, 0x088720B4, 0x08871E24 |
| +0x54..+0x5F | unused - high confidence (12 bytes per RawPad) | nothing found |

Other spare storage: g+0x244..+0x253 (16 bytes, 0x08C15DD4; not 16-byte aligned, so no `lv.q/sv.q`), g+0x268..+0x26F,
PadBase+0x38..+0x3F in all seven pads. Residual risk for all of these: a pad pointer that escaped into another object and is
then indexed directly could not be ruled out statically; the one confirmed case of direct field access from outside the module
is 0x088CA680 reading LogicalPad+0x40.

I: g lives in BSS, so all of the above start as zero.

### 1.5 Vtables

| slot | PadBase 0x089C96B0 | LogicalPad 0x089C96E8 | signature |
|---|---|---|---|
| +0x08 | 0x08871CFC | 0x08872740 | `bool isHeld(this, -, Mask64)` |
| +0x10 | 0x08871D18 | 0x0887279C | `bool isPressed(...)` |
| +0x18 | 0x08871D34 | 0x088727F8 | `bool isReleased(...)` |
| +0x20 | 0x08871D50 | 0x08872854 | `bool isRepeat(...)` |
| +0x28 | 0x08871D6C | 0x088728B0 | `Vec4 getStick()` - struct return: **a0 = out, a1 = this** (call site example battle 0x08B23B74) |
| +0x30 | 0x08871DA4 (returns 0) | 0x08872968 (returns 1) | "is a LogicalPad" - used by 0x088892E8 and by 0x088CA680 before it reads +0x40 |

0x08871DA4 and 0x08872968 are 8-byte functions missing from `config/inventory/main.json`.
getStick consumers found by pattern scan: battle 0x08B23A48, 0x08B249B8; menu 0x08A33FFC, 0x08B47414 (x2).

---

## 2. Function table

Caller columns are direct `jal` sites per module, counted by scanning every instruction word.

| Address | Size | Proposed name | Behaviour | jal sites main / menu / battle / extra | Other refs |
|---|---|---|---|---|---|
| 0x08871B40 | 132 | `PadBase_maskState` | (pad, u64 mask): held/pressed/released/repeat &= mask; repeat timer = 0; stick = (0,0,0,0) via 0x089A3890. mask 0 = full clear | 6 / 0 / 0 / 0 |  |
| 0x08871BC4 | 12 | `PadBase_getHeld` | returns u64 held (v0=lo, v1=hi) | 3 / 0 / 0 / 0 |  |
| 0x08871BD0 | 232 | `PadBase_setButtons` | (pad, -, u64 new in a2:a3): computes pressed/released, stores held, runs auto-repeat using g+0x25C/g+0x260 and frame step 0x089D5EAC | 6 / 0 / 0 / 0 |  |
| 0x08871CB8 | 68 | `PadBase_ctor` | vtable=0x089C96B0, maskState(0) | 3 / 0 / 0 / 0 |  |
| 0x08871CFC | 28 | `PadBase_isHeld` | vslot +0x08: (held & mask) != 0 | 1 / 0 / 0 / 0 | ptr at 0x089C96BC |
| 0x08871D18 | 28 | `PadBase_isPressed` | vslot +0x10: (pressed & mask) != 0 | 1 / 0 / 0 / 0 | ptr at 0x089C96C4 |
| 0x08871D34 | 28 | `PadBase_isReleased` | vslot +0x18: (released & mask) != 0 | 1 / 0 / 0 / 0 | ptr at 0x089C96CC |
| 0x08871D50 | 28 | `PadBase_isRepeat` | vslot +0x20: (repeat & mask) != 0 | 1 / 0 / 0 / 0 | ptr at 0x089C96D4 |
| 0x08871D6C | 56 | `PadBase_getStick` | vslot +0x28: struct-return (a0=out float[4], a1=this): copies this+0x20 | 1 / 0 / 0 / 0 | ptr at 0x089C96DC |
| 0x08871DAC | 92 | `RawPad_reset` | maskState(0); inject.buttons=0; inject.lx=inject.ly=127 | 2 / 0 / 0 / 0 |  |
| 0x08871E08 | 28 | `RawPad_setStickFromBytes` | (pad, float vec[4]) -> PadMgr_applyDeadZone(pad+0x20, vec) | 1 / 0 / 0 / 0 |  |
| 0x08871E24 | 568 | `RawPad_poll` | (pad, index): read hardware (index 0 only) or injected SceCtrlData (mode==1), dead zone, virtual bits, confirm/cancel, setButtons | 3 / 0 / 0 / 0 |  |
| 0x0887205C | 44 | `RawPad_ctor` | PadBase_ctor; mode(+0x50)=0 | 1 / 0 / 0 / 0 |  |
| 0x08872088 | 28 | `PadMgr_getRawPad` | (i) -> g + i*0x60 | 6 / 19 / 0 / 0 |  |
| 0x088720A4 | 16 | `PadMgr_getSystemPad` | () -> g+0x1E0 | 2 / 0 / 4 / 0 |  |
| 0x088720B4 | 48 | `PadMgr_setInjectMode` | (mode): raw[0].mode = raw[1].mode = mode | 4 / 0 / 0 / 0 |  |
| 0x088720E4 | 84 | `PadMgr_resetRawPads` | RawPad_reset on raw[0], raw[1] | 2 / 0 / 0 / 0 |  |
| 0x08872138 | 460 | `PadMgr_applyDeadZone` | (float out[4], float in[4]) -> u64 virtual direction bits; out = (dirx, diry, 0, min(len,1)) | 2 / 0 / 0 / 0 |  |
| 0x08872304 | 404 | `RawPad_inject` | (pad, -, u64 buttons): inject.buttons = lo word; inject.lx/ly rebuilt from analog-direction bits (0/127/254); mode=1; poll; mode=0 | 2 / 0 / 0 / 0 |  |
| 0x08872498 | 52 | `RawPad_dtor` | deleting dtor (vtable reset, optional operator delete 0x08992F04) | 0 / 0 / 0 / 0 |  |
| 0x088724CC | 68 | `LogicalPad_ctor` | PadBase_ctor; vtable=0x089C96E8; +0x40..+0x4C = 0 | 0 / 0 / 0 / 0 |  |
| 0x08872510 | 48 | `LogicalPad_disable` | flags|=1; maskState(0) | 1 / 0 / 0 / 0 |  |
| 0x08872540 | 84 | `LogicalPad_enable` | flags&=~1; maskState(0); held = translate() (no press edge for already-held buttons) | 1 / 0 / 0 / 0 |  |
| 0x08872594 | 12 | `LogicalPad_isDisabled` | flags & 1 | 1 / 0 / 0 / 0 |  |
| 0x088725A0 | 156 | `LogicalPad_update` | if disabled: clear, flags|=2; else translate(); if flags&2 held=new first; setButtons(new); flags&=~2 | 2 / 0 / 0 / 0 |  |
| 0x0887263C | 116 | `Pad_translateTable` | (a2:a3 = u64 held, t0 = table): OR of entry.logical for every entry with (held & entry.phys)==entry.phys; table ends at phys==0 | 2 / 0 / 0 / 0 |  |
| 0x088726B0 | 144 | `LogicalPad_translate` | table A (0x089C9738) on src->held, plus table at +0x4C if non-NULL; copies src stick to this+0x20; returns u64 | 2 / 0 / 0 / 0 |  |
| 0x08872740 | 92 | `LogicalPad_isHeld` | vslot +0x08: 0 if g.flags&2 else PadBase_isHeld | 0 / 0 / 0 / 0 | ptr at 0x089C96F4 |
| 0x0887279C | 92 | `LogicalPad_isPressed` | vslot +0x10, same gate | 0 / 0 / 0 / 0 | ptr at 0x089C96FC |
| 0x088727F8 | 92 | `LogicalPad_isReleased` | vslot +0x18, same gate | 0 / 0 / 0 / 0 | ptr at 0x089C9704 |
| 0x08872854 | 92 | `LogicalPad_isRepeat` | vslot +0x20, same gate | 0 / 0 / 0 / 0 | ptr at 0x089C970C |
| 0x088728B0 | 184 | `LogicalPad_getStick` | vslot +0x28: (0,0,0,1) if g.flags&2 else PadBase_getStick | 0 / 0 / 0 / 0 | ptr at 0x089C9714 |
| 0x08872970 | 48 | `LogicalPad_injectAndUpdate` | (lp, -, u64): RawPad_inject(lp->src, buttons); LogicalPad_update(lp) | 1 / 0 / 0 / 0 |  |
| 0x088729A0 | 32 | `PadMgr_getLogicalPad` | (i) -> g + 0xC0 + i*0x50 | 38 / 587 / 19 / 17 |  |
| 0x088729C0 | 88 | `PadMgr_disableLogicalPads` | LogicalPad_disable on both | 4 / 4 / 0 / 2 |  |
| 0x08872A18 | 88 | `PadMgr_enableLogicalPads` | LogicalPad_enable on both | 8 / 3 / 0 / 2 |  |
| 0x08872A70 | 36 | `PadMgr_setMapSet` | (set) -> PadMgr_setMapSetImpl(g, set) | 1 / 0 / 0 / 0 |  |
| 0x08872A94 | 72 | `LogicalPad_dtor` | deleting dtor | 0 / 0 / 0 / 0 |  |
| 0x08872ADC | 40 | `NullPad_ctor` | PadBase_ctor only | 0 / 0 / 0 / 0 |  |
| 0x08872B04 | 68 | `NullPad_update` | stick = zero vector 0x09AF84C0; setButtons(0) | 1 / 0 / 0 / 0 |  |
| 0x08872B48 | 24 | `PadMgr_getNullPad` | (i) -> g + 0x160 + i*0x40 | 2 / 0 / 1 / 0 |  |
| 0x08872B60 | 52 | `NullPad_dtor` | deleting dtor | 0 / 0 / 0 / 0 |  |
| 0x08872B94 | 164 | `PadMgr_ctor` | array-constructs raw[2], logical[2], null[2]; RawPad_ctor(g+0x1E0); mapSet=-1; +0x254=0; deadZone=100, repeatDelay=30, repeatInterval=4; flags=0 | 1 / 0 / 0 / 0 |  |
| 0x08872C38 | 184 | `PadMgr_dtor` | registered as atexit destructor (record at 0x089D658C) | 0 / 0 / 0 / 0 | ptr at 0x089D6594 |
| 0x08872CF0 | 152 | `PadMgr_pollRawPads` | if flags&1: (flags&8 ? RawPad_reset : RawPad_poll(.,0)) on g+0x1E0; else RawPad_poll(raw[i], i) for i=0,1 | 1 / 0 / 0 / 0 |  |
| 0x08872D88 | 100 | `PadMgr_updateLogicalPads` | skipped if flags&1; else LogicalPad_update on both | 1 / 0 / 0 / 0 |  |
| 0x08872DEC | 100 | `PadMgr_updateNullPads` | skipped if flags&1; else NullPad_update on both | 1 / 0 / 0 / 0 |  |
| 0x08872E50 | 184 | `PadMgr_setMapSetImpl` | (g, set): logical[i] maskState(0xFFFFF); logical[i].map = ((void**)0x089D5FF8)[set*2+i]; g+0x240 = set | 2 / 0 / 0 / 0 |  |
| 0x08872F08 | 136 | `PadMgr_readTask` | "PAD_MANAGER::ReadTask": pollRawPads, updateLogicalPads, updateNullPads, then flags bit2 = raw[0].isHeld(0x10000) | 1 / 0 / 0 / 0 |  |
| 0x08872F90 | 244 | `PadMgr_init` | sceCtrlSetSamplingMode(1); sceCtrlSetSamplingCycle(0x411B); registers readTask (priority 0x3A98); copies default action table to 0x08B7F380/0x08B7F560; logical[i].src=&raw[i], .index=i; setMapSetImpl(g,0) | 1 / 0 / 0 / 0 |  |
| 0x08873084 | 24 | `PadMgr_isMuted` | (g.flags & 2) != 0 | 5 / 0 / 0 / 0 |  |
| 0x0887309C | 1020 | `PadMgr_applyControlOptions` | (player, moveOnAnalog, vertOpt, horizOpt, forceCamOnAnalog): rewrites the phys column of the 8 direction rows of that player's action table | 3 / 0 / 0 / 0 |  |

Not in the inventory but part of the module: 0x08871DA4 (`PadBase_isLogical`, returns 0), 0x08872968 (`LogicalPad_isLogical`, returns 1).

External functions that drive the module:

| Address | Module | Role |
|---|---|---|
| 0x088769CC | main | static initialisers; constructs g, stores 0x089D6008 |
| 0x0893354C | main | boot: `PadMgr_init(g)` at 0x0893375C, `PadMgr_setMapSet(1)` at 0x08933764 |
| 0x088EC010 | main | a manual frame pump that calls `PadMgr_readTask` directly (0x088EC070) |
| 0x08867800 | main | `sceImposeGetLanguageMode`; sets byte 0x089D5F1D = 1 when the system confirm button is CROSS |
| 0x088FB3C0, 0x0891468C, 0x0891813C | main | callers of `PadMgr_applyControlOptions` (section 3.2) |
| 0x0888948C | main | task "FIELD::ReplayRecordAndPlayTask": 0x088CA680 (record) or 0x088CA9A8 (play) |
| 0x088892C8 -> 0x088892E8 | main | task "ExecPadInitByChangeTask": swallows edges when a character's pad changes type |
| 0x08889BAC | main (called from menu 0x08A40DE8, 0x08A49A20) | start replay playback: `RawPad_inject(system, {0,0})`, battle manager mode = 9 |
| 0x0890C640 / 0x0890C7B0 | main | ad-hoc input sync start / stop: `PadMgr_setInjectMode(1 / 0)` + `PadMgr_resetRawPads` |
| 0x0890D664 / 0x0890DF58 | main | ad-hoc input sync per-frame, role 0 / role 1 (dispatched by 0x0890C5BC on netobj+0x40) |
| 0x0890CE58 / 0x0890D090 | main | pack SceCtrlData -> u16 / unpack u16 -> SceCtrlData |
| 0x08B13484 | battle | reads the hardware for L+R+START+SELECT while pads are network-driven (section 5) |
| 0x08B3F7F4, 0x08B40290 | battle | camera controllers that test the four camera logical bits |

---

## 3. Per-frame flow, remap layer, bit meanings

### 3.1 Update order

V: `PadMgr_readTask` (0x08872F08) is the only caller of 0x08872CF0. It is registered in `PadMgr_init` with
`0x088823C4(list, -1, 0x3A98, 0x08872F08, "PAD_MANAGER::ReadTask")` where `list = *(*(0x089D69A8) + 0x2000)`, and is also
called directly from 0x088EC010. Each call does:

1. `PadMgr_pollRawPads` (0x08872CF0)
   - normal (`flags&1 == 0`): `RawPad_poll(&raw[0], 0)`, `RawPad_poll(&raw[1], 1)`.
   - replay playback (`flags&1`): `RawPad_poll(&system, 0)` (or `RawPad_reset(&system)` if `flags&8`); raw[] untouched.
2. `PadMgr_updateLogicalPads` (0x08872D88): `LogicalPad_update` x2, skipped when `flags&1`.
3. `PadMgr_updateNullPads` (0x08872DEC), skipped when `flags&1`.
4. HOME bit -> `flags` bit 2.

`RawPad_poll(pad, index)` (0x08871E24):

```
vec = {127.5, 127.5, 0, 0}; buttons = {0,0}
if (pad->mode == 1)      { vec.x = inject.lx; vec.y = inject.ly; lo = inject.buttons; }
else if (index == 0)     { sceCtrlReadBufferPositive(&data, 1);
                           if (data.Buttons & 0x20000 /*HOLD*/) data.Lx = data.Ly = 0x80;
                           vec.x = data.Lx; vec.y = data.Ly; lo = data.Buttons; }
buttons |= applyDeadZone(pad->stick, vec);          /* analog-direction virtual bits */
if (*(u8*)0x089D5F1D == 0) { CIRCLE -> confirm bit, CROSS -> cancel bit } else { CROSS -> confirm, CIRCLE -> cancel }
PadBase_setButtons(pad, buttons);
```

`PadMgr_applyDeadZone` (0x08872138): per axis `v = byte - 127.5`; if `|v| <= 100` -> 0, else `(|v| - 100) / 27.5` with sign;
a nonzero axis sets its direction bit. Then `stick = {x/len, y/len, 0, min(len, 1)}` (VFPU at 0x08872294..0x088722C0).

`PadBase_setButtons` (0x08871BD0): `pressed = new & ~old`, `released = old & ~new`; auto-repeat: on a new press
`repeat = pressed, timer = repeatDelay(30)`; while held `timer -= max(*(float*)0x089D5EAC, 2.0f)`; at `timer <= 0`
`repeat = held, timer = repeatInterval(4)`. (0x089D5EAC is the global frame step written by 0x08864DD8 - relevant to a frame-rate mod.)

`LogicalPad_update` (0x088725A0) -> `LogicalPad_translate` (0x088726B0):
`logical = translate(src->held, tableA) | (actionMap ? translate(src->held, actionMap) : 0)`; the stick vector is copied verbatim
from `src`. An entry matches when **all** its phys bits are held (`(held & phys) == phys`, 0x08872668..0x08872674), so combo rows
(e.g. R+CROSS) fire together with their single-button rows.

### 3.2 The mapping tables and where they are edited

- V: **Table A**, const, 0x089C9738, 20 rows, always applied (menu/system bits, logical bits 0..19).
- V: **Table B** (game actions), 29 rows + terminator = 0x1E0 bytes. Default image in rodata 0x089C9888; `PadMgr_init` copies it
  (0x08873020, memcpy 0x08935524) to RAM **0x08B7F380 (player 1) and 0x08B7F560 (player 2)**.
- V: Which table B a logical pad uses comes from the pointer table at 0x089D5FF8: `{NULL, NULL, 0x08B7F380, 0x08B7F560}`,
  indexed `[mapSet*2 + i]` by 0x08872E50. `PadMgr_setMapSet` has exactly one caller (0x08933764, argument 1), so after boot
  mapSet is always 1.
- V: The only code that writes the RAM tables after the copy is `PadMgr_applyControlOptions` (0x0887309C). There is no general
  button-remap feature; it only re-points the phys column of the eight direction rows:

  `0x0887309C(player, moveOnAnalog, vertOpt, horizOpt, forceCamOnAnalog)`

  | logical row | moveOnAnalog && !force | otherwise |
  |---|---|---|
  | 0x02000000 | vertOpt ? D-pad DOWN : D-pad UP | vertOpt ? analog DOWN : analog UP |
  | 0x04000000 | vertOpt ? D-pad UP : D-pad DOWN | vertOpt ? analog UP : analog DOWN |
  | 0x08000000 | horizOpt ? D-pad LEFT : D-pad RIGHT | horizOpt ? analog LEFT : analog RIGHT |
  | 0x10000000 | horizOpt ? D-pad RIGHT : D-pad LEFT | horizOpt ? analog RIGHT : analog LEFT |
  | 0x20000000 / 0x40000000 / 0x80000000 / {0,1} | moveOnAnalog ? analog UP/DOWN/LEFT/RIGHT : D-pad UP/DOWN/LEFT/RIGHT (force flag ignored) | |

  Callers: 0x088FB3C0 (applies saved settings for both players from the u16 at 0x09AFD960 + player*4:
  bit 0x20 -> moveOnAnalog, bit 0x1 -> vertOpt, bit 0x2 -> horizOpt, bit 0x40 clear -> force), 0x0891468C (options screen:
  options 4, 1, 2 read through 0x088F47E0, force = 0), 0x0891813C (not examined in detail).
- I: so 0x02000000..0x10000000 are camera pitch/yaw keys and 0x20000000..{0,1} are the four move directions.
  Supported by battle 0x08B3F7F4, where 0x08000000/0x10000000 add/subtract a yaw angle and 0x02000000/0x04000000 a pitch value.

Table A (0x089C9738):

| phys | logical | | phys | logical |
|---|---|---|---|---|
| SELECT+START+L+R {0x309,0} | 0x1 | | analog LEFT {0,0x20} | 0x400 |
| START | 0x2 | | analog RIGHT {0,0x8} | 0x800 |
| confirm {0,0x1} | 0x4 | | TRIANGLE | 0x1000 |
| cancel {0,0x2} | 0x8 | | CIRCLE | 0x2000 |
| UP | 0x10 | | CROSS | 0x4000 |
| DOWN | 0x20 | | SQUARE | 0x8000 |
| LEFT | 0x40 | | L | 0x10000 |
| RIGHT | 0x80 | | R | 0x20000 |
| analog UP {0,0x4} | 0x100 | | SELECT+START {0x9,0} | 0x40000 |
| analog DOWN {0,0x10} | 0x200 | | SELECT | 0x80000 |

Table B defaults (0x089C9888). Logical masks in the first group are low-word, the rest high-word:

| phys | logical | | phys | logical |
|---|---|---|---|---|
| CROSS | {0x00100000,0} | | R+CROSS 0x4200 | {0,0x10} |
| CIRCLE | {0x00200000,0} | | R | {0,0x20} |
| SQUARE | {0x00400000,0} | | R+CIRCLE 0x2200 | {0,0x40} |
| TRIANGLE | {0x00800000,0} | | R+TRIANGLE 0x1200 | {0,0x80} |
| DOWN | {0x02000000,0} | | R+SQUARE 0x8200 | {0,0x100} |
| UP | {0x04000000,0} | | SQUARE | {0,0x200} |
| LEFT | {0x08000000,0} | | CIRCLE | {0,0x400} |
| RIGHT | {0x10000000,0} | | CIRCLE | {0,0x800} |
| analog UP | {0x20000000,0} | | SQUARE | {0,0x1000} |
| analog DOWN | {0x40000000,0} | | START | {0,0x2000} |
| analog LEFT | {0x80000000,0} | | L+R+SELECT 0x301 | {0,0x4000} |
| analog RIGHT | {0,0x1} | | L+R 0x300 | {0,0x20000} |
| L | {0,0x2} | | L+CIRCLE 0x2100 | {0,0x40000} |
| SELECT | {0,0x4} | | L+SQUARE 0x8100 | {0,0x80000} |
| CROSS | {0,0x8} | | | |

V: logical bits with no table row: low 0x01000000; high 0x8000, 0x10000 and 0x100000 upward.
V: table B has no spare rows (0x08B7F380 + 0x1E0 = 0x08B7F560, and P2's table ends at 0x08B7F740). Adding rows means relocating
the tables (pointer table 0x089D6000/0x089D6004, the memcpy at 0x08873008..0x08873024 and the base address at 0x088730D8/DC) or
OR-ing extra logical bits after 0x088726B0.

### 3.3 Raw high-word (virtual) bits

| raw hi bit | meaning | produced at |
|---|---|---|
| 0x01 | confirm (CIRCLE if byte 0x089D5F1D == 0, CROSS if 1) | 0x08871F7C / 0x08871FD8 |
| 0x02 | cancel (the other one) | 0x08871FA8 / 0x08872004 |
| 0x04 | analog UP (y byte below centre) | 0x08872138 |
| 0x08 | analog RIGHT | 0x08872138 |
| 0x10 | analog DOWN | 0x08872138 |
| 0x20 | analog LEFT | 0x08872138 |
| 0x40.. | never produced | |

Cross-checked three ways: `RawPad_inject` (0x08872304) rebuilds lx/ly from these bits, the net packer 0x0890CE58 and unpacker
0x0890D090 agree, and table A maps them to the menu's analog-direction bits.

---

## 4. The SceCtrlData buffer in 0x08871E24 and where to hook

Frame is 0x50 bytes (`addiu sp,sp,-0x50`):

| sp offset | content | read by |
|---|---|---|
| +0x00..+0x0F | float vec[4] passed to the dead-zone routine | |
| +0x10 | SceCtrlData.TimeStamp | never |
| +0x14 | SceCtrlData.Buttons | 0x08871EF4 (HOLD test), 0x08871F3C (-> s0, becomes raw.held.lo) |
| +0x18 | Lx (overwritten with 0x80 at 0x08871F0C when HOLD) | 0x08871F14 |
| +0x19 | Ly (overwritten at 0x08871F10 when HOLD) | 0x08871F1C |
| +0x1A | **Rx** (buffer+10) | never |
| +0x1B | **Ry** (buffer+11) | never |
| +0x1C..+0x1F | spare bytes (buffer+12..15) | never |
| +0x20 | saved f20 | |
| +0x24..+0x48 | saved s0..s7, fp, ra | |

The buffer is only filled on the path 0x08871EE0 -> 0x08871EE8..0x08871F40. On the inject path (mode==1) and for index != 0 the
bytes at sp+0x10.. are stale stack, so a hook must sit inside that path.

Recommended hook points (both are single-word patches, both reached only after a real hardware read):

- **A. 0x08871EF4** `8FA40014 lw a0,0x14(sp)` -> `jal hook`. The following instruction 0x08871EF8 `lui a1,0x2` is a legal delay slot
  and still runs, so the hook is entered with a1 = 0x00020000. On return (to 0x08871EFC `and a0,a0,a1`) the hook must leave
  **a0 = Buttons** (re-read from 0x14(sp) after any modification) and **a1 = 0x00020000**, with sp unchanged. Because Buttons is
  re-read at 0x08871F3C, editing the word at sp+0x14 in the hook changes what the game sees.
- **B. 0x08871EEC** `0E2716AA jal 0x089C5AA8` -> `jal wrapper` (delay slot `ori a1,zero,1` unchanged, a0 = sp+0x10 on entry).
  The wrapper calls the import itself and post-processes the buffer.

Register state at 0x08871EF4 (after the import returned):

- Free: v0 (return value, unused by the game), v1, a0, a2, a3 (and a1 for hook B; see hook A above), t0..t9, at, ra (saved at sp+0x48, next written by the jal at
  0x08871F34), f0..f19, HI/LO. a1 no longer holds the pad index (it was set to 1 for the call); on this path the index is 0.
- Must be preserved: sp; **fp = the RawPad being polled** (0x08C15B90 for player 1, 0x08C15D70 for `system` during replay
  playback - use this to tell them apart); s2:s3 = zero mask, s4:s5 = CROSS mask, s6:s7 = CIRCLE mask (all live until
  0x08872014); f20 = 0.0f (used at 0x08871F18/0x08871F20); s0, s1 are dead here (reloaded at 0x08871F3C/40) but are restored
  from the frame at exit, so they are usable as scratch.

Notes for the capture code: mirror the HOLD rule (Buttons & 0x20000 -> treat right stick as 0x80,0x80); bytes sp+0x1C..0x1F
are available at the same point.

---

## 5. The other two sceCtrlReadBufferPositive call sites

**main 0x0890D6EC, in 0x0890D664** - V: ad-hoc netplay input sender. It is one of two per-frame handlers selected by 0x0890C5BC;
its packets go through 0x0890569C -> import 0x089C59F0 and 0x08905728 -> import 0x089C5A08 (both sceNetAdhoc). The buffer is at
sp+0x00 of a 0xA0 frame. Flow: read hardware -> `0x0890CE58(netobj, &data)` packs to u16 -> appended to a short history
(`0x0890D498(buf, word, 8)`) and sent in 0x24-byte packets -> when every player's word for frame `netobj+0x48` has arrived, `0x0890D090` unpacks each into a
16-byte SceCtrlData on the stack and the loop at 0x0890DE30 copies it to `raw[i].inject` (+0x40..+0x4F). The pads are in
mode 1 for the whole session (0x0890C784), so `RawPad_poll` consumes those values instead of the hardware.

u16 layout (0x0890CE58 / 0x0890D090): bit0 LEFT, 1 RIGHT, 2 UP, 3 DOWN, 4 CIRCLE, 5 CROSS, 6 SQUARE, 7 TRIANGLE, 8 L, 9 R,
10 START, 11 SELECT, 12 analog LEFT, 13 analog RIGHT, 14 analog UP, 15 analog DOWN. Only Buttons, Lx, Ly are read
(0x0890CE5C, 0x0890CF78, 0x0890CF88); the unpacker writes Lx/Ly as 0x00/0x80/0xFF.

Needs patching? Only if right-stick input is to work in netplay. The local hardware never reaches `raw[0]` directly in a net
session, so a capture hook in 0x08871E24 does not fire there at all (mode==1 path). If the mod folds the right stick into
existing bits, do it here too - the natural point is 0x0890D6F8 `0E243396 jal 0x0890CE58` (a1 = SceCtrlData*), or the import
call at 0x0890D6EC.

0x0890DF58 (role 1) never reads the hardware; it receives 0xDC bytes per player and injects the same way (0x0890E594).
I: that is a spectator/receive-only role.

**battle 0x08B134E8, in 0x08B13484** - V: buffer at sp+0x00, only `Buttons` is read (0x08B134F0) and tested for
0x100, 0x200, 0x8, 0x1 all set (L+R+START+SELECT); if so and the net object 0x089D8364 exists it calls 0x089072AC(net, 1)
(which leaves inject mode via 0x0890C7B0) and 0x08906F6C. The function is reached through a pointer at battle 0x08B13B58.
I: an "abort network battle" combo that must bypass the synchronised pads. No patch needed for right stick or new buttons.

---

## 6. Injected / replay / network input, and what it means for a right stick

V - the three input sources of a raw pad:

| situation | who feeds `raw[i]` | hardware goes to |
|---|---|---|
| normal play | `RawPad_poll` from sceCtrl (raw[0] only) | raw[0] |
| ad-hoc battle | net code writes `raw[i].inject`, `mode = 1` on both pads for the session | the u16 packer in 0x0890D664 |
| replay playback (`g->flags & 1`) | 0x088CA9A8 -> `LogicalPad_injectAndUpdate(pad, mask)` per player per frame (temporarily mode 1) | `system` pad (g+0x1E0) |

V - replay format (recorder 0x088CA680, player 0x088CA9A8, mask table 0x089D7758): for each player's LogicalPad it queries the
**raw** pad (`lp->src`) with 15 masks: UP, DOWN, LEFT, RIGHT, analog UP, analog DOWN, analog LEFT, analog RIGHT, CIRCLE, CROSS,
SQUARE, TRIANGLE, L, R, SELECT. Bit 15 is an "edges were reset" flag. A record is 3 bytes: the 16-bit state plus an 8-bit run
length (0x088CA564), at most 0x1518 records per player. START is not recorded. On playback the stick is rebuilt as 0/127/254
per axis by 0x08872304.

V - consequences:

- Nothing outside Buttons/Lx/Ly survives either path. `inject.rsrv` (+0x4A..) is not initialised by the replay path and is
  filled with stack garbage by the net path; a mod that extends `RawPad_poll` to read `inject` bytes 10/11 in mode 1 must also
  make 0x0890D090 and 0x08872304 write them.
- Both formats have zero spare bits, so carrying analog right-stick values means changing the packet layout (breaks play
  against unmodified copies) and the replay file layout.
- The camera is not read from the raw pad. It is read from the per-player pad object through the logical camera bits, which in
  a net battle and in a replay are reconstructed from the transmitted/recorded raw bits.

I - is the camera purely local? Not established. The camera controller takes its input from a pad object that is
network/replay-driven, which is what a deterministic lockstep design needs if anything in the simulation depends on camera
orientation (camera-relative movement would). I did not trace whether character movement reads the camera yaw, nor whether a
camera context exists for the remote player.

I - safest design given the above: convert the right stick into the raw bits that already drive the camera (the D-pad bits
0x10/0x20/0x40/0x80 in `Buttons` under the default "move on analog" setting) at the two hardware read sites (0x08871EF4 and
0x0890D6F8). Replays and netplay then keep working unchanged, at the cost of a digital camera. A truly analog camera
(patching the rate in battle 0x08B3F7F4 / 0x08B40290) is safe only where the camera is provably local - the replay viewer is
the clear case, since it is driven by the `system` pad, which is live hardware.

For L2/R2/L3/R3: V: any extra bit present in `Buttons` already lands in `raw[0].held.lo` untouched; it then needs a table row
(or an OR after 0x088726B0) to become a logical bit. The same replay/netplay limit applies: a new button is invisible to both
unless it is mapped onto an existing recorded bit.

---

## 7. Not determined statically

1. Whether camera orientation feeds the battle simulation (decides whether un-synced right-stick camera input desyncs netplay/replays).
2. Execution order between "PAD_MANAGER::ReadTask" (priority 0x3A98, list `*(0x089D69A8)+0x2000`), the net update (0x08907F14 ->
   0x0890C5BC) and "FIELD::ReplayRecordAndPlayTask" (priority 0x12C on another list), and the task scheduler's priority direction.
3. Any writer of `g->flags` bit 1 (mute). None exists as a direct access; it may be dead debug code.
4. Which `Buttons` bits PPSSPP delivers for L2/R2/L3/R3. The saved sceCtrl.cpp masks Buttons with `CTRL_MASK_USER`, defined in a
   header that was not available. The same file does copy `analog[1]` (right stick, centre 0x80) into buffer+10/+11.
5. Gameplay names of the table B action bits; only the physical combinations are verified.
6. Meaning of the fifth argument of 0x0887309C / save flag 0x40, and of option ids 1, 2, 4; caller 0x0891813C was not read.
7. Purpose of g+0x254 and LogicalPad+0x48 (written, never read).
8. Whether any code indexes a pad pointer directly after it has been stored in another object (only vtable use and the one
   +0x40 read in 0x088CA680 were found).
9. The exact role of 0x0890DF58, and whether `null[1]` is ever requested.
10. The extra overlay was only scanned for call sites (17 calls to 0x088729A0, 2 each to 0x088729C0/0x08872A18), not read.
11. Where patch code can live (no code cave was looked for).
