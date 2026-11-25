/*
 * text_types.h - Dissidia 012 (USA ULUS10566) text system ("SYSTEM_FONT") data layouts.
 *
 * Established by reading the retail disassembly of main 0x0887A000-0x08883000 and checked in the running PC
 * port on 2026-11-25 (shop list scroll). See docs/decomp/text-system.md for the functions that read and write
 * each structure, the control codes and the draw calls.
 *
 * Marks in the field comments (as in docs/port/texture_types.h):
 *   V  verified in the disassembly (a load/store at that offset with that width, used that way)
 *   R  seen at run time in the port (the value in guest memory, the call or the store that wrote it)
 *   I  inferred (name/meaning from how it is used, or offset known but use not fully traced)
 *   D  from the PSP SDK's documented layout (SceFontInfo, SceFontCharInfo), not re-verified here
 *
 * Conventions:
 *   - All structures are the game's guest memory (PSP, 32-bit little endian). Every guest pointer is a
 *     uint32_t guest address (suffix _addr), so the layouts are the same in a 64-bit host build.
 *   - Unknown fields are unk_XX (XX = hex offset). Padding is explicit.
 *   - Sizes are asserted only where the size is known from an allocation, a stride or a copy.
 *   - Compiles as C and C++ under SNC (the decomp's compiler) and under a host compiler: the assertions use
 *     static_assert / _Static_assert where the language has them, else a negative-array typedef.
 */
#ifndef D012_TEXT_TYPES_H
#define D012_TEXT_TYPES_H

#ifdef __SNC__
/* SNC 1.2 (the decomp's compiler) ships neither <stdint.h> nor <stddef.h>. */
#ifndef D012_SNC_STDINT_DEFINED
#define D012_SNC_STDINT_DEFINED
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
#endif
#define D012_TEXT_OFFSETOF(type, field) ((unsigned int)&((type *)0)->field)
#else
#include <stdint.h>
#include <stddef.h>
#define D012_TEXT_OFFSETOF(type, field) offsetof(type, field)
#endif

#ifndef D012_STATIC_ASSERT
#if defined(__cplusplus) && __cplusplus >= 201103L
#define D012_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#elif !defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define D012_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
/* SNC 1.2 (C and C++) has neither: a typedef of a char array whose size is -1 when cond is false. */
#define D012_TEXT_JOIN2(a, b) a##b
#define D012_TEXT_JOIN(a, b) D012_TEXT_JOIN2(a, b)
#define D012_STATIC_ASSERT(cond, msg) typedef char D012_TEXT_JOIN(d012_assert_line_, __LINE__)[(cond) ? 1 : -1]
#endif
#endif

#define D012_TEXT_AT(type, field, offset) \
    D012_STATIC_ASSERT(D012_TEXT_OFFSETOF(type, field) == (offset), #type "." #field)

#ifdef __cplusplus
extern "C" {
#endif

/* The manager's global pointer and static instance. */
#define D012_TEXT_MANAGER_PTR      0x089D6998u  /* V  D012TextManager* (stored by 0x088769CC at 0x08877208) */
#define D012_TEXT_MANAGER_STATIC   0x08BEE94Cu  /* V R  the static object (guard byte 0x08BEE948) */
#define D012_TEXT_CLUT             0x089D68D0u  /* V  16 x 16-bit 4444 entries: 0x0FFF, 0x1FFF .. 0xFFFF */
#define D012_TEXT_EMPTY_STRING     0x09B49088u  /* V  UTF-16 "" used for missing message entries */
#define D012_TEXT_ARG_BYTES        0x089C9D2Cu  /* V  read as int[code] for codes 0x10-0x1F only (0x089C9D6C +
                                                 *    (code - 0x10) * 4): 0x10:4 0x11:2 0x14:4 0x15:2 0x1E:2 0x1F:2,
                                                 *    others 0 (bytes of arguments after the code). Corrected (review):
                                                 *    not an int[0x20]; the words below 0x089C9D6C are other data
                                                 *    (D012_TEXT_TEX_HEIGHTS among them) */
#define D012_TEXT_TEX_HEIGHTS      0x089C9D58u  /* V  u16 {512,256,..,1}: texture height = power of two >= rows */
#define D012_TEXT_TRADEMARK        0x089D6960u  /* V  UTF-16 "™" inserted by control code 0x1F 5 */
/* Icon ids of the variable codes (added, review 2: read in 0x0887F294's two passes). */
#define D012_TEXT_ICONS_ITEM_KIND  0x089D6904u  /* V  int[4] {22,23,24,25}: code 0x18's leading icon, by the item kind
                                                 *    (s8 at +0x0C of the record 0x088B97E8 gives) */
#define D012_TEXT_ICONS_1A_BEFORE  0x089D6914u  /* V  int[4] {12,12,12,12}: code 0x1A's leading icon, by (s8) record+0x12 */
#define D012_TEXT_ICONS_1A_AFTER   0x089D6924u  /* V  int[4] {13,14,15,16}: code 0x1A's trailing icon, by (s8) record+0x14 */
#define D012_TEXT_ICON_1D          0x1F         /* V  code 0x1D's leading icon id (no trailing icon) */
#define D012_TEXT_ICON_ITEM_AFTER  21           /* V  code 0x18's trailing icon id, or manager.icon_20 (20) */
#define D012_TEXT_BUTTON_SWAP      0x089D5F1Du  /* V  u8: set = ids 0x80 / 0x81 map to 0x31 / 0x2F instead of 0x2F / 0x31
                                                 *    (0x0887E114) */

/* Pools: counts and item sizes, all from 0x0887E278 (manager constructor) and 0x08881954 (font init). */
enum {
    D012_TEXT_POOL_TEXTS   = 0x122, /* V  290 text objects of 0x58 */
    D012_TEXT_POOL_BUFFERS = 0xCD,  /* V  205 buffer records of 0x40 */
    D012_TEXT_POOL_RUNS    = 0x1D6, /* V  470 runs of 0x18 */
    D012_TEXT_POOL_IMAGES  = 0xAA,  /* V  170 inline images of 0x10 */
    D012_TEXT_LINES_MAX    = 32,    /* V  0x0887BC2C keeps per-line x/y offsets for 32 lines */
    D012_TEXT_BUFFER_WIDTH_MAX = 1024, /* V  0x0887F294 caps a buffer at 1024 px (and stops drawing glyphs past it) */
    D012_TEXT_TEXIMAGE_WIDTH = 512     /* V  0x0887BC2C always passes width 512 to sceGuTexImage */
};

/* Flags of D012Text.flags (+0x10). */
enum {
    D012_TEXT_SCALE_IMAGES  = 0x0001, /* V  0x0887CA50: images scaled by scale / base_scale */
    D012_TEXT_REVEALED      = 0x0002, /* V  every letter shown (0x0887B7E8); images then drawn without limit */
    D012_TEXT_ALIGN_RIGHT   = 0x0004, /* V  x is the right edge of each line */
    D012_TEXT_ALIGN_CENTER  = 0x0008, /* V  x is the centre of each line */
    D012_TEXT_VALIGN_TOP    = 0x0010, /* V  set by 0x0887B9A4 (with 0x20, 0x40 clear): y is the top */
    D012_TEXT_VALIGN_BOTTOM = 0x0020, /* V  default (0x0887B9E4 from the init): y is the bottom of the block */
    D012_TEXT_VALIGN_MIDDLE = 0x0040, /* V  set by 0x0887B9C4: y is the middle of the block */
    D012_TEXT_DRAWN         = 0x0200, /* V  drawn this frame: set by the Draw pass, cleared by the Update pass */
    D012_TEXT_VISIBLE       = 0x0400, /* V  default on (0x0887B924 sets, 0x0887B944 clears) */
    D012_TEXT_NO_IMAGES     = 0x0800, /* V  inline images not drawn; per-run colours ignored */
    D012_TEXT_SHOW_ONCE     = 0x1000, /* V  drawn by the next Draw pass even when not VISIBLE; 0x0887BC2C clears it */
    D012_TEXT_BITMAP_FONT   = 0x2000, /* V  source is a D012BitmapText (control code 0x1C strings) */
    D012_TEXT_OWN_COLOUR    = 0x4000, /* V  per-run / per-glyph colours ignored: the object's colour only */
    D012_TEXT_MANUAL        = 0x8000  /* V R  skipped by the Draw pass: set when a text is linked into a layout
                                       *      node (0x088D40BC, 0x088D8FD0, 0x088CCF74, 0x088CD754), drawn by
                                       *      0x088D9E48 (which does not test this bit) */
};

/* Flags of D012TextRun.flags (+0x0D). */
enum {
    D012_RUN_NEW_LINE  = 0x01, /* V  first run of a line (code 0x000A only; not after 0x12 / 0x16) */
    D012_RUN_END       = 0x02, /* V  set on the run BEFORE code 0x0012: 0x0887BC2C stops after it */
    D012_RUN_BREAK4    = 0x04, /* V  set on the run before code 0x0016 (clears 0x02); the runs after it are not
                                *    new lines for 0x0887BC2C and keep 26.6 widths (I: hidden continuation) */
    D012_RUN_IMAGE     = 0x08, /* V  placeholder for an inline image: width not scaled by scale_x */
    D012_RUN_HIDDEN    = 0x10  /* V  not drawn (always set with 0x08) */
};

/* 8-byte ring node of a pool. The ring is closed (last node's next = first node). */
typedef struct D012TextPoolNode {
    uint32_t item_addr;      /* +0x00 V  free item, 0 while taken */
    uint32_t next_addr;      /* +0x04 V  next node */
} D012TextPoolNode;
D012_STATIC_ASSERT(sizeof(D012TextPoolNode) == 0x08, "D012TextPoolNode size");

/* One corner of a drawn quad: GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D (0x80011E). */
typedef struct D012TextVertex {
    uint16_t u;              /* +0x00 V  texels */
    uint16_t v;              /* +0x02 V */
    uint32_t colour;         /* +0x04 V  ABGR; 0x80 is full intensity (GU_FRAGMENT_2X is on) */
    int16_t  x;              /* +0x08 V  screen pixels */
    int16_t  y;              /* +0x0A V */
    int16_t  z;              /* +0x0C V  0xFFF0 from the Draw pass */
    int16_t  pad_0E;         /* +0x0E V  not written */
} D012TextVertex;
D012_STATIC_ASSERT(sizeof(D012TextVertex) == 0x10, "D012TextVertex size (0x0887BC2C: 64 bytes per quad)");

/* A text object (pool item, 0x58). Made by 0x0887D868 (string) or cloned by 0x0887E534 (memcpy 0x58),
 * released by 0x0887BA5C -> 0x0887DB1C. Set up by 0x0887B6A0. */
typedef struct D012TextMark {
    int16_t  x;              /* +0x00 V  pen x (px) of the glyph after control code 0x1F n, 101 <= n <= 200 */
    int16_t  y;              /* +0x02 V  glyph top y (px) in the buffer */
    int16_t  id;             /* +0x04 V  n - 1 */
    int16_t  unk_06;         /* +0x06 V  cleared by 0x0887BBC8 only */
} D012TextMark;
D012_STATIC_ASSERT(sizeof(D012TextMark) == 0x08, "D012TextMark size (0x0887BBC8 stride)");

typedef struct D012Text {
    uint32_t source_addr;    /* +0x00 V  D012TextBuffer* (flag 0x2000 clear) or D012BitmapText* (set) */
    int16_t  shown;          /* +0x04 V  letters shown: runs whose first letter > shown are not drawn
                              *          (set to the letter count by 0x0887B7E8; stepped by 0x0887BAD4) */
    int8_t   reveal_wait;    /* +0x06 V  frames per letter (0: no letter-by-letter reveal) */
    int8_t   reveal_tick;    /* +0x07 V */
    int16_t  line_pitch;     /* +0x08 V  0: line height * scale_y * largest size in the line (0x0887B900) */
    int16_t  alpha;          /* +0x0A V  -1 or 0..255: replaces every vertex alpha (0x0887B8A8 clamps) */
    uint32_t colour;         /* +0x0C V  ABGR vertex colour (0x0887B820 = arg 4 of 0x0887D868, 0x0887B828) */
    uint32_t flags;          /* +0x10 V  D012_TEXT_* */
    int16_t  x;              /* +0x14 V  screen x (0x0887B748, 0x0887B778) */
    int16_t  y;              /* +0x16 V  screen y; stored as y - 1 unless BITMAP_FONT */
    float    scale_x;        /* +0x18 V  1.0 by default (0x0887B7D0) */
    float    scale_y;        /* +0x1C V */
    float    base_scale_x;   /* +0x20 V  1.0 by default (0x0887B7DC); images: scale_x / base_scale_x */
    float    base_scale_y;   /* +0x24 V */
    D012TextMark marks[4];   /* +0x28 V  positions marked in the string (0x0887BBF4 returns them); 0x0887F294
                              *          does not bound them: a 5th mark overwrites next_addr */
    uint32_t next_addr;      /* +0x48 V  manager's active list */
    uint32_t node_next_addr; /* +0x4C V  layout node's text list (node+0x30): linked by 0x088D8FD0, drawn by 0x088D9E48 */
    uint32_t prev_addr;      /* +0x50 V */
    uint32_t node_prev_addr; /* +0x54 V  unlinked by 0x088D8A98 */
} D012Text;
D012_STATIC_ASSERT(sizeof(D012Text) == 0x58, "D012Text size (pool stride, 0x0887E534 copy)");
D012_TEXT_AT(D012Text, source_addr, 0x00);
D012_TEXT_AT(D012Text, shown, 0x04);
D012_TEXT_AT(D012Text, reveal_wait, 0x06);
D012_TEXT_AT(D012Text, reveal_tick, 0x07);
D012_TEXT_AT(D012Text, line_pitch, 0x08);
D012_TEXT_AT(D012Text, alpha, 0x0A);
D012_TEXT_AT(D012Text, colour, 0x0C);
D012_TEXT_AT(D012Text, flags, 0x10);
D012_TEXT_AT(D012Text, x, 0x14);
D012_TEXT_AT(D012Text, y, 0x16);
D012_TEXT_AT(D012Text, scale_x, 0x18);
D012_TEXT_AT(D012Text, scale_y, 0x1C);
D012_TEXT_AT(D012Text, base_scale_x, 0x20);
D012_TEXT_AT(D012Text, base_scale_y, 0x24);
D012_TEXT_AT(D012Text, marks, 0x28);
D012_TEXT_AT(D012Text, next_addr, 0x48);
D012_TEXT_AT(D012Text, node_next_addr, 0x4C);
D012_TEXT_AT(D012Text, prev_addr, 0x50);
D012_TEXT_AT(D012Text, node_prev_addr, 0x54);

/* A run: a stretch of one line drawn as one quad (pool item, 0x18). Made by 0x0887F294's first pass at the
 * string's start, at each line break, colour/size change and inline image, and once when a line passes 512 px. */
typedef struct D012TextRun {
    int32_t  width;          /* +0x00 V  26.6 while measuring, then whole pixels ((w + 63) >> 6); the run before
                              *          code 0x12/0x16 and every later run are never converted (stay 26.6) */
    uint32_t colour;         /* +0x04 V  ABGR; top byte 0 (0xFFFFFF, code 0x1B) = the object's colour */
    float    size;           /* +0x08 V  code 0x11 n: n / 100; scales the quad (bottom-aligned), not the glyphs;
                              *          1.0 for image placeholders and for the run right after them */
    uint8_t  row_top;        /* +0x0C V  line * line height, u8 (wraps past 255); 0x0887BC2C does not read it */
    uint8_t  flags;          /* +0x0D V  D012_RUN_* */
    uint16_t first_letter;   /* +0x0E V  letters before this run (compared with D012Text.shown) */
    uint32_t next_addr;      /* +0x10 V  D012TextBuffer.runs list */
    uint32_t prev_addr;      /* +0x14 V */
} D012TextRun;
D012_STATIC_ASSERT(sizeof(D012TextRun) == 0x18, "D012TextRun size (pool stride)");
D012_TEXT_AT(D012TextRun, colour, 0x04);
D012_TEXT_AT(D012TextRun, size, 0x08);
D012_TEXT_AT(D012TextRun, row_top, 0x0C);
D012_TEXT_AT(D012TextRun, flags, 0x0D);
D012_TEXT_AT(D012TextRun, first_letter, 0x0E);
D012_TEXT_AT(D012TextRun, next_addr, 0x10);
D012_TEXT_AT(D012TextRun, prev_addr, 0x14);

/* An inline image (button glyph, item icon) of a text (pool item, 0x10), made by 0x0887E114 and drawn by
 * 0x0887CA50 from an image set, not from the text buffer. Review 2: one node per 0x1E, per 0x15 (columns x rows
 * cells) and per icon of codes 0x18 / 0x1A / 0x1D (set 0, 1 x 1); linked at the tail of D012TextBuffer.images by
 * 0x0887F294's pass 2 (0x0887E114 itself links nothing, and returns an unfilled node when the set has no image). */
typedef struct D012TextImage {
    uint32_t next_addr;      /* +0x00 V */
    uint32_t prev_addr;      /* +0x04 V */
    int16_t  x;              /* +0x08 V  x in the buffer row (px): pass 2's pen, rounded up, at the image */
    uint8_t  u;              /* +0x0A V  ((id - 1) % per_row) * cell */
    uint8_t  v;              /* +0x0B V  ((id - 1) / per_row) * cell */
    uint8_t  cells;          /* +0x0C V  columns << 4 | rows (quad = cell * columns x cell * rows); code 0x15 a:
                              *          columns ((a >> 8) & 15) - 1, rows ((a >> 12) & 15) - 1 (review 2); rows 0
                              *          (id < 1) = not drawn */
    uint8_t  line_set;       /* +0x0D V  line (bits 0-5: pass 2's count of 0x0A/0x12/0x16) | image set << 6 */
    uint16_t pad_0E;         /* +0x0E I */
} D012TextImage;
D012_STATIC_ASSERT(sizeof(D012TextImage) == 0x10, "D012TextImage size (pool stride)");
D012_TEXT_AT(D012TextImage, x, 0x08);
D012_TEXT_AT(D012TextImage, u, 0x0A);
D012_TEXT_AT(D012TextImage, v, 0x0B);
D012_TEXT_AT(D012TextImage, cells, 0x0C);
D012_TEXT_AT(D012TextImage, line_set, 0x0D);

/* Head of a doubly linked list {head, tail, count}. */
typedef struct D012TextList {
    uint32_t head_addr;      /* +0x00 V */
    uint32_t tail_addr;      /* +0x04 V */
    int32_t  count;          /* +0x08 V */
} D012TextList;
D012_STATIC_ASSERT(sizeof(D012TextList) == 0x0C, "D012TextList size");

/* A whole-string buffer record (pool item, 0x40): the 4-bit picture of one text and how to cut it into quads.
 * Made by 0x0887F294; shared by clones (refcount); freed by the Update pass (0x0887DBD4) two passes after the
 * last release. */
typedef struct D012TextBuffer {
    uint16_t width;          /* +0x00 V R  pixels per row = sceGuTexImage tbw: (widest line + 31) & ~31, max 1024 */
    uint16_t tex_height;     /* +0x02 V  power of two >= rows (sceGuTexImage height) */
    uint32_t block_addr;     /* +0x04 V  heap block from the heap at manager+0x14 ("SYSTEM FONT T" or a screen's
                              *          own; corrected (review): not "SYSTEM FONT"), 0x0887D7B8; freed by 0x0887D828 */
    uint32_t pixels_addr;    /* +0x08 V R  4-bit pixels, low nibble first; = block_addr */
    uint16_t letters;        /* +0x0C V  letters in the text (0x0887B7E8 copies it to D012Text.shown) */
    uint16_t text_width;     /* +0x0E V  widest line (px) */
    int16_t  width_limit;    /* +0x10 V  10 + sum of trunc(px * size) of the runs closed before code 0x12/0x16
                              *          (all lines); 0x0887BB6C's width, 0x0887BC2C's cut-off */
    uint8_t  run_count;      /* +0x12 V  1 + runs (0x0887BC2C reads it SIGNED (lb) and allocates 64 bytes per run) */
    uint8_t  line_count;     /* +0x13 V  1 + line breaks */
    uint32_t on_free_addr;   /* +0x14 V  called with on_free_arg when the buffer is freed (0 here) */
    uint32_t on_free_arg;    /* +0x18 V */
    D012TextList runs;       /* +0x1C V  D012TextRun */
    int32_t  refcount;       /* +0x28 V  texts using it; 0 -> -1 -> freed by the next Update passes (0x0887A544) */
    D012TextList images;     /* +0x2C V  D012TextImage */
    uint32_t next_addr;      /* +0x38 V  manager's buffer list */
    uint32_t prev_addr;      /* +0x3C V */
} D012TextBuffer;
D012_STATIC_ASSERT(sizeof(D012TextBuffer) == 0x40, "D012TextBuffer size (pool stride)");
D012_TEXT_AT(D012TextBuffer, tex_height, 0x02);
D012_TEXT_AT(D012TextBuffer, block_addr, 0x04);
D012_TEXT_AT(D012TextBuffer, pixels_addr, 0x08);
D012_TEXT_AT(D012TextBuffer, letters, 0x0C);
D012_TEXT_AT(D012TextBuffer, text_width, 0x0E);
D012_TEXT_AT(D012TextBuffer, width_limit, 0x10);
D012_TEXT_AT(D012TextBuffer, run_count, 0x12);
D012_TEXT_AT(D012TextBuffer, line_count, 0x13);
D012_TEXT_AT(D012TextBuffer, on_free_addr, 0x14);
D012_TEXT_AT(D012TextBuffer, on_free_arg, 0x18);
D012_TEXT_AT(D012TextBuffer, runs, 0x1C);
D012_TEXT_AT(D012TextBuffer, refcount, 0x28);
D012_TEXT_AT(D012TextBuffer, images, 0x2C);
D012_TEXT_AT(D012TextBuffer, next_addr, 0x38);
D012_TEXT_AT(D012TextBuffer, prev_addr, 0x3C);

/* An image set for inline images (manager+0x30 and +0x38). */
typedef struct D012TextImageSet {
    uint32_t image_addr;     /* +0x00 V  image bound by 0x0899D2F8 (0x0887E090: set 0 = texture 0x1804 of group 7) */
    uint8_t  unk_04;         /* +0x04 V  0 at init */
    uint8_t  cell;           /* +0x05 V  18: cell size and placeholder width (px) */
    uint16_t image_width;    /* +0x06 V  128 (set 0), 256 (set 1): cells per row = image_width / cell */
} D012TextImageSet;
D012_STATIC_ASSERT(sizeof(D012TextImageSet) == 0x08, "D012TextImageSet size (stride of the two sets)");

/* Glyph metrics cached for the first 256 characters of a string (manager+0x2C, 0x800 bytes). */
typedef struct D012TextCharCache {
    int16_t  left;           /* +0x00 V  sceFontCharInfo.bitmapLeft (-1: missing) */
    int16_t  top;            /* +0x02 V  bitmapTop */
    int32_t  advance;        /* +0x04 V  26.6 advance used (after the U+201C / Cyrillic rules) */
} D012TextCharCache;
D012_STATIC_ASSERT(sizeof(D012TextCharCache) == 0x08, "D012TextCharCache size (index * 8)");

/* SceFontInfo as filled by sceFontGetFontInfo at manager+0x60 (0x108 bytes). */
typedef struct D012FontInfo {
    int32_t  max_glyph_width_i;     /* +0x00 D V  26.6; >> 6 -> manager.max_glyph_w */
    int32_t  max_glyph_height_i;    /* +0x04 D V  -> manager.max_glyph_h */
    int32_t  max_glyph_ascender_i;  /* +0x08 D V  OVERWRITTEN with 0x480 (18 px) by 0x08881954: the baseline */
    int32_t  max_glyph_descender_i; /* +0x0C D V  line height = ((0x480 - this) >> 6) + 3 */
    int32_t  unk_10[6];             /* +0x10 D  left X, base Y, centre X, top Y, advance X, advance Y */
    float    floats[10];            /* +0x28 D  the same as floats */
    uint16_t max_bitmap_width;      /* +0x50 D */
    uint16_t max_bitmap_height;     /* +0x52 D */
    uint32_t num_glyphs;            /* +0x54 D */
    uint32_t shadow_map_length;     /* +0x58 D */
    uint8_t  font_style[0xA8];      /* +0x5C D  SceFontStyle */
    uint8_t  bpp;                   /* +0x104 D V  bits per pixel; 8 / bpp -> manager.pixels_per_byte */
    uint8_t  pad_105[3];            /* +0x105 D */
} D012FontInfo;
D012_STATIC_ASSERT(sizeof(D012FontInfo) == 0x108, "D012FontInfo size (SceFontInfo)");
D012_TEXT_AT(D012FontInfo, bpp, 0x104);

/* The bitmap font of control-code 0x1C strings (0x39C4 bytes, manager+0x28 / +0xF2C0, built by 0x0887D574).
 * Its head and width table are copied by 0x0887D5C8 from a package resource whose first byte is 'F'
 * (0x088D2980); the image by 0x0887D66C. Used by the battle banners and tips in the fighters' moveset archives
 * (R: one 14-px-high glyph strip per character from a 512x256 CLUT4 image in a traced fight). Strings are bytes:
 * glyph c (0x22..0x7F) or c b (c >= 0x80: index b * 128 + c - 0xA2), 0x20 / 0x21 spaces of 4 / 8 px, control codes
 * as two bytes plus D012_TEXT_ARG_BYTES[code] bytes (V 0x0887E9CC). */
typedef struct D012BitmapGlyph {
    int16_t  x;              /* +0x00 V  pen x in the text */
    uint16_t u;              /* +0x02 V */
    uint8_t  v;              /* +0x04 V */
    uint8_t  size;           /* +0x05 V  width | height << 4 (pixels, <= 15) */
    uint16_t pad_06;         /* +0x06 I */
    uint32_t colour;         /* +0x08 V  ABGR from code 0x10, 0 = the object's */
} D012BitmapGlyph;
D012_STATIC_ASSERT(sizeof(D012BitmapGlyph) == 0x0C, "D012BitmapGlyph size (0x0887E9CC stride)");

typedef struct D012BitmapText {
    uint8_t  flags;          /* +0x00 V  bit 0: in use */
    uint8_t  glyph_count;    /* +0x01 V  at most 40 */
    uint8_t  refcount;       /* +0x02 V  0x0887D67C ++, 0x0887D68C -- (frees at 0) */
    uint8_t  pad_03;         /* +0x03 I */
    int16_t  width;          /* +0x04 V  pixels */
    uint16_t pad_06;         /* +0x06 I */
    D012BitmapGlyph glyphs[40]; /* +0x08 V */
    D012TextList images;     /* +0x1E8 V  D012TextImage */
} D012BitmapText;
D012_STATIC_ASSERT(sizeof(D012BitmapText) == 0x1F4, "D012BitmapText size (stride 500)");
D012_TEXT_AT(D012BitmapText, glyph_count, 0x01);
D012_TEXT_AT(D012BitmapText, refcount, 0x02);
D012_TEXT_AT(D012BitmapText, width, 0x04);
D012_TEXT_AT(D012BitmapText, glyphs, 0x08);
D012_TEXT_AT(D012BitmapText, images, 0x1E8);

typedef struct D012BitmapFont {
    uint8_t  unk_00[8];      /* +0x00 I  head bytes not read by the text code */
    uint8_t  cell_w_a;       /* +0x08 V  section A: glyph i at u = i * cell_w_a, v = 0 */
    uint8_t  cell_h_a;       /* +0x09 V  section A height; section B starts at v = cell_h_a */
    uint8_t  cell_w_b;       /* +0x0A V  section B: a grid of cols_b columns */
    uint8_t  cell_h_b;       /* +0x0B V */
    uint8_t  count_a;        /* +0x0C V  glyphs in section A */
    uint8_t  cols_b;         /* +0x0D V */
    uint8_t  unk_0E[2];      /* +0x0E I */
    uint8_t  widths[0x300];  /* +0x10 V  per glyph index (code - 0x22) */
    uint32_t image_addr;     /* +0x310 V  image bound by 0x0887EF84 */
    D012BitmapText texts[28];/* +0x314 V */
} D012BitmapFont;
D012_STATIC_ASSERT(sizeof(D012BitmapFont) == 0x39C4, "D012BitmapFont size (allocation in 0x08881954)");
D012_TEXT_AT(D012BitmapFont, cell_w_a, 0x08);
D012_TEXT_AT(D012BitmapFont, cols_b, 0x0D);
D012_TEXT_AT(D012BitmapFont, widths, 0x10);
D012_TEXT_AT(D012BitmapFont, image_addr, 0x310);
D012_TEXT_AT(D012BitmapFont, texts, 0x314);

/* One entry of a screen's label-to-layout-node binding table (8 bytes, ended by node == 0xFFFF). Walked by
 * 0x088D428C (attach: 0x088D40BC sets D012_TEXT_MANUAL and links the label into the node's text list at
 * node+0x30/+0x34/+0x38 through D012Text.node_next_addr / node_prev_addr), 0x088D44EC (each frame: the part's
 * matrix and colour into the label) and 0x088D442C (release). E.g. the shop's stat numbers: menu table 0x08B00726. */
enum {
    D012_BIND_LEFT   = 0x01, /* V  0x0887B970 */
    D012_BIND_CENTER = 0x02, /* V  0x0887B98C */
    D012_BIND_RIGHT  = 0x04, /* V  0x0887B958 */
    D012_BIND_TOP    = 0x08, /* V  0x0887B9A4 */
    D012_BIND_MIDDLE = 0x10, /* V  0x0887B9C4 */
    D012_BIND_BOTTOM = 0x20  /* V  0x0887B9E4 */
};
typedef struct D012TextBinding {
    uint16_t label;          /* +0x00 V  index into the screen's label (D012Text*) array */
    uint16_t align;          /* +0x02 V  D012_BIND_* (attach only) */
    uint16_t node;           /* +0x04 V  index into the layout's node array; 0xFFFF ends the table */
    uint16_t part;           /* +0x06 V  part of the node whose matrix and colour the label follows */
} D012TextBinding;
D012_STATIC_ASSERT(sizeof(D012TextBinding) == 0x08, "D012TextBinding size (0x088D428C / 0x088D44EC stride)");
D012_TEXT_AT(D012TextBinding, align, 0x02);
D012_TEXT_AT(D012TextBinding, node, 0x04);
D012_TEXT_AT(D012TextBinding, part, 0x06);

/* The manager, "SYSTEM_FONT" (0xF2C4 bytes, static at 0x08BEE94C; the next static's guard is at +0xF2C4). */
typedef struct D012TextManager {
    uint8_t  line_height;    /* +0x00 V R  ((0x480 - descender) >> 6) + 3: 26 with the port's font */
    uint8_t  max_glyph_h;    /* +0x01 V  font info max height >> 6 */
    uint8_t  max_glyph_w;    /* +0x02 V  font info max width >> 6 */
    uint8_t  icon_20;        /* +0x03 V  20: trailing icon id of an item text (code 0x18) when
                              *          0x088C7FE4(item)->+0x04 & 2, else 21. Corrected (review 2): not the kind */
    int16_t  pixels_per_byte;/* +0x04 V  8 / bpp = 2 */
    uint16_t pad_06;         /* +0x06 I */
    uint32_t font_mem_addr;  /* +0x08 V  0x2EC00-byte block (align 0x80) */
    uint32_t font_heap;      /* +0x0C V  heap "SYSTEM FONT" over it: font library memory (0x0887D6E0), the bitmap
                              *          font, the metrics cache (corrected (review): not the text buffers) */
    uint32_t tex_mem_addr;   /* +0x10 V  0x19000-byte block (align 0x80); 0 while 0x0887E798 has a heap in */
    uint32_t tex_heap;       /* +0x14 V  heap "SYSTEM FONT T": the text buffers (0x0887D7B8 allocates here;
                              *          corrected (review)); screens swap in T2 (0x0887E630) or their own (0x0887E798).
                              *          The font init also borrows it for the font list (review 2) */
    uint32_t tex_mem_saved_addr; /* +0x18 V  +0x10/+0x14 kept here while 0x0887E630 swaps in "SYSTEM FONT T2" */
    uint32_t tex_heap_saved;     /* +0x1C V  (zeroed by the font init) */
    uint32_t tex_mem_saved2_addr;/* +0x20 V  +0x10/+0x14 saved by 0x0887E798(mgr, heap), restored and cleared by
                                  *          0x0887E7B4 (review: checked in the assembly) */
    uint32_t tex_heap_saved2;    /* +0x24 V */
    uint32_t bitmap_font_mem_addr; /* +0x28 V  the 0x39C4-byte D012BitmapFont block */
    uint32_t char_cache_addr;      /* +0x2C V  D012TextCharCache[256] (0x800 bytes) */
    D012TextImageSet image_sets[2];/* +0x30 V */
    uint32_t libfont_module; /* +0x40 V  "libfont.prx" */
    uint32_t font_lib;       /* +0x44 V R  sceFontNewLib handle */
    int32_t  font_error;     /* +0x48 V  last sceFont error code */
    uint32_t fonts[5];       /* +0x4C V R  sceFontOpen handles; [0] is the one text is drawn with (and the only one
                              *            opened: 0x08881954's open loop runs once; review 2) */
    D012FontInfo font_info;  /* +0x60 V R  sceFontGetFontInfo (0x08BEE9AC traced) */
    D012TextList texts;      /* +0x168 V  active D012Text (next +0x48, prev +0x50), in draw order */
    D012TextList buffers;    /* +0x174 V  D012TextBuffer (next +0x38, prev +0x3C) */
    D012TextPoolNode text_nodes[0x122];   /* +0x180 V */
    D012Text texts_pool[0x122];           /* +0xA90 V */
    uint32_t text_take_addr; /* +0x6E40 V */
    uint32_t text_give_addr; /* +0x6E44 V */
    D012TextPoolNode buffer_nodes[0xCD];  /* +0x6E48 V */
    D012TextBuffer buffers_pool[0xCD];    /* +0x74B0 V */
    uint32_t buffer_take_addr; /* +0xA7F0 V */
    uint32_t buffer_give_addr; /* +0xA7F4 V */
    D012TextPoolNode run_nodes[0x1D6];    /* +0xA7F8 V */
    D012TextRun runs_pool[0x1D6];         /* +0xB6A8 V */
    uint32_t run_take_addr;  /* +0xE2B8 V */
    uint32_t run_give_addr;  /* +0xE2BC V */
    D012TextPoolNode image_nodes[0xAA];   /* +0xE2C0 V */
    D012TextImage images_pool[0xAA];      /* +0xE810 V */
    uint32_t image_take_addr;/* +0xF2B0 V */
    uint32_t image_give_addr;/* +0xF2B4 V */
    uint32_t update_task;    /* +0xF2B8 V  task "SYSTEM_FONT::Update" (0x0887E4F4, priority 0x88B9) */
    uint32_t draw_task;      /* +0xF2BC V  task "SYSTEM_FONT::Draw" (0x0887E514, priority 0x11196) */
    uint32_t bitmap_font_addr; /* +0xF2C0 V  D012BitmapFont* (= bitmap_font_mem_addr once built) */
} D012TextManager;
D012_STATIC_ASSERT(sizeof(D012TextManager) == 0xF2C4, "D012TextManager size (static layout)");
D012_TEXT_AT(D012TextManager, pixels_per_byte, 0x04);
D012_TEXT_AT(D012TextManager, font_mem_addr, 0x08);
D012_TEXT_AT(D012TextManager, font_heap, 0x0C);
D012_TEXT_AT(D012TextManager, tex_mem_addr, 0x10);
D012_TEXT_AT(D012TextManager, tex_heap, 0x14);
D012_TEXT_AT(D012TextManager, tex_mem_saved_addr, 0x18);
D012_TEXT_AT(D012TextManager, tex_mem_saved2_addr, 0x20);
D012_TEXT_AT(D012TextManager, bitmap_font_mem_addr, 0x28);
D012_TEXT_AT(D012TextManager, char_cache_addr, 0x2C);
D012_TEXT_AT(D012TextManager, image_sets, 0x30);
D012_TEXT_AT(D012TextManager, libfont_module, 0x40);
D012_TEXT_AT(D012TextManager, font_lib, 0x44);
D012_TEXT_AT(D012TextManager, font_error, 0x48);
D012_TEXT_AT(D012TextManager, fonts, 0x4C);
D012_TEXT_AT(D012TextManager, font_info, 0x60);
D012_TEXT_AT(D012TextManager, texts, 0x168);
D012_TEXT_AT(D012TextManager, buffers, 0x174);
D012_TEXT_AT(D012TextManager, text_nodes, 0x180);
D012_TEXT_AT(D012TextManager, texts_pool, 0xA90);
D012_TEXT_AT(D012TextManager, text_take_addr, 0x6E40);
D012_TEXT_AT(D012TextManager, text_give_addr, 0x6E44);
D012_TEXT_AT(D012TextManager, buffer_nodes, 0x6E48);
D012_TEXT_AT(D012TextManager, buffers_pool, 0x74B0);
D012_TEXT_AT(D012TextManager, buffer_take_addr, 0xA7F0);
D012_TEXT_AT(D012TextManager, buffer_give_addr, 0xA7F4);
D012_TEXT_AT(D012TextManager, run_nodes, 0xA7F8);
D012_TEXT_AT(D012TextManager, runs_pool, 0xB6A8);
D012_TEXT_AT(D012TextManager, run_take_addr, 0xE2B8);
D012_TEXT_AT(D012TextManager, run_give_addr, 0xE2BC);
D012_TEXT_AT(D012TextManager, image_nodes, 0xE2C0);
D012_TEXT_AT(D012TextManager, images_pool, 0xE810);
D012_TEXT_AT(D012TextManager, image_take_addr, 0xF2B0);
D012_TEXT_AT(D012TextManager, image_give_addr, 0xF2B4);
D012_TEXT_AT(D012TextManager, update_task, 0xF2B8);
D012_TEXT_AT(D012TextManager, draw_task, 0xF2BC);
D012_TEXT_AT(D012TextManager, bitmap_font_addr, 0xF2C0);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* D012_TEXT_TYPES_H */
