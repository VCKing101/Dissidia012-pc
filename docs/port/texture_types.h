/*
 * texture_types.h - Dissidia 012 (USA ULUS10566) texture-path data layouts.
 *
 * Established by reading the retail disassembly (EBOOT main, OVL_MENU/OVL_BATTLE) and checked in the
 * running PC port on 2026-11-21. See texture-pipeline.md next to this file for the functions that read and
 * write each structure.
 *
 * Marks in the field comments:
 *   V  verified in the disassembly (a load/store at that offset with that width, used that way)
 *   R  seen at run time in the port (the value in guest memory, or the store that wrote it)
 *   I  inferred (name/meaning from how it is used, or offset known but use not fully traced)
 *   D  from docs/port/texture-loading.md or the GIM format documentation, not re-verified here
 *
 * Conventions:
 *   - All structures are the game's guest memory (PSP, 32-bit little endian). Every guest pointer is a
 *     uint32_t guest address (suffix _addr), so the layouts are the same in a 64-bit host build.
 *   - Unknown fields are unk_XX (XX = hex offset). Padding is explicit.
 *   - Sizes are asserted only where the size is known from an allocation, a stride or a copy.
 */
#ifndef D012_TEXTURE_TYPES_H
#define D012_TEXTURE_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
#define D012_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#else
#define D012_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------------------------------
 * 1. Package files
 * ---------------------------------------------------------------------------------------------- */

/* One entry of the load list passed to FUN_088D00FC / FUN_088D2980 (t0). Built on the caller's
 * stack or in rodata; ends with kind == 0xFFFF. */
typedef struct D012PkgListEntry {
    uint16_t kind;        /* +0x00 V  0 = GIM pack (textures), 1 = seq pack (layout objects), 0xFFFF = end */
    uint16_t pad_02;      /* +0x02 V  not read */
    uint32_t path_addr;   /* +0x04 V  char* package path, e.g. "menu/menu_pk_party_battle_title_gim_0.bin";
                           *          localised by FUN_08867984 (ZQ / JP -> language code) before lookup */
} D012PkgListEntry;
D012_STATIC_ASSERT(sizeof(D012PkgListEntry) == 0x08, "D012PkgListEntry size");

/* Sub-file table entry of a pack file (*_gim_N.bin, *_seq_N.bin). */
typedef struct D012PackEntry {
    uint32_t offset;      /* +0x00 V  from the start of the pack */
    uint32_t size;        /* +0x04 V  0 = empty slot (skipped, but its id is still consumed) */
} D012PackEntry;
D012_STATIC_ASSERT(sizeof(D012PackEntry) == 0x08, "D012PackEntry size");

/* Header of a pack file as FUN_088D2980 reads it. */
typedef struct D012PackHeader {
    uint8_t  unk_00[0x0A];   /* +0x00 I  not read by the loader (magic/version?) */
    uint16_t count;          /* +0x0A V  number of sub-file entries */
    uint16_t base_id;        /* +0x0C V  texture id of entry 0; entry i gets id base_id + i */
    uint16_t unk_0E;         /* +0x0E I  not read */
    /* D012PackEntry entries[count];   +0x10 V
     *   kind 0: every entry is one sub-file (first byte 'M' = GIM, 'F' = 0x310-byte table, else ignored)
     *   kind 1: entries are taken in pairs (16 bytes); ids step by 2 */
} D012PackHeader;
D012_STATIC_ASSERT(sizeof(D012PackHeader) == 0x10, "D012PackHeader size");

/* Language container a sub-file may be wrapped in (FUN_08870B10). */
typedef struct D012MlngHeader {
    char     magic[4];       /* +0x00 V  "mlng" */
    uint32_t count;          /* +0x04 V  number of languages */
    /* uint32_t offset[count];  +0x08 V  from the start of this header, indexed by FUN_08867888() */
    /* uint32_t size[count];    +0x08 + 4*count V */
} D012MlngHeader;
D012_STATIC_ASSERT(sizeof(D012MlngHeader) == 0x08, "D012MlngHeader size");

/* Header of a block of the game heap (FUN_0885C6CC), 16 bytes before the block's data. The GRAPHICS and
 * EDRAM heaps use it too, so EDRAM blocks have their header inside VRAM. Seen at run time: the portrait's
 * image copy {next 0x092B4F40 -> 0x08E52030 once the next block was taken, prev 0x08E41F40, 0x10050, 0x10};
 * a VRAM block {0x04200000, 0x0414F200, 0x2140, 0x40}. */
typedef struct D012HeapBlockHeader {
    uint32_t next_addr;      /* +0x00 R  next block (I: the free remainder, or the heap end, while last) */
    uint32_t prev_addr;      /* +0x04 R  previous block */
    uint32_t size;           /* +0x08 R  block size including this header */
    uint8_t  align;          /* +0x0C R  alignment asked for (0x10, 0x40) (I) */
    uint8_t  pad_0D[3];      /* +0x0D I  not written by the allocation */
} D012HeapBlockHeader;
D012_STATIC_ASSERT(sizeof(D012HeapBlockHeader) == 0x10, "D012HeapBlockHeader size");

/* ------------------------------------------------------------------------------------------------
 * 2. Texture manager (*(0x089D7830) == 0x08B87E7C, built by FUN_088D11C8)
 * ---------------------------------------------------------------------------------------------- */

/* A loaded 2D texture: what FUN_088DE0E8 fills. 270 of them in a pool at manager+0xF444. */
typedef struct D012TexEntry {
    uint32_t image_addr;     /* +0x00 V  D012ImageObject* from the image library (0 when free) */
    uint16_t refcount;       /* +0x04 V  1 after load; FUN_088DE198 ++, FUN_088DE1A8 -- (destroys at 0) */
    uint16_t unk_06;         /* +0x06 V  a3 of FUN_088DE0E8; 0xFFFF from every package/manager caller */
    uint16_t id;             /* +0x08 V  texture id (pack base_id + index, or caller's id); 0xFFFF when freed */
    uint8_t  group;          /* +0x0A V  owner group (e.g. 0x23 for the party-battle title screen) */
    uint8_t  pad_0B;         /* +0x0B I */
    uint32_t next_addr;      /* +0x0C V  manager list (head manager+0xEBC8, tail +0xEBCC, count +0xEBD0) */
    uint32_t prev_addr;      /* +0x10 V */
} D012TexEntry;
D012_STATIC_ASSERT(sizeof(D012TexEntry) == 0x14, "D012TexEntry size");

/* Slot of the manager's object stock rings (take/return pointers walk these). */
typedef struct D012StockSlot {
    uint32_t object_addr;    /* +0x00 V  object, 0 while taken */
    uint32_t next_addr;      /* +0x04 V  next slot (ring) */
} D012StockSlot;
D012_STATIC_ASSERT(sizeof(D012StockSlot) == 0x08, "D012StockSlot size");

/* Offsets inside the texture manager that the texture path uses (the object is 0x109xx bytes). */
enum D012TexManagerOffsets {
    D012_TEXMGR_HEAP_A            = 0x00004, /* V heap used by FUN_088DAFC8 when +0x08 is 0 */
    D012_TEXMGR_HEAP_B            = 0x00008, /* V preferred staging heap of FUN_088DAFC8 */
    D012_TEXMGR_USE_FILE_CACHE    = 0x00023, /* V u8: 1 -> FUN_088FE7C8 / FUN_088FE708 instead of 08862C40 / 08862854 */
    D012_TEXMGR_SPRITE_TAKE       = 0x0336C, /* V sprite-instance (0x2C bytes, 252) stock: take pointer */
    D012_TEXMGR_SPRITE_RETURN     = 0x03370, /* V */
    D012_TEXMGR_SEQ_LIST_HEAD     = 0x0C4F4, /* V seq objects (0x74 bytes, 80): next +0x6C, prev +0x70 */
    D012_TEXMGR_SEQ_LIST_TAIL     = 0x0C4F8, /* V */
    D012_TEXMGR_SEQ_COUNT         = 0x0C4FC, /* V */
    D012_TEXMGR_SEQ_TAKE          = 0x0EBC0, /* V */
    D012_TEXMGR_SEQ_RETURN        = 0x0EBC4, /* V */
    D012_TEXMGR_TEX_LIST_HEAD     = 0x0EBC8, /* V D012TexEntry list */
    D012_TEXMGR_TEX_LIST_TAIL     = 0x0EBCC, /* V */
    D012_TEXMGR_TEX_COUNT         = 0x0EBD0, /* V */
    D012_TEXMGR_TEX_SLOTS         = 0x0EBD4, /* V D012StockSlot[270] (ring) */
    D012_TEXMGR_TEX_POOL          = 0x0F444, /* V D012TexEntry[270] */
    D012_TEXMGR_TEX_TAKE          = 0x1095C, /* V the doc's "0xEBD4 + 0x1D88" */
    D012_TEXMGR_TEX_RETURN        = 0x10960, /* V */
    D012_TEXMGR_TEX_ALLOC_CB      = 0x10968, /* V optional: D012TexEntry *(*)(void), replaces the take */
    D012_TEXMGR_TEX_FREE_CB       = 0x1096C  /* V optional: void (*)(D012TexEntry *), replaces the return */
};

/* ------------------------------------------------------------------------------------------------
 * 3. Image library (Sony, main 0x0899B6B0-0x0899F3FC)
 * ---------------------------------------------------------------------------------------------- */

/* GIM file block header (all GIM blocks). */
typedef struct D012GimBlockHeader {
    uint16_t type;           /* +0x00 V  2 root, 3 picture, 4 image, 5 palette, 6 sequence (D: 0xFF file info) */
    uint16_t unk_02;         /* +0x02 D */
    uint32_t size;           /* +0x04 V  whole block incl. children (end = block + size) */
    uint32_t child_offset;   /* +0x08 V  first child, from this header */
    uint32_t data_offset;    /* +0x0C V  this block's data, from this header (0x10 in practice) */
} D012GimBlockHeader;
D012_STATIC_ASSERT(sizeof(D012GimBlockHeader) == 0x10, "D012GimBlockHeader size");

/* Data of a GIM image (type 4) or palette (type 5) block. The image library COPIES this
 * (child_offset - data_offset bytes) into its own allocation and makes the level/frame table absolute.
 * The same layout is synthesised for TIM2/BMP/raw images by FUN_0899EB28 / FUN_0899EE30. */
typedef struct D012GimImageData {
    uint16_t header_size;    /* +0x00 D  0x30 */
    uint16_t refcount;       /* +0x02 V R  set to 1 by the copy (FUN_0899BC64), decremented by FUN_0899BEE0 */
    uint16_t format;         /* +0x04 V  GE texture format (TPF) for images; GE CLUT format (CMODE low bits) for
                              *          palettes; 3 = RGBA8888 doubles the CLUT load */
    uint16_t order;          /* +0x06 V  TMODE swizzle bit (0 normal, 1 swizzled) */
    uint16_t width;          /* +0x08 V  <= 512 (palette: entry count, <= 256) */
    uint16_t height;         /* +0x0A V  <= 512 */
    uint16_t bpp;            /* +0x0C V  bits per pixel */
    uint16_t pitch_align;    /* +0x0E V  row alignment in bytes: buffer width = align_up(width, pitch_align*8/bpp) */
    uint16_t height_align;   /* +0x10 D */
    uint16_t unk_12;         /* +0x12 D  dim count */
    uint16_t unk_14;         /* +0x14 D */
    uint16_t unk_16;         /* +0x16 D */
    uint32_t table_offset;   /* +0x18 V  offset (from this struct) of the level/frame pointer table; 0x30 when
                              *          synthesised and in the package GIMs seen at run time (R). Entries start as
                              *          offsets (0x40) and are made guest addresses (copy + 0x40) by the load (R) */
    uint32_t pixels_start;   /* +0x1C D  0x40 in practice */
    uint32_t data_size;      /* +0x20 V  used as the copy size for VRAM upload (FUN_0899C52C) */
    uint32_t plane_mask;     /* +0x24 V  image: bit mask used by FUN_0899C2C4 to derive CLUT shift/mask */
    uint16_t level_type;     /* +0x28 V  1 = mip levels halve in size */
    uint16_t level_count;    /* +0x2A V  mip levels (TMODE max level = count-1) */
    uint16_t frame_type;     /* +0x2C D */
    uint16_t frame_count;    /* +0x2E V  animation frames */
    /* uint32_t level_addr[frame_count][level_count];  at +table_offset (V) */
} D012GimImageData;
D012_STATIC_ASSERT(sizeof(D012GimImageData) == 0x30, "D012GimImageData header size");

/* The image object FUN_0899B9F0 allocates (0x90 bytes, class-0 memory = GRAPHICS heap).
 * R: at the bind of a 256x256 8-bit portrait the first 32 bytes read
 * 00000001 00000000 00000000 08E41FF0 08E52040 00000000 00000000 00200100, and every initial value below
 * marked "at create" was seen stored by FUN_0899B888. */
typedef struct D012ImageObject {
    uint32_t refcount;       /* +0x00 V R  1 at create; FUN_0899BB1C destroys at 0 */
    uint32_t flags;          /* +0x04 V R  set by FUN_0899BFE0: 1 image mips, 2 image frames, 4 palette levels,
                              *          8 palette frames, 0x10 animated (no fast path), 0x20|0x40 non-power-of-two
                              *          (UV scale in +0x28/+0x2C). 0 = the simple path in FUN_0899C944 */
    uint32_t draw_flags;     /* +0x08 V  0 at create. 1 = bind two frames/palettes (TBP0+TBP1);
                              *          2 = TEXLEVEL (0xC8, bit 0 adds the +0x40 bias); 4 = UV scale/offset
                              *          (0x48-0x4B, float bits >> 8; default block 0x089D13D0 = {0, 0, 1, 1});
                              *          0x100 = ALPHA blend (0xDF-0xE1) from table 0x089D13E0[+0x70], entries
                              *          {op, src, dst, FIXB}: 0 fixed, 1 normal, 2 additive, 3 reverse subtract;
                              *          0x200 = TFUNC (0xC9); 0x400 = TFILTER (0xC6) = +0x7C << 8 | +0x80;
                              *          0x800 = TWRAP (0xC7) = +0x88 << 8 | +0x84 */
    uint32_t image_addr;     /* +0x0C V R  D012GimImageData* (copy in the GRAPHICS heap) - PIXELS ARE HERE
                              *            (at image_addr + 0x40 for the package GIMs seen) */
    uint32_t palette_addr;   /* +0x10 V R  D012GimImageData* of the palette, or 0 */
    uint32_t list_addr;      /* +0x14 V  precompiled GE texture list (class 1) made by FUN_0899C52C, or 0 */
    uint32_t vram_addr;      /* +0x18 V  class-2 (EDRAM) block {image_copy, palette_copy} made by
                              *          FUN_0899C52C; when set, FUN_0899C944 binds these instead of +0x0C/+0x10 */
    int16_t  buffer_width;   /* +0x1C V R  TBW for level 0 (256 for a 256-wide 8-bit image); read signed (lh) */
    int16_t  clut_blocks;    /* +0x1E V R  read signed (lh). CLOAD count: (palette width / 16) * levels, x2 for RGBA8888, max 32
                              *            (32 for 256 RGBA8888 entries) */
    float    u_offset;       /* +0x20 V  0x4A UOFFSET when draw_flags & 4 and flags & 0x20 */
    float    v_offset;       /* +0x24 V  0x4B */
    float    u_scale;        /* +0x28 V  1.0, or width / pow2(width) */
    float    v_scale;        /* +0x2C V  1.0, or height / pow2(height) */
    float    unk_30;         /* +0x30 V  -1000000.0 at create (I: frame time limits) */
    float    unk_34;         /* +0x34 V  1000000.0 at create */
    float    unk_38;         /* +0x38 V  12.0 at create */
    uint32_t unk_3C;         /* +0x3C V  0 at create */
    float    mip_bias;       /* +0x40 V  TEXLEVEL (0xC8) bias * const when draw_flags & 3 == 3 */
    int32_t  image_frame;    /* +0x44 V  current image frame (and +0x48 next frame for blending) */
    int32_t  image_frame2;   /* +0x48 V */
    int32_t  palette_frame;  /* +0x4C V  current palette frame */
    int32_t  palette_frame2; /* +0x50 V */
    uint32_t seq_list_addr;  /* +0x54 V  array of sequence (type 6) block copies */
    int32_t  seq_count;      /* +0x58 V */
    int32_t  unk_5C;         /* +0x5C V  -1 at create */
    int32_t  clut_param;     /* +0x60 V  input of FUN_0899C2C4 (I: palette bank index) */
    uint32_t clut_shift;     /* +0x64 V  CMODE shift  (<< 2) */
    uint32_t clut_mask;      /* +0x68 V  CMODE mask   (<< 8), 0xFF at create */
    uint32_t clut_start;     /* +0x6C V  CMODE start  (<< 16) */
    uint32_t blend_index;    /* +0x70 V  1 at create (I: index into the blend table at 0x089D13E0) */
    uint32_t tfunc_func;     /* +0x74 V  0 at create (I: TFUNC 0xC9 low bits) */
    uint32_t tfunc_alpha;    /* +0x78 V  1 at create (I: TFUNC bits 8..) */
    uint32_t mag_filter;     /* +0x7C V  TFILTER mag (0xC6 bits 8..), 1 (linear) at create */
    uint32_t min_filter;     /* +0x80 V  TFILTER min (0xC6 low bits), 7 (linear mipmap linear) at create */
    uint32_t wrap_u;         /* +0x84 V  TWRAP u (0xC7 low bits), 0 (repeat) at create */
    uint32_t wrap_v;         /* +0x88 V  TWRAP v (0xC7 bits 8..), 0 at create */
    uint32_t pad_8C;         /* +0x8C I  not initialised */
} D012ImageObject;
D012_STATIC_ASSERT(sizeof(D012ImageObject) == 0x90, "D012ImageObject size");

/* Image-library memory classes (alloc table 0x089DA4C0[3], free table 0x089DA4CC[3]). */
enum D012ImageMemClass {
    D012_IMG_MEM_OBJECT = 0, /* V objects: GRAPHICS::GRAPHICS heap (FUN_088664C4 / FUN_088664FC) */
    D012_IMG_MEM_PIXELS = 1, /* V image and palette copies, precompiled lists: GRAPHICS::GRAPHICS heap */
    D012_IMG_MEM_VRAM   = 2  /* V GRAPHICS::EDRAM heap (FUN_08866528 / FUN_08866560), default returns 0 */
};

/* ------------------------------------------------------------------------------------------------
 * 4. Model (GMO) textures
 * ---------------------------------------------------------------------------------------------- */

/* Image or palette record of a GMO texture (0x30 bytes; copied 0x30 bytes at a time by FUN_089BBE20).
 * A texture keeps a chain of these: the record parsed from the GMO (kind 0, in RAM) and, after
 * FUN_089BCC84 with mode bit 2, a copy in VRAM (kind 1) appended to the chain. */
typedef struct D012GmoTexRecord {
    int16_t  refcount;       /* +0x00 V */
    uint16_t unk_02;         /* +0x02 V  copied with the record */
    uint32_t next_addr;      /* +0x04 V  next record in the chain */
    uint16_t kind;           /* +0x08 V  0 = original (RAM), 1 = VRAM copy (searched by FUN_089BCD70) */
    uint16_t unk_0A;         /* +0x0A I */
    uint32_t unk_0C;         /* +0x0C I */
    uint16_t format;         /* +0x10 V  GE TPF (image) / CLUT format (palette) */
    uint16_t width;          /* +0x12 V */
    uint16_t height;         /* +0x14 V */
    uint8_t  order;          /* +0x16 V  TMODE swizzle bit */
    uint8_t  bpp;            /* +0x17 V  bits per pixel */
    uint8_t  width_align;    /* +0x18 V  pixels; TBW = align_up(width, width_align) */
    uint8_t  height_align;   /* +0x19 V */
    uint8_t  clut_blocks;    /* +0x1A V  palette record: CLOAD count */
    uint8_t  unk_1B;         /* +0x1B I */
    uint32_t unk_1C;         /* +0x1C V  copied */
    uint32_t level_table_addr; /* +0x20 V  uint32_t addr[frame_count * level_count] (frame-major) */
    uint16_t level_count;    /* +0x24 V */
    uint16_t frame_count;    /* +0x26 V */
    uint8_t  level_type;     /* +0x28 V  1 = halving mips */
    uint8_t  frame_type;     /* +0x29 V */
    uint16_t extra_size;     /* +0x2A V  bytes copied from extra_addr */
    uint32_t extra_addr;     /* +0x2C V */
} D012GmoTexRecord;
D012_STATIC_ASSERT(sizeof(D012GmoTexRecord) == 0x30, "D012GmoTexRecord size");

/* GMO texture object (what the model's texture table points at). Created by FUN_089BB164
 * (FUN_089BA744(0, 0x10, 0x40): I read as class 0, align 0x10, 0x40 bytes) and filled by FUN_089BEAC0
 * (GIM/TIM2/BMP/raw parsers 0x089BF754, 0x089C0014, 0x089C09A8, 0x089C1824). May be shared by several
 * models through the name registry callbacks at 0x09AF7190 (register) / 0x09AF7198 (find), which the game
 * points at FUN_088663E0 / FUN_08866340 (D012GmoTexName table in the graphics object). */
/* R: the object FUN_089BCC84 receives. A 128x128 4-bit world-map texture at 0x08F8D920 read
 * 00000001 08F8FAC0 08F8FAC0 08F8D980 08F8D9B0 00000000 0000FF00 00FF0000 before its VRAM placement and
 * 00000001 041512C0 08F8FAC0 ... after it (0x041512C0 = the GE list FUN_089BCAC4 wrote in VRAM). */
typedef struct D012GmoTexture {
    int16_t  refcount;       /* +0x00 V R  ++ when shared (FUN_089BC7D8); 1 when not shared */
    uint16_t flags;          /* +0x02 V R  0 = static (simple path); bits 0-3 mip/anim; 0x10 = never precompile */
    uint32_t list_addr;      /* +0x04 V R  precompiled GE texture list, BASE + CALL target of FUN_089BD8CC.
                              *            Before VRAM placement it equals ram_list_addr (RAM); FUN_089BCC84
                              *            mode 2 replaces it with the list in VRAM (TMODE, TPF, TBP0, TBW0, TSIZE0,
                              *            TFLUSH, CMODE, CBP, CBPH, CLOAD, RET) */
    uint32_t ram_list_addr;  /* +0x08 R I  the RAM-side list (the first value of list_addr), kept after placement;
                              *            I: texture object + 0x21A0 for a 128x128 4-bit texture */
    uint32_t image_chain_addr;   /* +0x0C V R  D012GmoTexRecord chain (image); the kind-0 record follows the
                              *            object (texture object + 0x60 in the case above) */
    uint32_t palette_chain_addr; /* +0x10 V R  D012GmoTexRecord chain (palette) (texture object + 0x90) */
    uint32_t anim_addr;      /* +0x14 V  array of 0x30-byte animation states (+0x28/+0x2A image frames,
                              *          +0x2C/+0x2E palette frames) */
    uint8_t  anim_count;     /* +0x18 V R  0 */
    int8_t   anim_current;   /* +0x19 V R  -1 (0xFF) */
    uint16_t flags2;         /* +0x1A V  1 = blend two frames (TBP0/TBP1) */
    uint8_t  unk_1C;         /* +0x1C I */
    uint8_t  clut_shift;     /* +0x1D V R  CMODE (0) */
    uint8_t  clut_mask;      /* +0x1E V R  (0xFF) */
    uint8_t  clut_start;     /* +0x1F V R  (0) */
    uint8_t  unk_20[0x08];   /* +0x20 I */
    float    u_scale;        /* +0x28 V  1.0 at init (FUN_089BB0E0); I: UV scale, as the image object's */
    float    v_scale;        /* +0x2C V  1.0 at init */
    uint8_t  unk_30;         /* +0x30 V  1 at init */
    uint8_t  unk_31[2];      /* +0x31 I */
    uint8_t  unk_33;         /* +0x33 V  1 at init */
    uint8_t  mag_filter;     /* +0x34 V  1 at init (I: TFILTER mag, as the image object's +0x7C) */
    uint8_t  min_filter;     /* +0x35 V  7 at init (I: TFILTER min) */
    uint8_t  wrap_u;         /* +0x36 V  0 at init (I: TWRAP) */
    uint8_t  wrap_v;         /* +0x37 V  0 at init */
    uint8_t  unk_38[0x08];   /* +0x38 I */
} D012GmoTexture;
D012_STATIC_ASSERT(sizeof(D012GmoTexture) == 0x40, "D012GmoTexture size (I: allocation size)");

/* Entry of the model's texture table (model+0x18, count u16 at model+0x26). */
typedef struct D012GmoTexEntry {
    uint32_t unk_00;         /* +0x00 I */
    uint32_t texture_addr;   /* +0x04 V  D012GmoTexture* (0 = none); entry i = the GMO's i-th 0x000A texture block */
    uint32_t unk_08;         /* +0x08 I */
    uint32_t unk_0C;         /* +0x0C I */
} D012GmoTexEntry;
D012_STATIC_ASSERT(sizeof(D012GmoTexEntry) == 0x10, "D012GmoTexEntry size");

/* The parts of a GMO model object used here. Size I: FUN_089AC120 calls FUN_089AB5EC(0, 0x10, 0xD0),
 * read as (memory class 0, align 0x10, 0xD0 bytes). */
typedef struct D012GmoModel {
    uint8_t  unk_00[0x0C];   /* +0x00 I */
    uint32_t bones_addr;     /* +0x0C V  GMO block 4 (D: bones), 0x100-byte entries, count +0x20 */
    uint32_t parts_addr;     /* +0x10 V  GMO block 5 (D: parts), 16-byte entries {+4 array of 0x40-byte items, +8 u16 count} */
    uint32_t materials_addr; /* +0x14 V  GMO block 8 (D: materials). I: FUN_089B2DE8 reads *(x+0x28)->+4 as the texture */
    uint32_t textures_addr;  /* +0x18 V  GMO block 10, D012GmoTexEntry[texture_count] */
    uint32_t motions_addr;   /* +0x1C V  GMO block 0xB (D: motions), count +0x28 plus one */
    uint16_t bone_count;     /* +0x20 V */
    uint16_t part_count;     /* +0x22 V */
    uint16_t material_count; /* +0x24 V */
    uint16_t texture_count;  /* +0x26 V */
    uint16_t motion_count;   /* +0x28 V  block 0xB count (FUN_089B0E64) */
    uint8_t  unk_2A[0xD0 - 0x2A]; /* +0x2A I */
} D012GmoModel;
D012_STATIC_ASSERT(sizeof(D012GmoModel) == 0xD0, "D012GmoModel size (I: allocation size)");

/* Model holder: *(0x089D5F94) + 0x1C + i * 0x13C, 0xC0 of them (FUN_0886D2C4). */
typedef struct D012ModelPart {
    uint32_t unk_00;         /* +0x00 I */
    uint32_t unk_04;         /* +0x04 I */
    uint32_t model_addr;     /* +0x08 V  D012GmoModel* */
    uint32_t gmo_data_addr;  /* +0x0C V  source GMO inside the loaded file */
} D012ModelPart;
D012_STATIC_ASSERT(sizeof(D012ModelPart) == 0x10, "D012ModelPart size");

typedef struct D012ModelHolder {
    uint32_t unk_00;         /* +0x00 V  copied array (FUN_08869D44) */
    D012ModelPart parts[8];  /* +0x04 V  parts[0].model_addr at +0x0C; count u8 at +0x8C (I: max 8) */
    uint32_t current_part_addr; /* +0x84 V  points at parts[k]; the build code uses ->model_addr */
    uint32_t unk_88;         /* +0x88 V  non-zero skips the lazy build */
    uint8_t  part_count;     /* +0x8C V */
    uint8_t  unk_8D;         /* +0x8D I */
    uint8_t  unk_8E;         /* +0x8E V */
    uint8_t  unk_8F;         /* +0x8F I */
    uint32_t flags;          /* +0x90 V  build flags: 4 parts (always set for fighters), 8 textures to VRAM
                              *          (stages always; fighters only when the name starts "R_"), 1/2 other passes,
                              *          0x10 alternate path, 0x20 built, 0x40 build attempted.
                              *          R: fighters (obj/P_<ID>_1P.gmo) kept their textures in RAM; the world map's
                              *          obj/R_*.gmo models go through the same fighter setup (FUN_0884333C) */
    uint8_t  unk_94[0x13C - 0x94]; /* +0x94 I  (+0x110 name, 15 chars, V via FUN_08935BB4) */
} D012ModelHolder;
D012_STATIC_ASSERT(sizeof(D012ModelHolder) == 0x13C, "D012ModelHolder stride");

/* ------------------------------------------------------------------------------------------------
 * 5. Effects (FEP TEX blocks; bound by FUN_08972C14 in main)
 * ---------------------------------------------------------------------------------------------- */

typedef struct D012FepTexLevel {
    uint32_t width;          /* +0x00 V */
    uint32_t height;         /* +0x04 V */
    uint32_t pixel_offset;   /* +0x08 V  from the TEX block start */
} D012FepTexLevel;
D012_STATIC_ASSERT(sizeof(D012FepTexLevel) == 0x0C, "D012FepTexLevel size");

typedef struct D012FepTex {
    uint32_t unk_00;         /* +0x00 I */
    uint32_t cache_key;      /* +0x04 V  key of the effect VRAM cache (FUN_089742B4) */
    uint32_t unk_08;         /* +0x08 I */
    uint32_t unk_0C;         /* +0x0C I */
    uint32_t format_index;   /* +0x10 V  0..7 -> GE TPF {3,0,1,2,5,4,6,7} (table 0x089D0628) */
    uint32_t level_count;    /* +0x14 V */
    uint32_t clut_offset;    /* +0x18 V  D012FepClut from the TEX block start */
    /* D012FepTexLevel levels[level_count];  +0x1C V  TBW = align_up(width, 16); always swizzled */
} D012FepTex;
D012_STATIC_ASSERT(sizeof(D012FepTex) == 0x1C, "D012FepTex header size");

typedef struct D012FepClut {
    uint32_t entry_count;    /* +0x00 V */
    uint32_t format_index;   /* +0x04 V  0..3 -> GE CLUT format {3,0,1,2} (0x089D066C); entries per load block
                              *          {8,16,16,16} (0x089D067C) */
    uint32_t unk_08;         /* +0x08 I */
    uint32_t unk_0C;         /* +0x0C I */
    /* palette data            +0x10 V */
} D012FepClut;
D012_STATIC_ASSERT(sizeof(D012FepClut) == 0x10, "D012FepClut header size");

/* Effect VRAM caches: one for textures at effect manager + 0x30, one for palettes at + 0x640
 * (FUN_08973400, FUN_08972EBC). R: in a Quick Battle the manager was at 0x09B857E0, the texture cache's
 * base 0x04158C40 and the palette cache's base 0x04190C80; texture slots are 0x2000 / 0x4000 bytes
 * (128x64 / 128x128 8-bit), palette slots 0x400, and slots were reused (evicted) within one battle. */
enum D012EffectManagerOffsets {
    D012_EFFMGR_TEX_CACHE    = 0x030, /* V R the texture cache object (used when +0xC50 != 0) */
    D012_EFFMGR_PAL_CACHE    = 0x640, /* V R the palette cache object (used when +0xC54 != 0) */
    D012_EFFMGR_TEX_CACHE_ON = 0xC50, /* V */
    D012_EFFMGR_PAL_CACHE_ON = 0xC54  /* V */
};

enum D012FepCacheOffsets {
    D012_FEPCACHE_VRAM_BASE  = 0x000, /* V R VRAM address of the cache (0x04158C40 / 0x04190C80 in battle) */
    D012_FEPCACHE_SIZE       = 0x004, /* I  size in bytes */
    D012_FEPCACHE_SLOTS      = 0x60C  /* V  D012FepCacheSlot list */
};

/* Effect VRAM cache slot (list at cache+0x60C, next at +0x10). */
typedef struct D012FepCacheSlot {
    uint32_t vram_offset;    /* +0x00 V  added to *(cache+0) to give the VRAM address (FUN_089742E8) */
    uint32_t size;           /* +0x04 V  16-byte aligned */
    uint32_t key;            /* +0x08 V  D012FepTex.cache_key, -1 = free */
    uint32_t age;            /* +0x0C V  0 on (re)use; largest age is evicted (R: evictions seen in one battle) */
    uint32_t next_addr;      /* +0x10 V */
} D012FepCacheSlot;

/* ------------------------------------------------------------------------------------------------
 * 6. Graphics singletons
 * ---------------------------------------------------------------------------------------------- */

typedef struct D012GmoTexName {      /* graphics object +0x1AC8 + i * 0x24 */
    char     name[0x20];             /* +0x00 V  GMO texture name (block 0x12), strncpy 0x20 */
    uint32_t texture_addr;           /* +0x20 V  D012GmoTexture* */
} D012GmoTexName;
D012_STATIC_ASSERT(sizeof(D012GmoTexName) == 0x24, "D012GmoTexName size");

enum D012GraphicsOffsets {           /* object at *(0x089D5EC0) */
    D012_GFX_RAW_BLOCK     = 0x008,  /* V FUN_0888204C(0x780000) */
    D012_GFX_HEAP_GRAPHICS = 0x710,  /* V heap "GRAPHICS::GRAPHICS" over that block */
    D012_GFX_HEAP_EDRAM    = 0x714,  /* V heap "GRAPHICS::EDRAM": 0x04000000 + *(0x09B26AB0) to the end of EDRAM */
    D012_GFX_TEX_REGISTRY  = 0x1AC8, /* V D012GmoTexName[0x80] (GMO textures shared by name) */
    D012_GFX_TEX_REG_COUNT = 0x2CC8, /* V int */
    D012_GFX_TEX_REG_ON    = 0x2CCC, /* V u8: register while set (FUN_08865814) */
    D012_GFX_COLOR32_MODE  = 0x2CCF  /* V u8: 0 -> reserve 0x110000 (heap from 0x04110000), else 0x154000 */
};

enum D012GuContextOffsets {          /* libGu context at *(0x09AF6830) */
    D012_GU_LIST_START  = 0x04,      /* V */
    D012_GU_LIST_CURSOR = 0x08,      /* V FUN_089A0144 returns it, FUN_089A0180 sets it */
    D012_GU_LIST_SIZE   = 0x0C       /* V */
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* D012_TEXTURE_TYPES_H */
