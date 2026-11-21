#ifndef MAIN_AGENT34_SDK_H
#define MAIN_AGENT34_SDK_H

/*
 * Sony SDK texture libraries in main (psp-gcc 1.3.1): the image library (0x0899B6B0-0x0899F3FC),
 * libGu (0x0899F3FC-0x089A2C24), the GMO model library (0x089AB000-0x089B8000) and the GMO texture
 * library (0x089BA000-0x089BF400). Names follow docs/port/texture-pipeline.md and
 * docs/port/texture_types.h, where every field is described.
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef short s16;
typedef int s32;

/* ---- libGu: the context at *(0x09AF6830) ---- */

typedef struct D012GuContext {
    u32 unk_00;          /* 0x00 */
    u32 *list_start;     /* 0x04 */
    u32 *list_cursor;    /* 0x08: next command word */
    u32 list_size;       /* 0x0C: bytes */
} D012GuContext;

/* libGu's global state at 0x09AF67E8; the current context pointer is at +0x48 (0x09AF6830). */
typedef struct D012GuState {
    u32 unk_00[0x48 / 4];
    D012GuContext *context; /* 0x48 */
} D012GuState;

extern D012GuState D_09AF67E8;

/* Append one GE command word to the current list. */
#define GU_SEND(cmd, arg) (*D_09AF67E8.context->list_cursor++ = ((u32)(cmd) << 24) | (arg))

/* ---- Image library (0x0899B6B0-0x0899F030) ---- */

/* Data of a GIM image (block type 4) or palette (type 5), copied into class-1 memory; the level/frame
 * table is made absolute by the copy. TIM2, BMP and raw images are given the same layout. */
typedef struct D012GimImageData {
    u16 header_size;    /* 0x00: 0x30 */
    u16 refcount;       /* 0x02 */
    u16 format;         /* 0x04: GE texture format (palette: CLUT format) */
    u16 order;          /* 0x06: 1 = swizzled */
    u16 width;          /* 0x08 */
    u16 height;         /* 0x0A */
    u16 bpp;            /* 0x0C */
    u16 pitch_align;    /* 0x0E */
    u16 height_align;   /* 0x10 */
    u16 unk_12;         /* 0x12 */
    u16 unk_14;         /* 0x14 */
    u16 unk_16;         /* 0x16 */
    u32 table_offset;   /* 0x18: level/frame address table, from this struct */
    u32 pixels_start;   /* 0x1C */
    u32 data_size;      /* 0x20 */
    u32 plane_mask;     /* 0x24 */
    u16 level_type;     /* 0x28 */
    u16 level_count;    /* 0x2A */
    u16 frame_type;     /* 0x2C */
    u16 frame_count;    /* 0x2E */
} D012GimImageData;

/* The image object (0x90 bytes, class 0). */
typedef struct D012ImageObject {
    u32 refcount;                    /* 0x00 */
    u32 flags;                       /* 0x04 */
    u32 draw_flags;                  /* 0x08: 1 two frames/palettes, 2 TEXLEVEL, 4 UV scale+offset, 0x100 blend,
                                      *       0x200 TFUNC, 0x400 TFILTER, 0x800 TWRAP (func_0899D014) */
    D012GimImageData *image_addr;    /* 0x0C: pixels */
    D012GimImageData *palette_addr;  /* 0x10 */
    u32 *list_addr;                  /* 0x14: precompiled texture commands */
    u32 vram_addr;                   /* 0x18: {image copy, palette copy} in VRAM, used instead of +0x0C/+0x10 */
    s16 buffer_width;                /* 0x1C: TBW of level 0 (read with lh) */
    s16 clut_blocks;                 /* 0x1E: CLOAD count (read with lh) */
    float u_offset;                  /* 0x20 */
    float v_offset;                  /* 0x24 */
    float u_scale;                   /* 0x28 */
    float v_scale;                   /* 0x2C */
    float unk_30;                    /* 0x30 */
    float unk_34;                    /* 0x34 */
    float unk_38;                    /* 0x38 */
    u32 unk_3C;                      /* 0x3C */
    float mip_bias;                  /* 0x40 */
    s32 image_frame;                 /* 0x44 */
    s32 image_frame2;                /* 0x48 */
    s32 palette_frame;               /* 0x4C */
    s32 palette_frame2;              /* 0x50 */
    u32 seq_list_addr;               /* 0x54 */
    s32 seq_count;                   /* 0x58 */
    s32 unk_5C;                      /* 0x5C */
    s32 clut_param;                  /* 0x60 */
    u32 clut_shift;                  /* 0x64 */
    u32 clut_mask;                   /* 0x68 */
    u32 clut_start;                  /* 0x6C */
    u32 blend_index;                 /* 0x70 */
    u32 tfunc_func;                  /* 0x74 */
    u32 tfunc_alpha;                 /* 0x78 */
    u32 tfilter_mag;                 /* 0x7C: TFILTER (0xC6) magnification filter, 1 (linear) at create */
    u32 tfilter_min;                 /* 0x80: TFILTER minification filter, 7 (linear, mipmap linear) */
    u32 twrap_u;                     /* 0x84: TWRAP (0xC7) u mode, 0 (repeat) */
    u32 twrap_v;                     /* 0x88: TWRAP v mode */
    u32 pad_8C;                      /* 0x8C */
} D012ImageObject;

void func_0899BEE0(D012ImageObject *img);                              /* img_release_data */
void *func_0899C1D4(D012ImageObject *img, int level, int frame);       /* image level/frame pixels */
void *func_0899C24C(D012ImageObject *img, int level, int frame);       /* palette level/frame entries */
int func_0899EAEC(D012ImageObject *img, int format, int width, int height, int levels, int frames,
                  const void *pixels);                                 /* img_make_block, order 0 */
int func_0899EE30(D012ImageObject *img, int format, int count, int levels, int frames,
                  const void *entries);                                /* img_make_palette_block */
void func_0899E6F0(void *dst, const void *src, int width, int height, int format); /* convert texels */
void func_0899E900(void *clut, int format, int count);                 /* TIM2 CLUT (CSM1) to linear */
void func_089C6040(void);                                              /* write back the data cache */

/* Texture formats of the TIM2 image/CLUT types, indexed by TIM2 type (0x089D14C8). */
extern const int D_089D14C8[];

/* TIM2 file header. */
typedef struct D012Tim2Header {
    char magic[4];      /* 0x00: "TIM2" */
    s8 version;         /* 0x04 */
    s8 align_128;       /* 0x05: pictures start at 0x80 (else 0x10) */
    s16 picture_count;  /* 0x06 */
    u32 unk_08[2];      /* 0x08 */
} D012Tim2Header;

/* TIM2 picture header. */
typedef struct D012Tim2Picture {
    u32 total_size;     /* 0x00: header + CLUT + image */
    u32 clut_size;      /* 0x04 */
    u32 image_size;     /* 0x08 */
    s16 header_size;    /* 0x0C */
    s16 clut_colors;    /* 0x0E */
    s8 pict_format;     /* 0x10 */
    s8 mipmap_count;    /* 0x11 */
    s8 clut_type;       /* 0x12: bits 0-2 CLUT format; bit 7 = already linear (no CSM1 reorder) */
    s8 image_type;      /* 0x13 */
    s16 width;          /* 0x14 */
    s16 height;         /* 0x16 */
    u32 gs_regs[6];     /* 0x18 */
} D012Tim2Picture;

/* TIM2 mipmap header, after the picture header when mipmap_count > 1. */
typedef struct D012Tim2Mipmap {
    u32 miptbp[4];      /* 0x00 */
    u32 sizes[8];       /* 0x10: bytes per level */
} D012Tim2Mipmap;

/* BMP file and info headers as func_0899D674 reads them, field by field, into aligned memory. */
typedef struct D012BmpHeader {
    u16 type;            /* 0x00: "BM" */
    u32 file_size;       /* 0x04 */
    u16 reserved1;       /* 0x08 */
    u16 reserved2;       /* 0x0A */
    s32 pixel_offset;    /* 0x0C: from the start of the file; the palette sits between 0x36 and here */
    u32 info_size;       /* 0x10 */
    s32 width;           /* 0x14 */
    s32 height;          /* 0x18: negative = top-down rows */
    u16 planes;          /* 0x1C */
    s16 bpp;             /* 0x1E: 4, 8, 16, 24 or 32 */
    u32 compression;     /* 0x20: 0 = none, else RLE4/RLE8 or 16-bit bit fields */
    u32 image_size;      /* 0x24 */
    u32 x_ppm;           /* 0x28 */
    u32 y_ppm;           /* 0x2C */
    u32 colors_used;     /* 0x30 */
    u32 colors_important; /* 0x34 */
} D012BmpHeader;

void func_0899D674(const u8 *data, D012BmpHeader *hdr);                 /* bmp_read_header */

/* TGA file header as func_0899E270 reads it, field by field. */
typedef struct D012TgaHeader {
    u8 id_length;        /* 0x00 */
    u8 colormap_type;    /* 0x01: 1 = palette follows */
    u8 image_type;       /* 0x02: 1 colour-mapped, 2 true colour; | 8 = RLE */
    u8 pad_03;
    u16 colormap_first;  /* 0x04 */
    u16 colormap_length; /* 0x06: palette entries */
    u8 colormap_bpp;     /* 0x08: 24 or 32 */
    u8 pad_09;
    u16 x_origin;        /* 0x0A */
    u16 y_origin;        /* 0x0C */
    u16 width;           /* 0x0E */
    u16 height;          /* 0x10 */
    u8 bpp;              /* 0x12: 8, 16, 24 or 32 */
    u8 descriptor;       /* 0x13: bit 5 = top-down rows */
} D012TgaHeader;

const u8 *func_0899E270(const u8 *data, D012TgaHeader *hdr);            /* tga_read_header */
/* tga_pixels: `count` pixels of `bpp` bytes to RGBA (8: index, 16: 5551 swapped), repeating one
 * source pixel when `repeat` (RLE runs). */
void func_0899E364(void *dst, const u8 *src, int count, int bpp, u8 repeat);
/* BMP pixel converters: (dst, src, width, height), rows bottom-up when height > 0. */
void func_0899D7CC(void *dst, const u8 *src, int width, int height);    /* 4-bit */
void func_0899DB74(void *dst, const u8 *src, int width, int height);    /* 4-bit RLE */
void func_0899D868(void *dst, const u8 *src, int width, int height);    /* 8-bit */
void func_0899DD7C(void *dst, const u8 *src, int width, int height);    /* 8-bit RLE */
void func_0899D8E8(void *dst, const u8 *src, int width, int height);    /* 16-bit 1555 */
void func_0899D994(void *dst, const u8 *src, int width, int height);    /* 16-bit 565 */
void func_0899DA38(void *dst, const u8 *src, int width, int height);    /* 24-bit */
void func_0899DADC(void *dst, const u8 *src, int width, int height, int alpha); /* 32-bit / palette */

/* ---- GMO texture library: small vectors and matrices ---- */

typedef struct D012Vec3 {
    float x, y, z;
} D012Vec3;

typedef struct D012Vec4 {
    float x, y, z, w;
} D012Vec4;

/* 4x4 matrix, four rows of four floats (VFPU quad loads/stores). */
typedef struct D012Mat4 {
    D012Vec4 x, y, z, w;
} D012Mat4;

/* The vector helpers are GCC inline asm with explicit VFPU registers and "m" operands in a noreorder
 * block, like the SDK's own libvfpu (src/main/sdk/libvfpu_*.c). */
#define VFPU_BEGIN ".set push\n .set noreorder\n"
#define VFPU_END ".set pop\n"

#endif
