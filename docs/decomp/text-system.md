# The game's text system ("SYSTEM_FONT")

25 November 2026, USA disc (ULUS10566), main 0x0887A000-0x08883000 and the code around it. Written for the native
text fix (the host drawing the game's text from a glyph atlas) and for the decomp. Every statement is marked:

- **traced**: seen in the port (the existing build `tools/local/port/build-d012-skills`, test data
  `build/port/data-stutter`, the shop route of `build/scratch/text-system/run_shop.py`); the run is named.
- **read**: from the retail code only (assembly and Ghidra pseudocode through `tools/dc.py context`).

Layouts: [include/main/text_types.h](../../include/main/text_types.h) (C and C++, offsets asserted; compiles under SNC
and host compilers). Readable C: `build/scratch/text-system/drafts/` (create/release/clone `text_new_0887d868.c`,
the two frame passes `text_frame_passes_0887de04.c`, drawing `text_draw_0887bc2c.c`, layout `text_layout_0887f294.c`,
the bitmap-font path `bitmap_text_0887e9cc.c`).
Runs: `build/scratch/text-system/runs/` (font1, pc_d868nc, pc_ba5c, ge_index, ge_list; second pass: pc_bc2c,
acc_d868, acc2_d868, abil_d868, abil2_d868, gfx/ and gfx2/; third pass: pc_zmsk, ge_zmsk0-ge_zmsk4). The log of the
work: `build/scratch/text-system/progress-log.md`.

Second pass (agent90, 25 November evening): the statements the native fix depends on were checked instruction by
instruction in the retail assembly of 0x0887BC2C, 0x0887F294, 0x0887D868, 0x0887DAC0, 0x0887DB1C, 0x0887E534,
0x0887E278, 0x0887DE04 and 0x0887DBD4 (a **read** statement below has been checked there unless it says
"Ghidra"); corrections are marked **corrected**. New traces: every 0x0887BC2C call of shop frames (pc_bc2c), the
accessory and ability lists, and the battle and story routes.

Third pass (agent92, 25 November night): 0x0887CA50, 0x0887E9CC and 0x0887EF84 checked in the assembly as well, so
no statement below rests on Ghidra alone unless it says so; the depth-write state at text draws (traced through
every display list from boot to the shop), where the 0x001C strings are (a read-only scan of the disc's message
files, and one traced on screen), which control codes the files use, who links the shop's numbers into the layout,
and the budget numbers for the fix (section 6) were added; superseded text was removed rather than kept beside its
correction.

Reviews (26 November, `build/scratch/text-system/review.md`): two independent checks against the assembly and the
runs; their fixes are marked **corrected (review)** / **corrected (review 2)** and **added (review)** / **added
(review 2)**. The second covered the variable and image codes of 0x0887F294, 0x0887CA50, the manager constructor,
the font init and the binding tables.

## In one paragraph

A text is a 0x58-byte object taken from a pool of 290 in a static manager ("SYSTEM_FONT", 0x08BEE94C, pointer at
0x089D6998). Making one (0x0887D868) lays the UTF-16 string out with the PSP font library (sceLibFont), cuts it
into *runs* (one per line, colour, size or inline image) and rasterises the whole string into one fresh 4-bit
buffer (`(widest line + 31) & ~31` x 26 per line) from the heap at manager+0x14 ("SYSTEM FONT T", or a screen's own
heap swapped in there; **corrected (review)**: not the "SYSTEM FONT" heap). Every frame the Draw pass
(0x0887DE04) or the layout node the text is attached to (0x088D9E48) draws each visible text with 0x0887BC2C: the
buffer becomes a CLUT4 texture (CLUT = white with an alpha ramp) and each run one TRIANGLE_FAN quad, coloured by
the vertex colour, usually scaled to about half size. A text never changes its string: the game releases it
(0x0887BA5C) and makes a new one; the buffer is freed two Update passes later. There is no glyph atlas for these
glyphs: every glyph is drawn into the buffer of its string (**traced**, 7263 of 7263). Most visible menu text is
owned by layout nodes and drawn by them (0x088D9E48) at the node's z; a shadow is a second text object sharing the
buffer, drawn first at (+1, +1) in a dark colour (**traced**). A second, small path draws byte strings that start
with 0x001C from a bitmap font image in the game's files, one quad per glyph: on this disc these are the battle
banners and tips (HP attack names, the EX Mode prompt, Lightning's Paradigm tip), stored in the fighters' moveset
archives (**read**; **traced** on screen in a fight, section 3).

## 1. Text objects

### The manager (`D012TextManager`, 0xF2C4 bytes)

Static object at 0x08BEE94C (guard byte 0x08BEE948), built once by 0x0887E278 from the boot function 0x088769CC,
which stores its address at 0x089D6998 (0x08877208) (**read**). The font init 0x08881954 (called from 0x0893354C)
fills the font part and rebuilds the pools (**read**; NewLib/Open/GetFontInfo **traced** from it: ra 0x08881B0C,
0x08881C10, 0x08881C50, font info written to 0x08BEE9AC = manager + 0x60).

| Offset | Type | Meaning | Written by |
| --- | --- | --- | --- |
| +0x00 | u8 | line height: `((0x480 - font descender) >> 6) + 3` = 26 with the port's font info (**traced** 26-row lines) | 0x08881954 |
| +0x01, +0x02 | u8 | max glyph height, width (font info >> 6) | 0x08881954 |
| +0x03 | u8 | 20 (constant): trailing icon id of an item name (code 0x18) when `0x088C7FE4(item)->+0x04 & 2` (else 21; **corrected (review 2)**: not the item's kind) | 0x08881954 |
| +0x04 | s16 | pixels per byte = 8 / bpp = 2 | 0x08881954 |
| +0x08, +0x0C | ptr, heap | 0x2EC00-byte block and the heap "SYSTEM FONT" over it: the font library's memory (NewLib alloc/free 0x0887D6E0/0x0887D750), the bitmap font, the metrics cache (**corrected (review)**: not the text buffers) | 0x08881954 |
| +0x10, +0x14 | ptr, heap | 0x19000-byte block and heap "SYSTEM FONT T": **the text buffers** (**corrected (review)**: 0x0887D7B8 allocates from the heap at +0x14 and keeps that heap in the block's 16-byte header, which 0x0887D828 frees through). Screens swap their own heap in: 0x0887E630 makes "SYSTEM FONT T2" over a caller's block, 0x0887E798 installs a caller's heap (e.g. "SYSTEM FONT T3" from 0x0887E74C), so texts made meanwhile get their buffers there. **added (review 2)**: the font init also takes the font list (`sceFontGetNumFontList` x 0xA8 bytes, one SceFontStyle per font) from this heap through 0x0887D7B8 and gives it back (0x0887D828, 0x08881C64) once sceFontGetFontInfo succeeded; on its error paths the list stays allocated | 0x08881954, 0x0887E630, 0x0887E798 |
| +0x18, +0x1C | ptr, heap | the +0x10/+0x14 pair while T2 is in (zeroed by the font init) | 0x0887E630 |
| +0x20, +0x24 | ptr, heap | the +0x10/+0x14 pair saved by 0x0887E798(mgr, heap) (which then sets +0x10 = 0, +0x14 = heap), put back and cleared by 0x0887E7B4 (**read**, assembly, review) | 0x0887E798 |
| +0x28 | ptr | the 0x39C4-byte bitmap font block | 0x08881954 |
| +0x2C | ptr | 0x800-byte metrics cache: `{s16 left, s16 top, s32 advance}` for the first 256 characters of the string being made | 0x08881954, 0x0887F294 |
| +0x30, +0x38 | 2 x `D012TextImageSet` | inline image sets `{image, 0, cell 18, width}` (the font init sets both to `{0, -, 18, 128}`; installed by 0x0887E090 from 0x088EBAA8 (set 0, width 128) and 0x088FD928 (set 1, width 256); 0x0887E0DC puts a set back to `{0, -, 18, 128}`; review 2) | 0x08881954, 0x0887E090 / 0x0887E0DC |
| +0x40 | int | libfont.prx module | 0x08881954 |
| +0x44, +0x48 | handle, int | sceFontNewLib handle (params: 5 fonts, no cache, alloc/free 0x0887D6E0/0x0887D750, open/close/read/seek 0x0887D790/0x0887D79C/0x0887D7A4/0x0887D7B0; **traced** numFonts 5, alloc/free), last font error | 0x08881954, 0x0887F294 |
| +0x4C | 5 handles | opened fonts; [0] is the one used (**traced** Open index 0). **added (review 2)**: the open loop runs once, so only [0] is ever set (from the first entry of the font list, 0x089C58E8 then sceFontOpen mode 0); a failed open, sceFontGetFontInfo error or NewLib failure returns before the pools are rebuilt and before the two tasks exist | 0x08881954 |
| +0x60 | `D012FontInfo` (0x108) | sceFontGetFontInfo; the ascender (+0x68) is overwritten with 0x480 (18 px): the baseline of each line | 0x08881954 |
| +0x168 | list | active texts {head, tail, count}; draw order = creation order | 0x0887D868, 0x0887E534, 0x0887DB1C |
| +0x174 | list | text buffers {head, tail, count} | 0x0887F294, 0x0887DBD4 |
| +0x180 / +0xA90 / +0x6E40 | | text pool: 290 ring nodes, 290 objects x 0x58, take / give cursors | 0x0887E278 |
| +0x6E48 / +0x74B0 / +0xA7F0 | | buffer pool: 205 nodes, 205 records x 0x40, take / give | 0x0887E278 |
| +0xA7F8 / +0xB6A8 / +0xE2B8 | | run pool: 470 nodes, 470 runs x 0x18, take / give | 0x0887E278 |
| +0xE2C0 / +0xE810 / +0xF2B0 | | inline image pool: 170 nodes, 170 x 0x10, take / give | 0x0887E278 |
| +0xF2B8, +0xF2BC | task | "SYSTEM_FONT::Update" (0x0887E4F4, priority 0x88B9), "SYSTEM_FONT::Draw" (0x0887E514, priority 0x11196) | 0x08881954 |
| +0xF2C0 | ptr | bitmap font (= +0x28 once built) | 0x08881954 |

Pools (**read**): each pool is a closed ring of 8-byte nodes `{item, next}`. Take: `item = take->item; take->item = 0;
take = take->next`. Give: `give->item = item; give = give->next`. Nothing checks for an empty pool: a 291st text, 206th
buffer or 471st run would take a null item.

### The text object (`D012Text`, 0x58)

| Offset | Type | Meaning | Written by |
| --- | --- | --- | --- |
| +0x00 | ptr | source: `D012TextBuffer` (flag 0x2000 clear) or `D012BitmapText` (set) | 0x0887F294, 0x0887BA48 |
| +0x04 | s16 | letters shown; runs whose first letter is beyond it are not drawn | 0x0887B7E8 (= all), 0x0887BAD4 (+1 per step) |
| +0x06, +0x07 | s8 | frames per letter (0: no reveal), frame counter | 0x0887B6A0 (0), callers |
| +0x08 | s16 | line pitch (0: line height x scale_y x largest size in the line) | 0x0887B900 |
| +0x0A | s16 | alpha override (-1 none, else 0..255 replaces every vertex alpha) | 0x0887B8A8 (clamps), init -1 |
| +0x0C | u32 | ABGR vertex colour (0x80 = full: FRAGMENT_2X doubles it) | 0x0887B820 (arg 4 of 0x0887D868), 0x0887B828 |
| +0x10 | u32 | flags, below | |
| +0x14, +0x16 | s16 | screen x, y (y stored minus 1 unless bitmap text) | 0x0887B748, 0x0887B778 (from a matrix) |
| +0x18, +0x1C | float | scale x, y (1.0) | 0x0887B7D0 |
| +0x20, +0x24 | float | base scale x, y (1.0): images are drawn at scale / base scale when flag 0x1 | 0x0887B7DC |
| +0x28 | 4 x {s16 x, s16 y, s16 id, s16} | positions marked in the string by code 0x1F 101..200: the next glyph's bitmap top-left in the buffer (x64 / 64, y64 / 64). No bound: a 5th mark overwrites +0x48 | 0x0887F294, cleared by 0x0887BBC8 |
| +0x48, +0x50 | ptr | next, prev in the manager's list | |
| +0x4C, +0x54 | ptr | next, prev in a layout node's text list (node +0x30 head, +0x34 tail, +0x38 count) | 0x088D40BC and 0x088D8FD0 link, 0x088D8A98 unlinks |

Flags at +0x10 (**read**; init 0x0887B6A0 leaves 0x420):

| Bit | Meaning |
| --- | --- |
| 0x0001 | inline images drawn at scale / base scale |
| 0x0002 | every letter shown (0x0887B7E8); images then drawn without the reveal limit |
| 0x0004 / 0x0008 | x is the right edge / centre of each line (else left) |
| 0x0010 / 0x0020 / 0x0040 | y is the top / bottom (default) / middle of the block |
| 0x0080, 0x0100 | cleared with the alignment bits by 0x0887BA04; use not found |
| 0x0200 | drawn this frame: set by the Draw pass and by 0x088D9E48, cleared by the Update pass |
| 0x0400 | visible (default; 0x0887B924 sets, 0x0887B944 clears) |
| 0x0800 | no inline images, and run colours ignored (bitmap texts: images only) |
| 0x1000 | show once: drawn by the next pass even if not visible, then cleared by 0x0887BC2C |
| 0x2000 | bitmap-font text (code 0x1C strings) |
| 0x4000 | run/glyph colours ignored: the object's colour only |
| 0x8000 | skipped by the Draw pass: set when a text is linked into a layout node (0x088D40BC through binding tables, 0x088D8FD0, 0x088CCF74, 0x088CD754), which draws it (0x088D9E48) (**traced**: flags 0x8612/0x8616/0x8646/0x864A/0x8E4A on node texts) |

### The buffer record (`D012TextBuffer`, 0x40), runs and images

Buffer record: +0x00 width (= tbw), +0x02 texture height (power of two), +0x04 heap block, +0x08 pixels (= +0x04),
+0x0C letter count (inline images count as letters), +0x0E widest line (px, not rounded to 32), +0x10
`10 + sum of trunc(px x size)` over the runs closed before code 0x12/0x16 (all lines; 0x0887BB6C returns it x
scale_x as the text's width; 0x0887BC2C's cut-off), +0x12 1 + runs (read as a SIGNED byte by 0x0887BC2C),
+0x13 1 + line breaks,
+0x14/+0x18 a callback and argument called when freed (0x0887BA80 -> 0x0887A5F8 sets them, only while not shared),
+0x1C runs list, +0x28 refcount, +0x2C images list, +0x38/+0x3C manager list (**read**).

Run (`D012TextRun`, 0x18): width (26.6 while measuring, then px; **corrected**: the run before code 0x12/0x16 and
every run after it stay in 26.6), colour (top byte 0 = the object's), size (code 0x11; 1.0 for an image
placeholder and for the run right after it), row top (u8, wraps; not read by 0x0887BC2C), flags (0x01 new line
(code 0x0A only), 0x02 end (on the run before 0x12), 0x04 (on the run before 0x16), 0x08 image placeholder, 0x10
not drawn), first letter index, next, prev (**read**).

Inline image (`D012TextImage`, 0x10): next, prev, x, u, v, `cols << 4 | rows`, `line | set << 6` (**read**,
0x0887E114). Icon id -> cell: `u = ((id - 1) % per_row) * 18`, `v = ((id - 1) / per_row) * 18`, per_row = image
width / 18 (7 for set 0, 14 for set 1); ids 0x80 / 0x81 (confirm / cancel) become 0x2F / 0x31, swapped when the
byte 0x089D5F1D is set (the circle/cross layout).

Bitmap font (`D012BitmapFont`, 0x39C4) and its 28 text records (`D012BitmapText`, 500 bytes, 40 glyphs of 12 bytes
and an images list): see the header (**read**).

## 2. Lifecycle

### Creating

**0x0887D868** `text = func_0887D868(mgr, a1, str, colour, t0, vars, nvars, t3)` (**read**; 15 call sites in main,
165 in menu, 16 in battle):

| Arg | Meaning |
| --- | --- |
| a0 | the manager (`*(0x089D6998)`) |
| a1 | **unused** (passed to 0x0887F294 as a2, overwritten there before any read) |
| a2 | UTF-16 string, 0-terminated. First unit 0x001C: the rest is a byte string for the bitmap font |
| a3 | ABGR colour -> +0x0C (0xFFFFFFFF from the message-entry makers and the shop's number labels; 0 from 0x0887AD78/0x0887B3EC, whose callers then set it) |
| t0 | **unused** (0x0887F294 overwrites t0 before reading it) |
| t1, t2 | variables: an array of pointers and its length, for codes 0x13, 0x18, 0x19, 0x1A, 0x1D |
| t3 | **unused** (**corrected**: saved as a byte and passed on as t3, but 0x0887F294 never reads a2, t0 or t3) |

Steps: take an object, 0x0887B6A0 (defaults), 0x0887B820 (colour), then 0x0887F294 (or 0x0887E9CC(mgr+0xF2C0,
mgr+0x30, obj, str + 1, vars, nvars) for 0x001C). Returns 0 if that fails, after 0x0887DAC0 and giving the object
back; else appends it at the tail of the active list and returns it. **corrected** failure cases of 0x0887F294:
only a sceFontGetCharGlyphImage error (pass 2) fails, and then the buffer record is never put on the manager's
buffer list, so it and its block are never freed (a leak); a sceFontGetCharInfo error only skips that character
(error code kept at mgr+0x48). The string and the variables are read during the call only: **the object keeps no
pointer to its string** (**read**).

Other makers (**read**): 0x0887B3EC (block `index`, entry `number` of a multi-block text file, probably a "mess pak"
message pack, section 3; missing entry: the empty string 0x09B49088 with colour 0xFFFFFFFF), 0x0887AD78 and
0x0887AEAC (an entry of a text file, converting UTF-8
files with 0x0887A940), 0x0887D034 (a label slot of the 10-slot table at 0x09AEF804, created hidden), and
**0x0887E534**, which clones a text (copies the 0x58 bytes, buffer refcount + 1, appends at the TAIL of the active
list, so it draws after every older text; the node links +0x4C/+0x54 and all flags are copied as they are) so the
clone shares the buffer (**traced**: two 1-glyph buffers drawn 6 times each on the shop screen).

### Changing

There is no call that changes a text's string (**read**): every string change is a release and a new text. What
changes in place: position (0x0887B748, 0x0887B778), scale (0x0887B7D0, 0x0887B7DC), colour (0x0887B820/0x0887B828),
alpha (0x0887B8A8), line pitch (0x0887B900), alignment (0x0887B958/0x0887B970/0x0887B98C, 0x0887B9A4/0x0887B9C4/
0x0887B9E4, 0x0887BA04), visibility (0x0887B924/0x0887B934/0x0887B944), the letter-by-letter reveal (+0x06,
0x0887BAD4 each Update, 0x0887B7E8 to finish).

### Releasing

**0x0887BA5C(text)** -> 0x0887DB1C: buffer refcount - 1 (0x0887A534; bitmap records: 0x0887D68C, freed at 0), unlink
from the active list, object back to the pool (**read**). It does not unlink the object from a layout node's list
(+0x4C/+0x54: the owner must, 0x088D8A98 / 0x088D442C) and leaves its flags as they were until 0x0887B6A0. Call
sites: 642 in menu, 87 in main, 78 in battle, 9 in extra (**read**).
Layout code releases a node group's texts with 0x088D442C(nodes, labels, bindings): each binding's label released
and the node's text list cleared (**read**; **traced** 552 releases from 0x088D4498 in one scroll, run pc_ba5c).

### The buffer

- Allocated by 0x0887F294 through 0x0887D7B8 from the heap at manager+0x14 ("SYSTEM FONT T", or the heap a screen
  swapped in with 0x0887E630 / 0x0887E798; 16-byte header holding the heap), cleared to 0 (**read**; **corrected
  (review)**: the first passes said the "SYSTEM FONT" heap at +0x0C, which is the font library's). So the text
  buffers of a screen sit next to each other in a heap of their own. Buffer addresses climb through the scroll
  (**traced**, font1).
- Width: widest line in pixels rounded up to 32, or 1024 if wider (**read**; **traced** 32, 64, 96, 128, 160, 192,
  224, 256, 288 ... 736). Height: (line breaks + 1) x 26 (**traced** x26 and 384x78). Texture height: power of
  two >= height (32 for one line: **traced** tsize 0x0509). 4 bits per pixel, low nibble first, `width / 2` bytes
  per row (**traced**: bpl = width / 2).
- Lifetime: refcount 1 when made, + 1 per clone, - 1 per release. The Update pass (0x0887DBD4) turns refcount 0 into
  -1, and frees the block, the runs and images and the record at the next Update with refcount < 0 (**read**). So a
  released buffer stays readable for one more frame or two.
- The string is not kept; the buffer keeps only its runs (widths, colours, sizes, letter indices) and images (**read**).

## 3. Layout (0x0887F294, 0x0887E9CC)

### Encoding and control codes

Strings are UTF-16 (**read**; **traced**: codes such as 0x0043, 0x201C, 0x3013 passed to the font). Text files may be
UTF-8 (converted by 0x0887A940 into a static UTF-16 buffer 0x09B48C88, 1022 units) and code builds 8-bit strings that
0x0887DF14 converts (ASCII; two-byte codes through the table at 0x089D6964, else '?') (**read**).
Control codes are 0x0010-0x001F followed by their arguments (u16 units; argument bytes from the int table at
0x089C9D2C indexed by code); 0x0887F294's first pass dispatches 0x0A-0x1F through the jump table at 0x089C9E68
(0x0D tested before it); its second pass is an if-chain (**read**, assembly). **corrected**: units 0x01-0x09,
0x0B, 0x0C, 0x0E, 0x0F, 0x14 and a non-leading 0x1C have no case: they are measured and drawn as characters
(0x14's argument is not skipped).

Use in the data (**read**: a read-only scan of the disc's message tables, progress log 21:46 and 21:51): the 25
menu message packs ("mess pak" files, 22,422 strings) use 0x0A (18,202 times), 0x10 / 0x1B (5,213 / 5,211), 0x15
(9,066 icons), 0x1E (4,839 button pictures), 0x13 (678), 0x11 (634 sizes, e.g. 80 %), 0x1F (70 marks), 0x19 (42),
0x1A (19), 0x18 (17) and 0x1D (5); the tables in `mlng` language containers (60,372 strings) use only 0x0A, 0x10 /
0x1B and 0x1E. Neither uses 0x12, 0x14, 0x16 or 0x17, so the 0x12/0x16 cut-off (and its box) can only come from
strings built by code. (A table at a bare file start, with no container, has no magic and was not scanned.)

| Code | Args | Pass 1 (runs) / pass 2 (glyphs) |
| --- | --- | --- |
| 0x0000 | | end; at the end of a variable string, back to the saved place |
| 0x000A | | new line: run with flag 0x01, row + 26 |
| 0x000D, 0x0017 | | ignored |
| 0x0010 | u32 | colour from here (new run): argument bytes b0, b1, b2 as R, G, B, alpha 0xFF, the low bit of R, G and B cleared (`(b0 \| b1 << 8 \| b2 << 16 \| 0xFF000000) & 0xFFFEFEFE`; the 4th byte is ignored) |
| 0x0011 | u16 | size n / 100 from here (new run; the quad is scaled, the glyphs are not) |
| 0x0012 | | the run before it gets flag 0x02 (drawing stops after it); next line in the buffer (pass 2 still draws the rest into the buffer); from here runs are not closed: the run before 0x12 and all later ones keep 26.6 widths and add nothing to +0x10 (**corrected**) |
| 0x0013 | | variable string `vars[i++]` (UTF-16) inserted (one return place only: no nesting); no new run and no pen rounding: its characters join the current run. Nothing happens (and i stays) when `i >= nvars` or `vars` is null. **corrected (review 2)**: a null entry is skipped by pass 1 only; pass 2 has no null check and reads a string at address 0 (invalid on the PSP) |
| 0x0014 | u32 | **corrected**: no case: drawn as a character, its argument units too |
| 0x0015 | u16 a | ONE image from set 1: id `((a >> 4 & 15) - 1) * per_row + (a & 15)` (per_row 14), `((a >> 8) & 15) - 1` cells wide and (**corrected (review 2)**, left out before) `((a >> 12) & 15) - 1` cells tall; its placeholder is 18 x wide px and it counts as `wide` letters |
| 0x0016 | | like 0x12 but flag 0x04 on the run before it (0x02 cleared): 0x0887BC2C goes on, but the next run has no new-line flag, so it is drawn on the SAME quad row (u continuing, v of the first line) with a 26.6 width, and cut at the width limit (section 4). Not used in the disc's message files (see above) |
| 0x0018 | | item `vars[i++]`: icon (id from 0x089D6904 by item kind: 22..25; kind = the s8 at +0x0C of the record 0x088B97E8 gives), the item's name (0x088B98FC), and an icon after it when 0x088C7F78(item) is non-zero: id 21, or 20 (manager +0x03) when `0x088C7FE4(item)->+0x04 & 2` (review 2: "0x15" was this id 21, not the code) |
| 0x0019 | | `vars[i++]` -> 0x088B1D9C -> entry of the text table 0x09B49108 (entry number: the s16 at +0x02 of the record). No icon and no new run. **added (review 2)**: pass 2 rounds the pen up to a whole pixel here and pass 1 does not, so the glyphs from here to the end of the run sit up to 1 px right of where pass 1 measured them (the run's last pixel column can cut them) |
| 0x001A | | `vars[i++]` -> 0x088B29D8 -> entry of 0x09B49118 (entry: s16 at record +0x02), icons before (0x089D6914[(s8) record +0x12]: all 12) and after (0x089D6924[(s8) record +0x14]: 13..16; always) |
| 0x001B | | colour back to the object's |
| 0x001C | | only as the first unit: bitmap-font byte string |
| 0x001D | | `vars[i++]` -> 0x088C6CA4 -> entry (u16 at record +0x04) of the text file 0x088C6D30() returns, icon id 0x1F (31) before, none after |
| 0x001E | u16 a | one image from set `(a & 0xFF) - 1`, id `a >> 8` (button glyphs; 0x80/0x81 = confirm/cancel) |
| 0x001F | u16 n | n = 5: inserts U+2122 (string 0x089D6960 = {0x2122, 0}); n - 1 in 100..199: the next drawn glyph's bitmap position goes to the next mark at +0x28 (no bound check) |

Every image or icon becomes a *placeholder run* (flags 0x18, size 1.0, never drawn) in the buffer row and an image
node drawn separately; the buffer has blank pixels there (**read**; **traced**: the shop's help bar, frame 4600 of
ge_index, has no fan over its icons' u ranges). **corrected (review 2)**, from the assembly (pass 1 0x08880488-
0x0888073C, 0x0887FDC4-0x0888047C, 0x0887F450-0x0887F64C; pass 2 0x08880B98-0x08880C6C, 0x08880E18-0x08881570):

- Width: 18 px per cell (the set's cell byte), so a 0x15 image is 18 x `wide` px and an icon 18 px. Consecutive
  0x15/0x1E codes share ONE placeholder run as wide as all of them together (**traced**: two adjacent help-bar icons
  leave one 36-px gap, u 218-254). A 0x15 image counts as `wide` letters, every other image or icon as 1.
- Pen: before a placeholder the open run is closed and the pen rounded up to a whole pixel; the pen then moves by
  exactly the placeholder's width, so the next run starts on a whole pixel. Pass 2 places each image node at its
  pen (whole px) and moves the same way, so node x = the placeholder's u in the buffer (plus the widths of earlier
  images of the same group).
- Icons of 0x18 / 0x1A / 0x1D: a leading 18-px placeholder before the inserted name, and for 0x18 (when 0x088C7F78
  says so) and 0x1A a trailing one, made when the inserted name ends (both passes keep the trailing id until the
  variable string's terminator). Image nodes for icons always use set 0.
- Size after an image: the run after a 0x15/0x1E group and the run after a trailing icon get size 1.0 even inside a
  0x11 size (quirk, as before); the run after a LEADING icon (the inserted name) keeps the current 0x11 size. Only
  that one run: the next run break goes back to the 0x11 size.
- A string that starts with an image has an empty first run (width 0), and one that ends with an image group an
  empty last run (**read**); both are drawn as zero-width fans (**traced** for a leading image: help bar, fan
  (167,10)-(167,23) uv (0,0)-(0,25)).
- Image node line: pass 2 counts 0x0A, 0x12 and 0x16, while 0x0887BC2C's per-line offsets count only new-line runs
  (0x0A), so an image after 0x12/0x16 would take another line's offsets (no message file uses them).

### Pens and font calls

Pass 1 measures (**read**; **traced** ra 0x088807AC for all 7263 GetCharInfo): `sceFontGetCharInfo(fonts[0], code,
&info)` (an error skips the character); a code the font lacks (advance 0) is measured again as U+3013. Advance =
`info.sfp26AdvanceH` (26.6), except U+0400-U+04FF (left bearing 0, advance `min(advanceH, (bitmap width + 1) x 64)`)
and U+201C (advance `advanceH - (left - 2) x 64`, left bearing forced to 2). The first 256 characters' {left, top,
advance} are cached at mgr+0x2C ({-1,-1,-1} for a missing glyph). The pen is 26.6; a run closes at whole pixels
(`(w + 63) >> 6`) and EVERY new run rounds the pen up to a whole pixel (`((pen + 63) >> 6) << 6`), so run
boundaries sit on whole pixels in the buffer. When the pen first passes 512 px on a line, the run is cut before
the character that crossed (that character starts the new run) and the pen is rounded up after it.

Pass 2 draws (**read**; **traced** ra 0x08881840 for all 7263 GetCharGlyphImage, fmt 0): for each character,
`sceFontGetCharGlyphImage(fonts[0], code, &image)` with `image = {fmt 0 (4-bit), x64 = pen + left x 64, y64 =
line x 26 x 64 + 0x480 - top x 64, buffer w, h, bytes per line, 0, the whole buffer}` (line = count of
0x0A/0x12/0x16 so far). So the font library draws every glyph straight into the string's buffer at a sub-pixel pen
(**traced**: x64 = 2505). Pass 2 takes the metrics from the cache (first 256 characters) or asks again; codes
0x10/0x11/0x1B round the pen up as in pass 1, but pass 2 does NOT round at the 512-px cut, so glyphs after it can
sit up to 1 px left of their run's u range. A glyph whose `x64 + advance` passes 0x10000 (1024 px) ends the whole
string: nothing after it is drawn, later lines included. After the 256th character a glyph the font lacks is
drawn as itself, not as U+3013. A path that would draw U+FF3F before a character is dead (its flag is never set).
There is no second, bolder pass and no shadow (**traced**: one GetCharGlyphImage per character).

### Lines, widths, alignment

Lines come only from codes 0x0A/0x12/0x16 in the string: there is no word wrap (**read**). Each line's height is
26 rows in the buffer. Alignment is not done in the buffer (every line starts at x = 0) but when drawing: per line
for x (flags 0x4, 0x8) and for the whole block for y (0x10, 0x20, 0x40) (**read**).

### Whole-string buffers versus cells

- sceFont path (0x0887F294): one buffer per text. Every text made by 0x0887D868 without 0x001C: menus, the shop,
  battle labels and windows, story and mail text alike (**traced** for menus, the shop, the accessory and ability
  lists, the story route and the battle route: every glyph fmt 0 into a whole-string buffer; mail **read**).
  The port's `host/fonts/game_font.hpp` describes 256x256 atlas pages of glyph cells: **this game has no such
  path** (only 0x0887F294 calls sceFontGetCharGlyphImage, at 0x0888161C and 0x08881838; the trace saw only whole
  buffers).
- Bitmap-font path (0x0887E9CC, drawn by 0x0887EF84; **read**, whole function checked in the assembly by the third
  pass): the bytes after the 0x001C unit, one line, at most 40 glyphs, 28 such texts at once (records of the bitmap
  font). Glyphs are taken from one image whose head and width table come from a package resource starting with 'F'
  (0x088D2980 -> 0x0887D5C8; image set by 0x0887D66C from 0x0888C4C8). **corrected** format:
  - a glyph is one byte 0x22-0x7F (index `c - 0x22`) or two bytes, a lead byte 0x80-0xFF and a non-zero byte b
    (index `b x 128 + c - 0xA2`); index < count_a: cell `index x cell_w_a, 0` of height cell_h_a, else cell k =
    index - count_a in a grid of cols_b columns of cell_w_b x cell_h_b below it; the glyph is `widths[index]` wide
    (<= 15) and the pen moves by that much;
  - 0x20 = 4-px space, 0x21 = 8-px space (one byte each);
  - a control code takes two bytes (the code and one ignored byte, as a UTF-16 unit) plus its argument bytes (the
    table at 0x089C9D2C: 0x10: 4, 0x11: 2, 0x14: 4, 0x15: 2, 0x1E: 2, 0x1F: 2): 0x10 colour (the same rule as the
    sceFont path), 0x1B colour back to the object's, 0x13 variable byte string `vars[i++]` (no null check: a null
    entry reads address 0), 0x15 and 0x1E one 18-px image (set = first argument byte - 1, id = second; 0x15 is NOT
    the multi-cell code of the sceFont path), the pen moving 18 px; 0x11, 0x12, 0x16-0x1A, 0x1D, 0x1F are skipped;
  - 0x00 ends the string; 0x0A ends it too (or a variable); 0x14, 0x1C and stray bytes 0x01-0x0F become a garbage
    glyph (the index wraps), 0x14 and 0x1C then skipping their bytes like a code.

  Where they are: no code writes 0x001C at a string start (scan of all modules), so these strings come from data.
  A read-only scan of the disc's PACKAGE.BIN (every message table in the 807 `mlng` language containers and the 25
  "mess pak" message packs, in the format 0x0887A6A0 / 0x0887A800 read; scratchpad scripts, not kept) finds 0x001C
  strings in exactly one kind of file: the `text` chunk of the fighters' moveset archives (`ARC\x01` files,
  obj/p_xxxNNN.objx), 110 of them, whose tables hold nothing else; none of the other ~83,000 strings (menus,
  story, help) starts with 0x001C (**read**, data). These are the battle banners (HP attack names; the EX Mode
  prompt): the repo's `tools/character_text.py` documents them (glyph numbers as little-endian base-128 numbers,
  i.e. index + 0x22, which is 0x0887E9CC's decoding), and main 0x08856D70 makes a banner text from such an entry
  (0x0887AD78, up to 6 banner slots) that 0x08857134 draws after the banner window. **traced** (pc_bc2c): none of
  the 135,505 text draws from boot to the shop whose object was dumped had flag 0x2000, as expected: no fight on
  that route. **traced** on screen (battle route `gfx2`, sprite trace of frame 8800): Lightning's tip "Press [R]
  while holding [L] to Change Paradigms!", shown at the top left through the fight, is 35 TRIANGLE_STRIP quads
  (one per glyph, 14 px high, 3-11 px wide, pens continuous, 4-px spaces) from one 512x256 CLUT4 image
  (0x08C8DF30) plus two 18x18 button pictures from another image, 2 px above the glyphs: exactly what 0x0887EF84
  and 0x0887CA50 draw for a bitmap text.

## 4. Drawing

### Passes

- **Draw pass** 0x0887DE04 ("SYSTEM_FONT::Draw" task; also from 0x088EC010) (**read**): `sceGuDisable(CULL_FACE)`,
  `sceGuEnable(FRAGMENT_2X)`, `sceGuEnable(TEXTURE_2D)`, `sceGuBlendFunc(ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, 0,
  0)`; for each active text in creation order: skip if 0x200 or 0x8000, set 0x200, draw with z = 0xFFF0 if 0x400 or
  0x1000; then `sceGuDisable(FRAGMENT_2X)`, `sceGuEnable(CULL_FACE)`.
- **Layout nodes**: a layout (seq) file's own labels are made by 0x088D8FD0 (through 0x0887A3C8: from the label
  cache or a new text, centred, middle-aligned) and linked into the node's list (+0x30, through text+0x4C/+0x54);
  the layout draw 0x088D16F4 calls 0x088D9E48(node, z) for each node with texts, which draws them like the Draw
  pass with the node's z (-1 becomes 0), during the layout's own drawing (**added (review)**: unlike the Draw pass it
  sets no GU state at all, no FRAGMENT_2X, CULL_FACE or blend function, and it does not test 0x8000; node texts get
  whatever state the layout's drawing left, traced the same as the Draw pass's at frame 4600); the Draw pass skips them (0x8000, and
  0x200) (**read**). Screen code links its own texts into nodes with **0x088D40BC**(nodes, labels, label, align,
  node, part): show, set 0x8000, append to the node's list, alignment from `align` (0x1 left, 0x2 centre, 0x4 right;
  0x8 top, 0x10 middle, 0x20 bottom; the first bit found of each group wins), line pitch (**corrected (review 2)**:
  from the NODE, not the part: field +0x08 of entry `(s16) node+0x0C` of the 28-byte table at `((node+0x24)+0x0C)
  +0x0C`), and the part's matrix and colour copied in (0x088D4584 -> 0x088DA0A8, then 0x0887B778 / 0x0887B828).
  Most screens call it through **0x088D428C**(nodes, labels, bindings, clear), which walks a binding table of `{u16
  label, u16 align, u16 node, u16 part}` entries ended by node 0xFFFF (clear != 0 empties each bound node's list
  first, without touching the old labels' links; entries whose label is null are skipped; 312 call sites in menu). Each frame the same tables go to
  0x088D44EC, which only re-copies the matrix and colour; 0x088D442C releases the bound labels and empties the lists
  (all **read**, **corrected** by the third pass: the first two passes named 0x088CCF74 / 0x088CD754, which set a
  node's text from a file string, and did not find this linker). 0x088D8FD0, 0x088CCF74 and 0x088CD754 also set
  0x8000 for their own texts.
  **traced** (pc_bc2c, shop frames split at the Draw pass): 67-71 texts drawn by 0x088D9E48 (z 0x13 to 0x4F), then
  14 by the Draw pass (z 0xFFF0); those 14 sit at their default position (0, -1) with scale 1.0 and colour 0 /
  0x00FFFFFF / 0xFFFFFFFF, so they are drawn ABOVE the screen (quads at y -27..-1): invisible work (and, in the
  port, Sharp text textures nobody sees). **corrected**: the first map said the shop's numbers were drawn by the
  Draw pass.
- Also 0x08857134 (the battle banner window: frame pieces, then its text at slot+0x78) and battle 0x089E1650 call
  0x0887BC2C (**read**).
- **Update pass** 0x0887DBD4 clears 0x200, steps reveals, frees unused buffers (**read**).

### 0x0887BC2C, whole-string text

GU calls in order (**read**, call order checked in the assembly):

1. `sceGuGetMemory((runs + 1) x 64)`: 4 vertices of 16 bytes per run.
2. Build one quad per drawn run (below).
3. `sceGuTexWrap(CLAMP, CLAMP)`, `sceGuClutMode(GU_PSM_4444, 0, 0xFF, 0)`, `sceGuClutLoad(1, 0x089D68D0)`,
   `sceGuTexMode(GU_PSM_T4, 0, 0, 0)`.
4. Only if the text passed its width limit (after code 0x12/0x16): `sceGuPixelMask(0xFFFFFF)`,
   `sceGuDisable(TEXTURE_2D)`, `sceGuColor(0xFF0000FF)`, `sceGuDrawArray(TRIANGLE_STRIP, 0x800100, 4, 0, box)` at
   z + 1, `sceGuPixelMask(0)`, `sceGuEnable(TEXTURE_2D)`: a box from that x to the run's end that writes only the
   framebuffer's alpha and depth (invisible; see below).
5. Only if at least one quad: `sceGuTexImage(0, 512, tex_height, width, pixels)`, `sceGuDrawArrayN(TRIANGLE_FAN,
   0x80011E, 4, quads, 0, verts)`. Buffers wider than 512: runs ending past u = 512 are drawn with u - 256 from
   `pixels + 0x100 / pixels_per_byte (= 128) bytes`, consecutive quads with the same offset in one TexImage +
   DrawArrayN each (a leading offset quad first emits a DrawArrayN of 0 quads; a run reaching past 768 px still
   samples past u = 512: clamped).
6. If the buffer has images and not 0x800: 0x0887CA50 (below).

- **CLUT**: 16 entries of 4444 at 0x089D68D0: `0x0FFF, 0x1FFF, ..., 0xFFFF`: white, alpha = index x 17 (**read**;
  **traced** CLUT address 0x089D68D0, format 0x00FF02, 1 block).
- **Vertex** (0x80011E = through mode, 16-bit texcoords and positions, 8888 colour): `{u16 u, u16 v, u32 ABGR,
  s16 x, y, z, pad}` (**read**; **traced** vtype 0x80011E, prim 5, n = 4).
- **Quad of a run** (**read**, every operation checked in the assembly): corners top-left, top-right,
  bottom-right, bottom-left. With `lh` = manager line height (26), `sx`, `sy` = scale:
  - `u0` = the run's x in its buffer row = sum of the widths of the runs before it on the line (hidden and
    unrevealed runs included); `u1 = u0 + width`. `v0` = 26 x line (counted from new-line runs, not run+0x0C),
    `v1 = v0 + lh - 1` (the 26th row is left out) (**traced** uv (0,0)-(w,25)).
  - `x0 = text.x + pen`, `x1 = x0 + trunc(width x size x sx)`, where `pen = trunc(T x sx + I)`, T = sum of
    `trunc(width x size)` of the line's text runs before this one, I = the same for image placeholders (images
    are NOT scaled by sx).
  - `y0 = trunc(lh x sy x (1 - size) + text.y)`, `y1 = trunc(y0 + lh x sy x size)` (bottom-aligned in the line).
  - Per line, when it ends (at the next new-line run or the end): `pitch = text+0x08` if not 0, else
    `trunc(lh x sy x maxsize)`, maxsize = largest size of the line's runs AND of the next line's first run (the
    maximum is updated before the line test: quirk, no effect with one size). Line n is moved down by the sum of
    the pitches of lines 0..n-1 and left by `pen/2` (flag 0x8, tested first; C division) or `pen` (0x4), `pen` =
    the full line width (unrevealed runs included, so a reveal does not shift the line).
  - Block: 0x40 (tested first): up by total/2; else 0x20: up by total; else none; total = sum of all pitches.
  - z = the caller's z on all four corners.
- **Scale**: what the owner sets; on the shop screen 0.44 (stat numbers: uv 43 wide -> 19 px, 26 -> 11 px), 0.55
  (description) and 0.6 (**traced**, frame 4600).
- **Colour**: the run's colour with its alpha byte replaced by the object's (`run & 0x00FFFFFF | obj & 0xFF000000`),
  or the object's colour as it is when the run's top byte is 0 or flags have 0x4000/0x800; then every alpha byte
  replaced by (u8) +0x0A when it is not -1 (**read**).
- **Reveal**: a run is drawn only if it has no 0x10 flag and `text+0x04 >= run first letter` (signed).
- **Width limit and the box** (**corrected**, **read**): `limit = trunc(buffer+0x10 x sx)`; `drawn` = sum of the
  quad widths of all runs so far (all lines, not reset per line). A run is handled only while `limit - drawn > 0`;
  after that the rest of the line is skipped and drawing stops at the next new-line or end run. With +0x10 = 10 +
  the widths of every closed run, ordinary texts never reach it; only texts with code 0x12/0x16 do (their later
  runs keep 26.6 widths and add nothing to the limit). The run that crosses the limit is drawn whole, and once per
  text a box (vtype 0x800100, 4-vertex STRIP, z + 1) from `limit - drawn + pen` to the run's right edge is drawn
  BEFORE the quads with `sceGuPixelMask(0xFFFFFF)` (colour bits kept), TEXTURE_2D off and `sceGuColor(0xFF0000FF)`:
  it writes only the framebuffer's alpha and depth z + 1 (depth writes are on, below), so under the inherited
  GEQUAL test it hides the crossing run past the limit (a cut-off). Game bugs (**corrected (review)**, assembly
  0x0887C6B4-0x0887C864): the box is built from the crossing quad's corners before any line or block offset, then
  moved once at the end: its right x and all its y get text.x / text.y added a second time; its x is aligned with the
  LAST line's width (flag 0x8 / 0x4), not its own line's; its y gets `s2 + text.y` and then, for flag 0x20 (the
  default) or 0x40, the block shift subtracted, where s2 is that same block shift, so the box gets NO block shift and
  no line offset (the first passes said the shift was added instead of subtracted); with neither flag (top) s2 is the
  last line's offset. So it lands off the text (for the default bottom alignment one block height below the quads
  even when text.x/y are 0). No message file on the disc uses 0x12/0x16 (section 3), so the box can only come from
  strings built by code.
- **Depth writes** (**traced**, runs ge_zmsk0-4: every display list from boot to the shop scroll dumped): the only
  ZMSK command is libGu's init list (0x089D1510: 0xE7000000, writes enabled); no later list changes it, and every
  frame's list sets ZTST GEQUAL (0xDE000007). So texts are drawn with depth test GEQUAL and depth WRITES ON: every
  glyph pixel that passes the alpha test writes its z (the node's z, or 0xFFF0 from the Draw pass); fully
  transparent texels fail the alpha test and write nothing. Later sprites with a smaller z fail the depth test on
  those pixels. (**read**: the code that does call sceGuDepthMask, mostly in battle and full-screen pictures, turns
  writes off for its own draw and back on after it.)
- **State not set by the text code** (inherited from the frame; **traced** at all 95 text quads of frame 4600):
  TEXFUNC 0x010100 (MODULATE, RGBA, colour doubling = FRAGMENT_2X), BLEND on (ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA),
  ALPHA TEST on (GREATER 0; two quads GREATER 4), DEPTH TEST on (GEQUAL) with depth writes on, texture filter
  0x0107 (min LINEAR_MIPMAP_LINEAR, mag LINEAR), scissor (0,0)-(479,271), dither on, stencil off. So on the PSP a
  glyph's colour is `2 x vertex colour x white` and its alpha `coverage x vertex alpha`, bilinear-filtered as it is
  shrunk.
- **Sprites per text**: one quad per drawn run: a plain label is one quad (**traced**: one fan per label); an icon
  in a label splits it into the runs before and after it plus the image drawn by 0x0887CA50.
- **Shadows/outlines**: none in the text code. **corrected** (**traced**, pc_bc2c and ge_index): screens do draw
  shadows, as a second text object sharing the same buffer, drawn just before the text at (+1, +1) in a dark
  colour: e.g. buffer record 0x08BF877C drawn with colour 0x00402826 at (90,108) then 0x00FFFFFF at (89,107);
  0x08BF8FBC with 0xFE201413 at (241,262) then 0xFEFFFFFF at (240,261). Both are node texts. A host drawer that
  handles each 0x0887BC2C call gets the shadow for free.
- **Scrolling/ticker text**: nothing in the text code scrolls; owners move a text by its x/y each frame; the
  letter-by-letter reveal is +0x04/+0x06 (**read**). A ticker's clipping, if any, is the owner's scissor (not seen).
- **Draw order** (**traced**, frames 4500 and 4600): text quads are interleaved with the layout's other sprites (node
  texts, drawn by 0x088D9E48 inside the layout's drawing), followed by a block at the end (the Draw pass, here the
  off-screen texts); depth test GEQUAL with z = 0xFFF0 or the node's z.
- **Other readers of a buffer**: none found; the pixels are written by the font library and read by the GE only
  (**read**: 0x0887BC2C is the only code passing `pixels` on, 0x0887DBD4 frees it). Not proven for every caller (the
  record pointer is reachable from any text).

### 0x0887CA50, inline images (button glyphs, item icons)

(**read**, whole function checked in the assembly by the third pass) `func_0887CA50(scale, text, z, xoffs[32],
yoffs[32], limit)`; `limit` = the number of images revealed (0xAA when every letter is shown); the per-line offsets
come from 0x0887BC2C's line alignment.

1. Saves the enable bits (sceGuGetAllStatus 0x089A04E8), enables BLEND, FRAGMENT_2X, ALPHA_TEST
   (`sceGuAlphaFunc(GREATER, 0, 0xFF)`) and DEPTH_TEST, disables CULL_FACE, sets `sceGuTexFunc(MODULATE, RGBA)` and
   TexWrap(CLAMP, CLAMP). It sets no depth mask, filter or texture mode of its own.
2. Per image node, while fewer than `limit` nodes have been visited: binds the set's image (0x0899D2F8, which writes
   the image's own texture commands), builds one quad in TRIANGLE_STRIP order (top-left, top-right, bottom-left,
   bottom-right; vtype 0x80011E), sets BlendFunc(ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA) and calls
   `sceGuDrawArrayN(TRIANGLE_STRIP, 0x80011E, 4, 1, 0, v)`: one draw call per image.
   - UVs: the whole cell block, `(u, v)` to `(u + 18 x cols, v + 18 x rows)` (no left-out row).
   - Size on screen: 18 x cols by 18 x rows pixels, **not** scaled with the text (an 18-px icon stays 18 px in a
     0.44-scale label); only with flag 0x1 is it scaled by scale / base scale, about its centre.
   - Position: x = text.x + xoffs[line] + pen, y = text.y + yoffs[line]. pen = `trunc(image x x scale + the unscaled
     widths of the earlier images on the line, less their scaled widths)`, so images keep their full size while the
     text around them shrinks, as in 0x0887BC2C; the next image of the same group continues at the previous one's
     right edge.
   - **corrected (review 2)**, what the offsets are (0x0887BC2C 0x0887C0FC-0x0887C138 and 0x0887C550-0x0887C58C,
     block shift 0x0887C648): `xoffs[line]` = minus the line's alignment shift (the same as its text quads);
     `yoffs[line]` = the line's offset (sum of the earlier pitches, less the block shift) **minus
     `(18 - (trunc(lh x sy x maxsize) - 1)) / 2`** (C division; lh x sy x maxsize = the line's natural height, even
     when +0x08 sets the pitch). So an image is centred vertically on its line's text height, not top- or
     bottom-aligned: at scale 1.0 its top is 3 px below the line top (an 18-px icon inside the 26-px line), at 0.5 it
     is 3 px above the 13-px text (**traced**, ge_index frame 4600: help bar text quads y 10-23, its icons y 7-25;
     off-screen Draw-pass texts at scale 1.0: text y -27..-1, icon y -24..-6). Offsets are written only for lines
     0-31. The text quads above use no such centring.
   - The image pen ignores 0x11 sizes: `image x` is the buffer position, so after text at a size below 1.0 on the
     same line the icon sits right of where 0x0887BC2C drew that text's end, and the text after the icon can start
     left of the icon's right edge (**read**, quirk).
   - Colour: RGB 0x80 (white after FRAGMENT_2X) and the text colour's alpha byte. **corrected**: the alpha override
     (text+0x0A) is NOT applied to images, so a text faded by +0x0A keeps its icons at the colour's alpha. Run
     colours (code 0x10) and flag 0x4000 do not tint images either. A shadow clone shares the buffer's image list, so
     it draws the icons as well, untinted, at its own (+1, +1) and alpha (**read**).
   - Height 0 (an id below 1, e.g. 0x1E with a zero id byte: 0x0887E114 sets rows to 0): nothing drawn, the pen
     still moves (**read**).
3. Restores the enable bits (sceGuSetAllStatus 0x0899FDC4). AlphaFunc, TexFunc, BlendFunc, TexWrap and the bound
   texture stay changed for whatever draws next.

**added (review 2)**, the GE state at an image (0x0899D2F8 -> 0x0899C764 with flags 0xFFFF; see
`docs/port/texture-pipeline.md`): the bind writes the image's TMODE, TPF, TBP/TBW/TSIZE, CLUT and, per the image
object's `draw_flags`, TEXLEVEL (2), UV scale/offset (4), blend (0x100), TFUNC (0x200: the object's own `+0x78 <<
8 | +0x74`, which replaces the FRAGMENT_2X bit too), TFLT (0x400) and TWRAP (0x800). TexFunc and TexWrap are set once BEFORE the loop, BlendFunc per image AFTER the bind,
so an image object with draw_flags 0x200, 0x400 or 0x800 would override the texture function, the filter or the
wrap. 0x0887CA50 never enables TEXTURE_2D (it relies on the caller: the Draw pass, the box path's re-enable, or the
layout's state for node texts). **traced** (ge_list frame 4600, the eight strips on set 0's image 0x08C2E5B0, 128 x
128 CLUT8): TFUNC 0x010100, TFLT 0x0107, TWRAP clamp/clamp, ATST GREATER 0 mask 0xFF, blend ADD SRC_ALPHA /
ONE_MINUS_SRC_ALPHA, depth test on: the same state as the text fans, so set 0's image changes nothing there.

**Order against the text quads** (**read** 0x0887C9B8-0x0887CA10, **traced** ge_index frame 4600): 0x0887BC2C draws
all of a text's run quads first (one DrawArrayN per page group), then calls 0x0887CA50 (when the buffer has images and
the text has no 0x800), which draws that text's images one strip each; the next text follows. The help bar shows it:
five fans, then its six icon strips. So a host that draws a text's glyphs exactly where the game's DrawArrayN was
(approach (b), or (a) with a marker at that place) keeps the game's order: glyphs, then the same text's icons, then the
next text. The images overlap nothing of their own text's glyphs in the buffer, but on screen an 18-px icon reaches
3 px above and 2 px below a 13-px (scale 0.5) line, and it is drawn after the glyphs at the same z (depth GEQUAL
passes at equal z), so it covers any glyph pixels it overlaps.

The images come from the image sets, never from the text buffer.
Set 0 is texture 0x1804 of texture-manager group 7 (0x088D09D8), 18-px cells in a 128-px-wide image, installed by
main 0x088EBAA8; set 1 is a per-screen image (0x088D0700), 18-px cells, 256 px wide, installed by 0x088FD928
(**read**; also set from menu 0x08AD788C and battle 0x08A3878C, 0x08A431C0, not read).

### 0x0887EF84, bitmap-font text

(**read**, whole function checked in the assembly by the third pass) `func_0887EF84(font, z, record, text)`:

- Saves the enable bits and clears them all (sceGuSetAllStatus(0): the scissor test is off too), then enables
  TEXTURE_2D, BLEND, FRAGMENT_2X, ALPHA_TEST (GREATER 0) and DEPTH_TEST, disables CULL_FACE, sets TexFunc(MODULATE,
  RGBA) and TexWrap(CLAMP, CLAMP); no depth mask call. Nothing is drawn for a record with no glyphs.
- Origin: x0 = text.x, less the record's width (flag 0x4) or half of it (0x8); y0 = text.y, less the font's
  cell_h_b (0x20, the default) or half of it (0x40); 0x10 = no shift. (The sceFont path tests 0x8 and 0x40 first;
  this one tests 0x4 and 0x20 first.)
- Binds the font image (+0x310), then one TRIANGLE_STRIP quad per glyph (top-left, top-right, bottom-left,
  bottom-right): UVs `(u, v)` to `(u + w, v + h)` from the glyph's cell (w, h <= 15), screen `(x0 + glyph x, y0)`
  with the same w x h size. The text's scale and the letter-by-letter reveal (+0x04) are ignored.
- Colour: the text colour when the glyph's colour is 0 or flag 0x4000 is set, else the glyph's colour with its
  alpha byte replaced by the text colour's; the alpha override +0x0A is not applied.
- BlendFunc(ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA), one `sceGuDrawArrayN(TRIANGLE_STRIP, 0x80011E, 4, count, 0,
  verts)` for all glyphs, restores the enable bits. Inline images are then drawn by 0x0887CA50 at scale 1.0
  (called from 0x0887BC2C).

## 5. Why scrolling the shop list makes text

Shop route of `run_shop.py` (Customization > Lightning > Shop > Buy, d-pad held):

- **traced** (pc_d868nc, TRACE_PC 0x0887D868 with chaining off): every step of the held scroll makes about 25 texts.
  hold_down (23 steps): 138 x each of menu 0x08AFE35C, 0x08AFE400, 0x08AFE4AC (in 0x08AFE03C), 38 x main 0x0887AE88
  (0x0887AD78), 23 x 0x0887B4D8 (0x0887B3EC), 23 x each of 0x08AF5580, 0x08AFF148, 0x08B05184, 15 x 0x08B10FD4.
  Strings of one step (font1): six rows of "value, value with the item, difference" ("2375", "2375", "0", "339", ...),
  then " ", "1", "Sword", "ATK+9 ".
- **traced** (pc_ba5c, TRACE_PC 0x0887BA5C): each step releases 24 texts through main 0x088D4498 (0x088D442C), 4
  through 0x08AFEBD4, 1 through 0x08B05024, 2 through 0x08B108BC (15 steps of 23) and ~1 through 0x088D3664.
- **read**: menu 0x08AFE03C is the **stat comparison panel**: for each stat row it formats the current value, the
  value with the highlighted item and the difference (sprintf 0x0893476C with the format at 0x08B0128C, 0x0887DF14 to
  UTF-16), makes three texts (0x0887D868, colour 0xFFFFFFFF), stores them in the panel's label array and clones an
  arrow label (0x0887E534) for the up/down/equal state. 0x08AFE69C runs it when its countdown (owner+0x140 ->
  +0x2C5C) runs out, after releasing the old labels with 0x088D442C.
- **read** (third pass), who links the new numbers into the layout: right after 0x08AFE03C, 0x08AFE69C calls menu
  **0x08AF4F6C**(owner, 0), which calls 0x088D428C(nodes, labels, 0x08B006BE, clear 1) and 0x088D428C(nodes,
  labels, 0x08B00726, 0) (another layout mode uses 0x08B007EE / 0x08B0085E); each binding goes to 0x088D40BC (flag
  0x8000, node list, alignment). Table 0x08B00726 (24 bindings) binds the 18 numbers (labels 4, 6, ..., 38) with
  align 0xC (right, top: flags 0x8616, as traced) to parts 59-76 of node 12 and 6 more labels (7, 13, ..., 37: one per
  row, probably the cloned arrows) with align 0x9 to parts 53-58; table 0x08B006BE binds 12 labels (5, 9, 11, 15, 17,
  21, 23, 27, 29, 33, 35, 39) with align 0x9 (left, top: flags 0x8612, as traced) to parts 41-52 of node 12 (the
  tables re-read from the menu overlay by review 2: as stated; the other mode's 0x08B007EE has 13 entries, labels 3,
  5, 9, ... to parts 0 and 14-25 of node 12, and 0x08B0085E 24: the same 18 numbers and 6 labels, partly to other parts of
  node 12). On every call 0x08AFE69C passes the same
  tables to 0x088D44EC, which only re-copies each part's matrix and colour into its label.

So the list rows are not made again: a step moves the cursor to another item, and the game throws away and rebuilds
the **stat comparison panel** (18 numbers + 6 cloned arrows) and the **item detail panel** (name, count, effect line,
about 5 texts) for that item. Each new text is a new buffer (26-row, 32-288 px wide), which the port's Sharp text
then redraws at the internal resolution.

**The other lists** (**traced**, `build/scratch/text-system/run_list.py`, TRACE_PC 0x0887D868 + NO_CHAIN +
TRACE_FONT; Customization menu order: Abilities, Equipment, Accessories, Summons, Assist, Adjust Level, Shop):

| Screen (run) | Steps in the hold | Texts made per step | Frames > 25 ms |
| --- | --- | --- | --- |
| Accessories > Equip > a slot > the accessory list (acc2_d868) | 47 | 18 from the stat comparison panel (menu 0x08AFE35C/400/4AC, the same 0x08AFE03C as the shop), ~2 description lines (0x0887AD78, ra 0x0887AE88; **corrected (review)**: not 0x0887AEAC), 1 short label once the list scrolls (menu 0x08ADD4BC) | 42 of 310 (p99 55 ms, 2252 textures) |
| Accessories > Equip > the 3 slots (acc_d868) | 1 | 18 (stat panel) + 1 | - |
| Abilities > Offensive > Bravery Attacks, the 6 slots (abil_d868) | 5 | ~8: 4 x menu 0x08AD251C, 0x08AD1800, 0x08AD179C, 0x0887B3EC (description) ... | 0 |
| ... > a slot > the learned-ability list (abil2_d868) | 2 | ~5: menu 0x08AD1D3C/1B2C/1AC8/1800/179C (AP bar, description) | 0 |

So the accessory list stutters exactly like the shop, for the same reason: every step re-makes the 18-number stat
comparison panel for the highlighted accessory. The ability screens re-make only their description panel (5-8
texts) and did not stutter here; the save's Lightning knows only three abilities per slot, so a long ability list
(more steps per second of holding) could not be tested. In no list are the rows themselves re-made.

### Battle and story (**traced**, gfx_tour routes with D012_TRACE_FONT=1, runs in `build/scratch/text-system/gfx/`)

| Route, scene | Frames | sceFont texts made | Shapes |
| --- | --- | --- | --- |
| Story: world map walk | 1910 | 5 | 288x26, 96x26, 128x26 |
| Story: gate transition | 140 | 10 | 288x26, 192x26, 32x26 |
| Story: gateway board (Lightning's lines) | 970 | 2 | 384x26, 256x52 (two lines) |
| Battle: menus to the fight (Quick Battle setup, loading) | 6300 | 203 | 384x78 help, 96x26, 64x26 |
| Battle: intro | 268 | 7 | 480x52 quote, 416x26, 288x26 |
| Battle: the fight (attacks, dash, assist) | 600 | **0** | - |

Story and battle text goes through the same path (0x0887F294, whole-string 4-bit buffers), but texts are made a
few per scene, not per frame: no list-like churn. During the fight no sceFont text is made, and a traced fight
frame (8800, `gfx2`) draws no text fan: the battle HUD (numbers, names, gauges) comes from 128x128 CLUT4 sprite
sheets in VRAM, not from the sceFont path. The fight's banners and tips are bitmap-font texts (section 3): they
call no sceFont function and are drawn as strips from the banner font's image, so neither the font trace nor the
fan check can see them; the same frame 8800 draws one (Lightning's Paradigm tip, 35 glyph strips, **traced**).
They are made once, not per frame. So the stutter and its fix concern menus; battle and story only need the fix
to keep working (quotes, dialogs, help windows), and the banners keep their own path.

## 6. For the native fix

What any host drawing must reproduce, from sections 2-4: the run quads (u/v ranges, the left-out last row, size
scaling bottom-aligned, per-line x alignment, block y alignment, line pitches), the vertex colour x 2 with the CLUT's
alpha ramp, the text and run alpha rules, the frame's blend and alpha-test state, the depth test (GEQUAL) and depth
writes (on) at the text's z, the scissor, the draw order among other sprites, the reveal (runs hidden by letter
index), and the inline images (left to the game).

### Budget: the shop screen while the list scrolls (**traced**, existing runs)

| Quantity | Shop | Source |
| --- | --- | --- |
| Texts drawn per frame | 81-85: 67-71 by layout nodes + 14 by the Draw pass, off screen | pc_bc2c, every 0x0887BC2C call |
| Letters drawn per frame (glyphs + inline images) | 304-372, ~26 of them off screen | pc_bc2c (the objects' letter counts) |
| Distinct buffers drawn per frame | 61-66 (shadows and clones share one) | pc_bc2c |
| New buffers per scroll step | ~24: 18 stat numbers (mostly 32x26) + 5-7 detail lines | font1 (559 buffers in 23 steps), pc_bc2c |
| Glyphs rasterised by the game per step | ~76 (38 for the numbers, 20-70 for the detail panel) | font1 (1755 GetCharGlyphImage in 23 steps) |
| Worst frame | one panel per frame: 18 new buffers / 38 glyphs, or 5-7 / 20-70 | pc_bc2c (a step's two panels land on two frames) |
| Sharp text today, per step | ~57 textures, ~190 glyph stamps: 2.4 textures per new buffer, as it also re-makes textures of unchanged buffers (port notes below) | font1 (1318 textures, 4337 stamps; 0x09492630 re-made 21 times with no new glyph) |
| Accessory list, per step | ~21 new texts, ~75 glyphs | acc2_d868 (47 steps) |

So a glyph-atlas drawer handles about 300-370 glyph quads per frame on this screen and about 76 new glyph
placements per step, with no new texture once the atlas holds the font's glyphs at the needed scale.

### (a) Hook on 0x0887BC2C: the host draws one text object's glyphs

**Possible** (**read**). At entry the host has everything: the object (position, scale, colour, alpha, flags,
reveal count), its buffer record and runs (u ranges, widths, sizes, colours, flags, row tops, letter indices), the
manager (line height 26, baseline 18). What it does not have is the **string**: the object and buffer do not keep it,
and the glyph positions exist only inside 0x0887F294. So the hook needs a second hook on 0x0887F294 (or on
sceFontGetCharGlyphImage, which the port already intercepts: code, x64, y64 per glyph and the buffer address, exactly
what Sharp text records today) to keep, per buffer, the glyph list `{code, x64, y64}`. With that, at 0x0887BC2C the host
can draw each glyph of each drawn run from a screen-resolution atlas: glyph pen in buffer coordinates -> screen by the
run's quad mapping (`screen = quad origin + (buffer x - run u0) x (quad width / run width)`, y likewise), skip the
game's DrawArrayN for that text and still let the game draw its images (0x0887CA50) and the overflow box. The game's
own `sceGuGetMemory`/vertex building can stay (cheap); only the DrawArrayN calls must not reach the renderer, or the
renderer must drop them.
Must reproduce: the frame's GE state at that point (blend, alpha test, depth GEQUAL at z with depth writes on,
scissor, FRAGMENT_2X, MODULATE), i.e. the glyph draws must be inserted into the GE command stream at the same place
(a host-side marker in the display list), not drawn later, or the text order against sprites changes (texts are
interleaved, **traced**).
Also skip the buffer's rasterisation cost? Not with this hook: the game still calls the font library per glyph; the
host can make sceFontGetCharGlyphImage record only and skip drawing pixels, as long as nothing reads the buffer
(none found, but the Sharp-text fallback and texture packs would then see empty buffers).

Fallbacks needed:
- bitmap-font texts (flag 0x2000, the battle banners): another drawer (0x0887EF84, strips from the banner font's
  image, scissor off); leave to the game;
- buffers whose glyph list is unknown or stale (made before the hook was on, font reloaded, the buffer changed
  after its glyphs: Sharp text's existing checks);
- a glyph the host font lacks (the game drew U+3013 instead) or a code above U+FFFF (none: UTF-16 units);
- buffers wider than 512 (two TexImage pages) and runs past 768 px (clamped on the PSP). **corrected (review)**: not
  rare in menus: the one-line help sentences of the menus are 544-736 px wide (16 of the 1424 buffers of the font1
  route, all on the screens before the shop; none made during the shop scroll). The second page is only the texture
  address moved by 128 bytes (u - 256), which `fonts::sharp_text` already resolves to column 256 of the buffer, so
  it is cheap to handle exactly; only a run past 768 px (clamped) needs the game's picture;
- the overflow box (code 0x12/0x16 texts, none in the disc's message files): alpha and depth only, can stay the game's;
- texts drawn by other callers of 0x0887BC2C (0x088D9E48 layout nodes, 0x08857134 windows, battle 0x089E1650):
  the same function, so the same hook covers them;
- anything that reads text buffers as a texture outside 0x0887BC2C (not found) and texture packs that replace a
  text buffer by its hash (they would key on the game's pixels: keep drawing them, or keep the old path when a pack
  matches).

### (b) The renderer recognises a text buffer's quad and draws glyphs instead

**Possible**, and closest to what Sharp text already does (**read** + **traced**): every text quad is a
TRIANGLE_FAN of 4 through-mode vertices (vtype 0x80011E) with a T4 texture whose address is a known buffer (Sharp
text keeps the buffers by address), CLUT 0x089D68D0, TexImage width 512. From the quad's u range the renderer knows
which glyphs of the buffer's glyph list fall inside (pen x64 within [u0, u1)), and from the quad's screen corners the
mapping to the screen; it draws those glyphs from a screen-resolution atlas, with the vertex colour and alpha, in the
command stream's place and state (so draw order and blend are kept for free). Cost: no texture per text at all.
Must reproduce: the CLUT's alpha ramp (coverage -> alpha) times the vertex alpha, colour x 2 (TEXFUNC doubling) x
vertex colour, alpha test GREATER 0/4 (affects only fully transparent pixels: a coverage atlas must do the same),
depth writes at z only where a glyph pixel survives the alpha test (later sprites with a smaller z are hidden
there; a sharper glyph covers slightly different pixels, a difference too small to matter in practice but not nil),
bilinear minification of the PSP (a sharp atlas will look different by design), the left-out 26th row, partial-u
quads (a run's quad covers only its glyphs; glyphs that straddle a run boundary would be cut at the same u), sizes
(quad height != line height: scale glyphs by quad size / run size).
Fallbacks needed: as (a), plus
- quads whose UVs do not cover whole glyphs in the expected way (a game owner drawing a text buffer through another
  path, e.g. a custom sprite: none found);
- the CLUT not being 0x089D68D0 or the format not T4 (not text: draw as before);
- texture packs that replace a text buffer (keep the pack's picture);
- a quad drawn after the buffer was freed and reused (the buffer is freed two Update passes after release; the same
  address can then hold a new text: key the glyph list on the buffer's contents or a generation, as Sharp text does).

Of the two, (b) needs no change to the game's code path and keeps every caller and order; (a) saves the game's
vertex work but needs the GE stream insertion anyway. Both need the per-buffer glyph list from the font calls.

Other points the fix must handle (**read** / **traced**):
- Shadows are separate text objects sharing a buffer (two fans from one buffer, different colour and position):
  nothing special, but a per-buffer cache must allow several draws per frame.
- The Draw pass draws texts that are off screen (the shop's 14 at y -27..-1): skipping Sharp text (or any host
  work) for quads wholly outside the scissor is free and cuts part of the cost today.
- Texts with code 0x12/0x16 (26.6 widths, the box) and glyph positions after a 512-px cut (pass 2 does not round
  there): fall back to the game's picture, or reproduce the run u ranges exactly as the fans give them.
- Run boundaries are whole pixels in the buffer, glyph pens are not: a glyph can straddle two runs, and the PSP
  cuts it at the run's u. Drawing per fan with the fan's u range as a clip reproduces that.
- Sizes (code 0x11) occur in menu text (634 times in the message packs): such runs have shorter, bottom-aligned
  quads, so glyphs must be scaled by the quad, not drawn at the line's size.
- Inline images (0x15 9,066 times, 0x1E 4,965 times in the files) keep their full 18-px size while the text
  shrinks, are one draw call each, and ignore the alpha override +0x0A; leaving them to the game keeps all of that.

**added (review)**, from the independent check (`build/scratch/text-system/review.md`):
- A quad's mapping is not the text's nominal scale: horizontally a run of w texels becomes `trunc(w x size x sx)`
  pixels; vertically 25 texel rows (v0 to v0 + 25) become `trunc(26 x sy x size)` pixels (11 at 0.44, 14 at 0.55, 26
  at 1.0: up to 4 % taller than `sy x size`; one more pixel when y0 + that is negative). Map glyphs through the fan's
  corners, as (a)'s formula says, never by scale x glyph size.
- 0x0887BC2C leaves its state set for whatever draws next: TexWrap(CLAMP, CLAMP), the CLUT mode and load, TexMode
  T4, the TexImage, and after an overflow box the material colour 0xFF0000FF. A hook that skips the whole function
  (approach (a)) must still emit these or show that no later draw relies on them; (b) keeps them for free.
- Fans with vertex alpha 0 write nothing (every texel fails the alpha test: no colour, no depth). They are common:
  the traced shadow pair of buffer record 0x08BF877C was drawn 598 of 600 times at alpha 0 (a faded panel). Skipping
  them, before any texture or glyph work, is free and exact.
- Today's Sharp-text texture is the TexImage's size, `512 x 32 x factor^2 x 4` bytes (1 MiB at 4x) for every text,
  whatever the buffer's width: a 32-px number costs as much as a 512-px line.
- Port-side cases the map cannot show: frame interpolation (the port draws 60 frames from the game's 30) and the
  Stretch / ultrawide 2D (different x and y scales). Glyph quads must be made from the fan's corners as the
  renderer finally draws them, and an atlas made at one scale needs a scale per axis or a per-axis transform.
- A buffer put back by a save state or rollback without glyph calls is not in the registry as such: Sharp text's
  byte check already falls back to the game's picture there.
- For (a): PortableKit's `docs/ARCHITECTURE.md` says generated units chain directly only when the target has not
  been replaced by a host override, so an override of 0x0887BC2C should see all three callers, the Draw pass
  included (**read**, not tested).

**added (review 2)**, inline images and the host drawer (`build/scratch/text-system/review.md`, "Second review"):
- The game's images come right after their own text's quads (0x0887BC2C calls 0x0887CA50 after its last
  DrawArrayN), so glyphs drawn in the fans' place keep the order; images need no host work and no change.
- Images are centred vertically on the line (section 4), not aligned with the glyph quads: nothing to reproduce
  if the game keeps drawing them, but a host that ever drew icons itself must use the same `yoffs` rule.
- Zero-width fans (an empty run before a leading image or after a trailing group) are drawn and must be skipped,
  not divided by (u1 - u0 = 0).
- Glyph pens and run u ranges can disagree by up to 1 px after code 0x19 (pass 2 rounds, pass 1 does not), as after
  the 512-px cut: clip each glyph to its fan's u range, as already advised.
- 0x0887CA50 sets its own blend, alpha test and texture function and binds its own texture, so it does not depend on
  the text quads' state; a host that skips the text DrawArrayN leaves its state intact (it inherits only
  TEXTURE_2D, the filter, the depth mask and the scissor, which the text quads do not change either).

### What the port already has, and what is missing (**read**, PortableKit unchanged)

- `host/hle/hle_font.cpp` (~line 211): the port's own sceFontGetCharGlyphImage / _Clip draws each glyph into guest
  memory and calls `fonts::before_glyph` / `fonts::after_glyph` around it.
- `host/fonts/sharp_text.cpp`: a registry (`std::map` by buffer address, up to 512 buffers of at most 1 MiB, least
  recently used dropped) holding per buffer its shape (address, width, height, bytes per line, 4/8-bit), the glyph
  stamps `{code, x64, y64, clip}` drawn since the buffer was last clear (0x0887F294 clears every new buffer), a copy
  of the bytes as the last glyph left them, a "foreign" flag and the font generation. So **the per-buffer glyph list
  that (a) and (b) need exists today**, keyed by address and checked against the bytes (which also catches a freed
  block reused for a new text).
- `host/gpu/vulkan_renderer.cpp`: `texture_for` (~line 5304) -> `sharp_text_picture` (~5603) -> `fonts::sharp_text`:
  for a CLUT4/CLUT8 unswizzled texture inside a known, unchanged buffer, every stamp is rasterised on the CPU at
  `factor` (internal lines per PSP line, 2-8) into a coverage image that becomes a new texture (cached, evicted past
  SHARP_TEXT_MEMORY, 256 MB). CPU rasterisation and a new texture per buffer is the stutter's cost; during the shop
  scroll it makes about 2.4 textures per new buffer (budget above), re-making some for buffers that did not change.
  The reason (**read**, `host/gpu/texture_decode.cpp` `texture_key`, consistent with the trace): the cache key
  hashes `TexImage width x height x 4 bits` = 512 x 32 / 2 = 8192 bytes from the buffer's address, while a one-line
  buffer is only `tbw x 26 / 2` bytes (416 for 32 px). The key therefore covers the next ~8 KB of the text-buffer
  heap (manager+0x14, section 2; **corrected (review)**), i.e. the following buffers: every new text made there changes the keys of the older texts before it, whose
  textures (and Sharp text pictures) are then made again (0x09492630 was re-made 21 times in the hold while new
  buffers were drawn at 0x09493850-0x09494430). Keying a text buffer by `tbw x rows` bytes would stop that today,
  before any glyph atlas.
- Missing for (b): a glyph atlas (each code rasterised once per scale, kept across texts); a draw path that, for a
  prim-5 fan with vtype 0x80011E whose texture lies in such a buffer, emits per-glyph quads from the atlas in the
  fan's place in the command stream (fan colour and alpha, z, the current blend/alpha-test/depth/scissor state,
  colour doubling), mapping buffer coordinates to the fan's screen rectangle and clipping to the fan's u/v range;
  the stamp -> fan assignment (bitmap x range overlapping [u0, u1), y inside [v0, v0 + 26)). Where: beside
  `texture_for` in the renderer's draw-call path, with the atlas next to `sharp_text.cpp`.
- Missing for (a): a hook on 0x0887BC2C in the recompiled game (a PortableKit function override, like the guard at
  0x089DE360) and a marker in the GE list the renderer resolves; so (a) needs (b)'s renderer work as well.
- `host/fonts/game_font.hpp` describes **another game's** text path (glyphs copied into 256x256 atlas pages of
  cells, one sprite per character). **This game does not use it**: only 0x0887F294 calls
  sceFontGetCharGlyphImage, always into a whole-string buffer (traced: every glyph of the shop, list, story and
  battle runs).

## 7. Functions

Matched = byte-identical C in the build before tonight (`build/scratch/text-system/matched.txt`); "tonight" =
matched tonight (`build/dc-results/agent87.md`, `agent89.md`, `agent91.md`; their files were renamed to the
names below before integration); "no" = not matched. **Name** is the role name to use
(lower_snake_case); the file name an earlier agent gave is in brackets where it differs. Rows marked (not text)
sit in the range but belong to other systems; their names are the earlier ones unless the role was read.

| Address | Name [file name] | Matched | Role |
| --- | --- | --- | --- |
| 0x0887A0EC | multi_dtor | yes | (not text) destructor |
| 0x0887A19C | object_deleting_dtor [main_0887a19c] | yes | (not text) deleting destructor, members +0x1AF0/+0x1874/+0x90 |
| 0x0887A204 | sphere_overlap | tonight | (not text) sphere overlap test |
| 0x0887A2B4 | label_handle_ctor [main_0887a2b4] | yes | label handle: vtable 0x089C9D30 at +0xC |
| 0x0887A2EC | label_handle_dtor [main_0887a2ec] | yes | label_close, then delete if asked |
| 0x0887A344 | label_handle_reset [main_0887a344] | yes | two words = 0 |
| 0x0887A354 | label_close [sound_release] | yes | a cached label is hidden again (0x0887D45C), else released |
| 0x0887A3C8 | label_open [font_open] | yes | (file, entry): a cached label (0x0887D330) or a new text (0x0887AD78) |
| 0x0887A484 | label_handle_is_open [main_0887a484] | yes | first byte |
| 0x0887A48C | label_handle_state [voice_state] | yes | state byte if open |
| 0x0887A4C4 | label_handle_text [voice_state] | yes | the text (+4) if open |
| 0x0887A4FC | buffer_reset [main_0887a4fc] | yes | D012TextBuffer: refcount 1, run/line counts 1, lists empty |
| 0x0887A524 | buffer_ref [main_0887a524] | yes | refcount + 1 (clone) |
| 0x0887A534 | buffer_unref [main_0887a534] | yes | refcount - 1 (release) |
| 0x0887A544 | buffer_collect [delay_take] | yes | refcount 0 -> -1; < 0 -> hands the block back |
| 0x0887A584 | buffer_add_run [queue_push] | yes | append a run to +0x1C |
| 0x0887A5D0 | buffer_clear_runs [main_0887a5d0] | yes | empty the run list |
| 0x0887A5E0 | buffer_next_run [main_0887a5e0] | yes | the run after prev (first if 0) |
| 0x0887A5F8 | buffer_set_on_free [pending_set] | yes | free callback, only while refcount < 2 |
| 0x0887A61C | text_file_ctor [main_0887a61c] | yes | text-file object |
| 0x0887A644 | text_file_delete [main_0887a644] | yes | 0x0887AD1C, id back to its pool |
| 0x0887A690 | text_file_reset [main_0887a690] | yes | |
| 0x0887A6A0 | text_file_fixup | no | walk a loaded text file's entries |
| 0x0887A800 | text_file_find_entry [entry_size] | yes | offset of an entry |
| 0x0887A898 | text_file_header_size [header_size] | yes | |
| 0x0887A8F0 | text_file_has_long_headers [magic_test] | yes | 4-byte entry headers |
| 0x0887A918 | text_file_is_utf8 [magic_test] | yes | |
| 0x0887A940 | text_utf8_to_utf16 | no | entry -> static UTF-16 0x09B48C88 (0x3FE units), codes copied with their arguments |
| 0x0887AACC | text_file_load | tonight | disc or package -> heap block |
| 0x0887AC24 | text_file_bind_header [header_bind] | yes | |
| 0x0887AC70 | text_file_copy | no | copy a text file into a heap block |
| 0x0887AD1C | text_file_free | no | |
| 0x0887AD78 | text_new_from_file_entry | tonight | a text from an entry of a text file (colour 0, caller sets it) |
| 0x0887AEAC | text_new_from_block [text_line_draw] | yes | a text from entry `index` of a {count, data} block |
| 0x0887AFB0 | text_file_is_loaded [main_0887afb0] | yes | |
| 0x0887AFBC | text_file_entry_string [text_line_get] | yes | entry string (UTF-8 converted) |
| 0x0887B080 / 0x0887B0D0 | text_file_id_take / text_file_id_give [pool_take/pool_give] | yes | 8-entry id pool |
| 0x0887B120 / 0x0887B130 | text_files_from_package_on / _off [flag_set/flag_clear] | yes | 0x089D68A8 = 1 / 0 |
| 0x0887B13C | text_file_pool_init [block_pool_init] | yes | |
| 0x0887B1B4 | text_multi_block [entry_get] | yes | block `index` of a multi-block text file |
| 0x0887B1E4 / 0x0887B3B8 | block_ref_init / block_ref_free [main_*] | yes | |
| 0x0887B1F4 | text_multi_free | no | free a multi-block file, id back |
| 0x0887B260 | text_multi_load | no | load a multi-block text file |
| 0x0887B3EC | text_new_from_multi_entry [text_entry_line_draw] | yes | a text from (block, entry); missing: "" in 0xFFFFFFFF |
| 0x0887B500 | text_multi_entry_string [text_line_after] | yes | |
| 0x0887B5A8 / 0x0887B5F8 | text_multi_id_take / text_multi_id_give [pool_take/pool_give] | yes | 4-entry id pool |
| 0x0887B648 | text_multi_pool_init [block_pool_init] | yes | |
| 0x0887B6A0 | text_init | tonight | defaults: (0, -1), scales 1.0, flags 0x420, alpha -1, colour 0 |
| 0x0887B748 | text_set_position [play_range] | yes | x, y (y - 1 unless bitmap text) |
| 0x0887B778 | text_set_from_matrix [sprite_place] | yes | scale = m[0], m[5]; position = m[12], m[13] |
| 0x0887B7D0 | text_set_scale [play_pair] | yes | +0x18/+0x1C |
| 0x0887B7DC | text_set_base_scale [play_pair] | yes | +0x20/+0x24 |
| 0x0887B7E8 | text_reveal_all [play_start] | yes | shown = letters, flags 0x2 |
| 0x0887B820 | text_set_colour [main_0887b820] | yes | +0x0C |
| 0x0887B828 | text_set_colour_bytes [main_0887b828] | yes | +0x0C from 4 bytes |
| 0x0887B8A8 | text_set_alpha [play_clamp] | yes | +0x0A clamped -1..255 |
| 0x0887B900 | text_set_line_pitch [play_set] | yes | +0x08 |
| 0x0887B908 | text_finish_reveal [main_0887b908] | yes | calls 0x0887B7E8 |
| 0x0887B924 / 0x0887B934 / 0x0887B944 | text_show / text_show_once / text_hide [main_*] | yes | 0x400 / 0x1000 / ~0x400 |
| 0x0887B958 / 0x0887B970 / 0x0887B98C | text_align_right / text_align_left / text_align_center [play_flags, main] | yes | 0x4 / none / 0x8 |
| 0x0887B9A4 / 0x0887B9C4 / 0x0887B9E4 | text_valign_top / text_valign_middle / text_valign_bottom [play_flags] | yes | 0x10 / 0x40 / 0x20 |
| 0x0887BA04 | text_align_reset [play_flags] | yes | clears 0x4..0x100 |
| 0x0887BA48 | text_set_bitmap_source [main_0887ba48] | yes | source = record, flags 0x2000 |
| 0x0887BA5C | text_release [main_0887ba5c] | yes | 0x0887DB1C on the manager |
| 0x0887BA80 | text_set_on_free [sprite_draw] | yes | buffer free callback (not bitmap) |
| 0x0887BAD4 | text_reveal_step [anim_step] | yes | letter-by-letter reveal |
| 0x0887BB6C | text_width | tonight | buffer +0x10 (or bitmap width) x scale_x |
| 0x0887BBC8 / 0x0887BBF4 | text_clear_marks / text_marks [main_*] | yes | +0x28 |
| 0x0887BC2C | text_draw | no | **draw one text** (section 4) |
| 0x0887CA50 | text_draw_images | no | inline images of a text |
| 0x0887CEB0 / 0x0887CEE8 | label_cache_ctor / label_cache_dtor [main_*] | yes | Obj9C9D48 (static 0x09AEF804, 10 slots) |
| 0x0887CF40 | label_cache_reset [table_reset] | yes | |
| 0x0887CF80 | label_cache_clear [main_0887cf80] | yes | release all, reset |
| 0x0887CFAC | label_cache_start [main_0887cfac] | yes | clear, active = 1 |
| 0x0887CFDC | label_cache_is_active [main_0887cfdc] | yes | |
| 0x0887CFE4 | label_cache_instance [main_0887cfe4] | yes | built on first use |
| 0x0887D034 | label_cache_add | no | preload a hidden label from a file entry (battle 0x08B1E5DC: 57 calls) |
| 0x0887D160 | label_cache_release | no | |
| 0x0887D2A8 | label_cache_release_all [items_apply] | yes | |
| 0x0887D330 | label_cache_show | no | find (file, entry), show it |
| 0x0887D45C | label_cache_hide | no | |
| 0x0887D574 | bitmap_font_ctor [main_0887d574] | yes | |
| 0x0887D5C8 | bitmap_font_load_metrics [main_0887d5c8] | yes | copy the 0x10-byte head and 0x300 widths |
| 0x0887D60C | bitmap_font_clear_metrics [main_0887d60c] | yes | |
| 0x0887D648 | bitmap_font_clear_texts [main_0887d648] | yes | the 28 records: in use 0, count 0 |
| 0x0887D66C / 0x0887D674 | bitmap_font_set_image / bitmap_font_clear_image [main_*] | yes | +0x310 |
| 0x0887D67C | bitmap_text_ref [main_0887d67c] | yes | record refcount + 1 |
| 0x0887D68C | bitmap_text_release [timer_tick] | yes | refcount - 1; at 0 free its images |
| 0x0887D6E0 | font_lib_alloc | no | sceFontNewLib allocator ("SYSTEM FONT" heap, 16-byte header) |
| 0x0887D750 | font_lib_free [main_0887d750] | yes | |
| 0x0887D7B8 | text_buffer_alloc | no | a text buffer block from the heap at manager+0x14 ("SYSTEM FONT T" or a swapped-in one; **corrected (review)**) |
| 0x0887D828 | text_buffer_free [main_0887d828] | yes | |
| 0x0887D868 | text_new | tonight | **make a text from a string** |
| 0x0887DA08 | text_images_free | tonight | give an image list back to its pool |
| 0x0887DAC0 | text_drop_source | no | buffer / bitmap record reference - 1 |
| 0x0887DB1C | text_manager_release | no | release: drop source, unlink, back to the pool |
| 0x0887DBD4 | text_update_all | no | **Update pass** |
| 0x0887DE04 | text_draw_all | no | **Draw pass** |
| 0x0887DF14 | text_bytes_to_utf16 | tonight | 8-bit string -> UTF-16 (pair table 0x089D6964, '?') |
| 0x0887E090 / 0x0887E0DC | text_image_set_set / text_image_set_reset [slot_set/slot_reset] | yes | inline image sets |
| 0x0887E114 | text_image_new | no | an inline image node |
| 0x0887E278 | text_manager_ctor | no | the four pools |
| 0x0887E4C4 | text_manager_dtor [dtor] | yes | |
| 0x0887E4F4 / 0x0887E514 | text_update_task / text_draw_task [main_*] | yes | the tasks' functions |
| 0x0887E534 | text_clone | no | **clone a text** (shares the buffer) |
| 0x0887E630 | text_heap_t2_push [font_heap_swap] | yes | "SYSTEM FONT T2" at +0x10/+0x14, old pair to +0x18/+0x1C |
| 0x0887E684 / 0x0887E718 | state_pop / child_ready | yes | (not text) state machine |
| 0x0887E74C / 0x0887E77C | text_heap_t3_new / text_heap_delete [font_heap_init, main] | yes | |
| 0x0887E798 / 0x0887E7B4 | text_heap_t_save / text_heap_t_restore [main_*] | yes | +0x10/+0x14 <-> +0x20/+0x24 |
| 0x0887E7D0 | text_images_release [main_0887e7d0] | yes | 0x0887DA08 on the manager |
| 0x0887E7F4 | control_code_arg_units [glyph_half_width] | yes | argument units of a control code |
| 0x0887E81C | text_copy_remapped | tonight | UTF-16 copy through the {from, to} table 0x089D6988 |
| 0x0887E8C8 | text_length | tonight | UTF-16 length to a stop code, skipping code arguments |
| 0x0887E9CC | bitmap_text_layout | no | **lay out a 0x001C byte string** (battle banners) |
| 0x0887EF84 | bitmap_text_draw | no | **bitmap-font draw** |
| 0x0887F294 | text_layout | no | **measure, cut into runs, rasterise** (sceFont) |
| 0x08881954 | text_font_init | no | **font init** (heaps, sceFont, pools, tasks) |
| 0x08881F2C, 0x08881F4C | heaps_init, dtor | yes | (not text) system heaps |
| 0x08881F78 / 0x08881FFC | system_heaps_set / system_heaps_clear | no | (not text) +0xECDC00/+0xECDC04 |
| 0x0888204C | system_heap_alloc | yes | (not text) |
| 0x088820D8 | heap_alloc | tonight | (not text) |
| 0x0888208C-0x088822B4 | heap_alloc, state_init, dtor, system_task_heap, main_* | yes | (not text) heaps, task heap |
| 0x088822BC-0x0888274C, 0x088828BC, 0x08882950 | task_construct ... task_unlink | yes | (not text) the task system |
| 0x08882790 | task_insert_by_priority | no | (not text) |
| 0x08882958 | frustum_build | no | (not text) |
| 0x08882CBC / 0x08882D78 | frustum_sphere_test / frustum_sphere_test_planes | no | (not text) sphere against the frustum planes |
| 0x08882E44-0x08882FF4 | volatile_mem_lock, object_init, sound_dtor, main_* | yes | (not text) |

### Related functions outside the range (all **read**; 0x088D44EC and 0x088D8A98 matched tonight)

| Address | Name | Role |
| --- | --- | --- |
| main 0x088D40BC | layout_attach_label | link one label into a layout node's text list (flag 0x8000, alignment, pitch, matrix) |
| main 0x088D428C | layout_attach_labels | the same for a binding table (`D012TextBinding`), optionally emptying the nodes' lists first |
| main 0x088D44EC / 0x088D4584 | layout_bind_labels / layout_bind_label | each frame: a node part's matrix and colour into its label (0x088D44EC matched tonight: `src/main/node_texts_bind_088d44ec.c`; 0x088D4584 matched before) |
| main 0x088D442C | layout_release_labels | release the bound labels, empty the nodes' lists |
| main 0x088D8FD0 | layout_label_open | a layout file's own label (from the label cache or a new text), linked to its node |
| main 0x088D8A98 | layout_unlink_label | unlink a label from its node's text list, clear 0x8000, close the handle (matched tonight: `src/main/node_text_unlink_088d8a98.cpp`) |
| main 0x088D9E48 | layout_node_draw_texts | draw a node's texts with 0x0887BC2C at the node's z (from the layout draw 0x088D16F4) |
| main 0x08856D70 / 0x08857134 | banner_open / banner_draw | battle banner: a text from a fighter archive's entry (0x0887AD78), drawn after the window pieces |
| main 0x0899D2F8 | img_bind | write an image's texture commands into the display list (see docs/port/texture-pipeline.md) |
| main 0x089A119C | sceGuDepthMask | ZMSK (1 = depth writes off) |
| menu 0x08AFE03C | stat_panel_make | the stat comparison panel's 18 numbers and 6 arrows (shop, accessories) |
| menu 0x08AFE69C | stat_panel_update | on its countdown: release, 0x08AFE03C, 0x08AF4F6C; each call: bind |
| menu 0x08AF4F6C | stat_panel_attach | 0x088D428C with tables 0x08B006BE / 0x08B00726 (or 0x08B007EE / 0x08B0085E) |

## Open questions

Settled by the third pass: where the 0x001C strings are (battle banners and tips in the fighters' archives, traced
on screen in a fight, section 3), the depth-write state at text draws (on, section 4), whether the message files use
0x12/0x16 (no, section 3), and who links the shop's numbers into the layout (0x08AF4F6C -> 0x088D428C -> 0x088D40BC,
section 5). Still open:

1. Which banner strings each fighter's archive holds besides names and tips (tools/character_text.py decodes only
   the glyphs it knows), and the banner font's resource name (the 'F' resource of 0x088D2980, the image at
   0x08C8DF30 in the traced fight).
2. Whether any string built by code uses 0x12, 0x14 or 0x16 (no message file does; code builds strings with
   sprintf and 0x0887DF14), and whether a table at a bare file start (no container, not scanned) holds any.
3. A long ability list: the test save's Lightning knows three abilities per slot; the ability screens re-make
   only a description panel per step, so they should not stutter like the shop and accessory lists.
4. Whether any game code reads a text buffer's pixels (none found by reading; not proven by a trace).
5. Depth writes in battle at text draws (battle quotes, help windows): the battle code turns writes off and on
   around its own draws; the state when its texts are drawn was not dumped.
6. Whether keying text buffers by their real size (`tbw x rows`, section 6 port notes) removes the re-made
   textures in practice: a port-side change, not tested (PortableKit left unchanged).
