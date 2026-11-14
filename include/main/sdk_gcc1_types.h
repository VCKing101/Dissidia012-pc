#ifndef MAIN_SDK_GCC1_TYPES_H
#define MAIN_SDK_GCC1_TYPES_H

/* Shared types for the Sony psp-gcc 1.3.1 library code in main
 * (0x089985B0-0x089AC3DC). */

/* ---- libgcc2 (GCC 3.3.3) ---- */

typedef int SItype;
typedef unsigned int USItype;
typedef long long DItype;
typedef unsigned long long UDItype;
typedef unsigned char UQItype;

struct DWstruct {
    SItype low, high;
};

typedef union {
    struct DWstruct s;
    DItype ll;
} DWunion;

/* __clz_tab: bit length of each byte value. */
extern const UQItype D_089D11F8[];

#define __ll_B ((USItype)1 << 16)
#define __ll_lowpart(t) ((USItype)(t) & (__ll_B - 1))
#define __ll_highpart(t) ((USItype)(t) >> 16)

#define count_leading_zeros(count, x)                                         \
    do {                                                                      \
        USItype __xr = (x);                                                   \
        USItype __a;                                                          \
        if (32 <= 32) {                                                       \
            __a = __xr < ((USItype)1 << 16)                                   \
                      ? (__xr < ((USItype)1 << 8) ? 0 : 8)                    \
                      : (__xr < ((USItype)1 << 24) ? 16 : 24);                \
        } else {                                                              \
            for (__a = 32 - 8; __a > 0; __a -= 8)                             \
                if (((__xr >> __a) & 0xff) != 0)                              \
                    break;                                                    \
        }                                                                     \
        (count) = 32 - (D_089D11F8[__xr >> __a] + __a);                       \
    } while (0)

#define sub_ddmmss(sh, sl, ah, al, bh, bl)                                    \
    do {                                                                      \
        USItype __x;                                                          \
        __x = (al) - (bl);                                                    \
        (sh) = (ah) - (bh) - (__x > (al));                                    \
        (sl) = __x;                                                           \
    } while (0)

#define umul_ppmm(w1, w0, u, v)                                               \
    do {                                                                      \
        UDItype __p = (UDItype)(USItype)(u) * (USItype)(v);                   \
        (w1) = (USItype)(__p >> 32);                                          \
        (w0) = (USItype)__p;                                                  \
    } while (0)

#define udiv_qrnnd(q, r, n1, n0, d)                                           \
    do {                                                                      \
        USItype __d1, __d0, __q1, __q0;                                       \
        USItype __r1, __r0, __m;                                              \
        __d1 = __ll_highpart(d);                                              \
        __d0 = __ll_lowpart(d);                                               \
                                                                              \
        __r1 = (n1) % __d1;                                                   \
        __q1 = (n1) / __d1;                                                   \
        __m = (USItype)__q1 * __d0;                                           \
        __r1 = __r1 * __ll_B | __ll_highpart(n0);                             \
        if (__r1 < __m) {                                                     \
            __q1--, __r1 += (d);                                              \
            if (__r1 >= (d))                                                  \
                if (__r1 < __m)                                               \
                    __q1--, __r1 += (d);                                      \
        }                                                                     \
        __r1 -= __m;                                                          \
                                                                              \
        __r0 = __r1 % __d1;                                                   \
        __q0 = __r1 / __d1;                                                   \
        __m = (USItype)__q0 * __d0;                                           \
        __r0 = __r0 * __ll_B | __ll_lowpart(n0);                              \
        if (__r0 < __m) {                                                     \
            __q0--, __r0 += (d);                                              \
            if (__r0 >= (d))                                                  \
                if (__r0 < __m)                                               \
                    __q0--, __r0 += (d);                                      \
        }                                                                     \
        __r0 -= __m;                                                          \
                                                                              \
        (q) = (USItype)__q1 * __ll_B | __q0;                                  \
        (r) = __r0;                                                           \
    } while (0)

/* libgcc2's __udivmoddi4, inlined into __divdi3, __udivdi3 and __umoddi3. */
static inline __attribute__((__always_inline__)) UDItype sdk_udivmoddi4(UDItype n, UDItype d, UDItype *rp)
{
    DWunion ww;
    DWunion nn, dd;
    DWunion rr;
    USItype d0, d1, n0, n1, n2;
    USItype q0, q1;
    USItype b, bm;

    nn.ll = n;
    dd.ll = d;

    d0 = dd.s.low;
    d1 = dd.s.high;
    n0 = nn.s.low;
    n1 = nn.s.high;

    if (d1 == 0) {
        if (d0 > n1) {
            /* 0q = nn / 0D */
            count_leading_zeros(bm, d0);

            if (bm != 0) {
                d0 = d0 << bm;
                n1 = (n1 << bm) | (n0 >> (32 - bm));
                n0 = n0 << bm;
            }

            udiv_qrnnd(q0, n0, n1, n0, d0);
            q1 = 0;
        } else {
            /* qq = NN / 0d */
            if (d0 == 0)
                d0 = 1 / d0; /* Divide intentionally by zero. */

            count_leading_zeros(bm, d0);

            if (bm == 0) {
                n1 -= d0;
                q1 = 1;
            } else {
                b = 32 - bm;

                d0 = d0 << bm;
                n2 = n1 >> b;
                n1 = (n1 << bm) | (n0 >> b);
                n0 = n0 << bm;

                udiv_qrnnd(q1, n1, n2, n1, d0);
            }

            udiv_qrnnd(q0, n0, n1, n0, d0);
        }

        if (rp != 0) {
            rr.s.low = n0 >> bm;
            rr.s.high = 0;
            *rp = rr.ll;
        }
    } else {
        if (d1 > n1) {
            /* 00 = nn / DD */
            q0 = 0;
            q1 = 0;

            if (rp != 0) {
                rr.s.low = n0;
                rr.s.high = n1;
                *rp = rr.ll;
            }
        } else {
            /* 0q = NN / dd */
            count_leading_zeros(bm, d1);
            if (bm == 0) {
                if (n1 > d1 || n0 >= d0) {
                    q0 = 1;
                    sub_ddmmss(n1, n0, n1, n0, d1, d0);
                } else
                    q0 = 0;

                q1 = 0;

                if (rp != 0) {
                    rr.s.low = n0;
                    rr.s.high = n1;
                    *rp = rr.ll;
                }
            } else {
                USItype m1, m0;

                b = 32 - bm;

                d1 = (d1 << bm) | (d0 >> b);
                d0 = d0 << bm;
                n2 = n1 >> b;
                n1 = (n1 << bm) | (n0 >> b);
                n0 = n0 << bm;

                udiv_qrnnd(q0, n1, n2, n1, d1);
                umul_ppmm(m1, m0, q0, d0);

                if (m1 > n1 || (m1 == n1 && m0 > n0)) {
                    q0--;
                    sub_ddmmss(m1, m0, m1, m0, d1, d0);
                }

                q1 = 0;

                if (rp != 0) {
                    sub_ddmmss(n1, n0, n1, n0, m1, m0);
                    rr.s.low = (n1 << b) | (n0 >> bm);
                    rr.s.high = n1 >> bm;
                    *rp = rr.ll;
                }
            }
        }
    }

    ww.s.low = q0;
    ww.s.high = q1;
    return ww.ll;
}

/* ---- libgcc fp-bit.c soft float (GCC 3.3.3) ----
 * _d: double (dp-bit, 64-bit fraction, NGARDS 8)
 * _f: float (sp-bit, 32-bit fraction, NGARDS 7) */

typedef float SFtype;
typedef double DFtype;
typedef int CMPtype;

typedef enum {
    CLASS_SNAN,
    CLASS_QNAN,
    CLASS_ZERO,
    CLASS_NUMBER,
    CLASS_INFINITY
} fp_class_type;

typedef struct {
    fp_class_type class;
    unsigned int sign;
    int normal_exp;
    union {
        UDItype ll;
        USItype l[2];
    } fraction;
} fp_number_type_d;

typedef struct {
    fp_class_type class;
    unsigned int sign;
    int normal_exp;
    union {
        USItype ll;
        unsigned short l[2];
    } fraction;
} fp_number_type_f;

/* MIPS builds dp-bit.c and sp-bit.c with FLOAT_BIT_ORDER_MISMATCH on
 * little-endian targets, which adds the packed `bits` view (and makes the
 * union BLKmode: 16-byte stack slots). */
typedef union {
    DFtype value;
    UDItype value_raw;
    USItype words[2];
    struct {
        UDItype fraction : 52 __attribute__((packed));
        unsigned int exp : 11 __attribute__((packed));
        unsigned int sign : 1 __attribute__((packed));
    } bits;
} FLO_union_type_d;

typedef union {
    SFtype value;
    USItype value_raw;
    struct {
        USItype fraction : 23 __attribute__((packed));
        unsigned int exp : 8 __attribute__((packed));
        unsigned int sign : 1 __attribute__((packed));
    } bits;
} FLO_union_type_f;

#define D_NGARDS 8
#define D_FRACBITS 52
#define D_EXPBIAS 1023
#define D_EXPMAX 0x7ff
#define D_QUIET_NAN 0x8000000000000LL
#define D_FRAC_NBITS 64
#define D_GARDROUND 0x7f
#define D_GARDMASK 0xff
#define D_GARDMSB 0x80
#define D_IMPLICIT_1 ((UDItype)1 << (D_FRACBITS + D_NGARDS))
#define D_IMPLICIT_2 ((UDItype)1 << (D_FRACBITS + 1 + D_NGARDS))

#define F_NGARDS 7
#define F_FRACBITS 23
#define F_EXPBIAS 127
#define F_EXPMAX 0xff
#define F_QUIET_NAN 0x100000L
#define F_FRAC_NBITS 32
#define F_GARDROUND 0x3f
#define F_GARDMASK 0x7f
#define F_GARDMSB 0x40
#define F_IMPLICIT_1 ((USItype)1 << (F_FRACBITS + F_NGARDS))
#define F_IMPLICIT_2 ((USItype)1 << (F_FRACBITS + 1 + F_NGARDS))

/* Bits a float fraction moves by when widened to a double one. */
#define F_D_BITOFF (D_FRACBITS + D_NGARDS - F_FRACBITS - F_NGARDS)

#define LSHIFT(a) { a = (a & 1) | (a >> 1); }

#define MAX_SI_INT ((SItype)0x7fffffff)
#define MAX_USI_INT ((USItype)~0)
#define BITS_PER_SI 32

static __inline__ int isnan_d(fp_number_type_d *x)
{
    return x->class == CLASS_SNAN || x->class == CLASS_QNAN;
}
static __inline__ int isinf_d(fp_number_type_d *x)
{
    return x->class == CLASS_INFINITY;
}
static __inline__ int iszero_d(fp_number_type_d *x)
{
    return x->class == CLASS_ZERO;
}
static __inline__ void flip_sign_d(fp_number_type_d *x)
{
    x->sign = !x->sign;
}

static __inline__ int isnan_f(fp_number_type_f *x)
{
    return x->class == CLASS_SNAN || x->class == CLASS_QNAN;
}
static __inline__ int isinf_f(fp_number_type_f *x)
{
    return x->class == CLASS_INFINITY;
}
static __inline__ int iszero_f(fp_number_type_f *x)
{
    return x->class == CLASS_ZERO;
}
static __inline__ void flip_sign_f(fp_number_type_f *x)
{
    x->sign = !x->sign;
}

/* __thenan_df */
extern const fp_number_type_d D_089D11E0;

static __inline__ fp_number_type_d *nan_d(void)
{
    return (fp_number_type_d *)&D_089D11E0;
}

/* dp-bit entry points */
DFtype func_0899AD20(fp_number_type_d *src);                 /* __pack_d */
void func_0899AFC0(FLO_union_type_d *src, fp_number_type_d *dst); /* __unpack_d */
CMPtype func_0899B0DC(fp_number_type_d *a, fp_number_type_d *b);  /* __fpcmp_parts_d */
DFtype func_0899A9E0(fp_class_type class, unsigned int sign, int exp, UDItype frac); /* __make_dp */
fp_number_type_d *func_08999DD0(fp_number_type_d *a, fp_number_type_d *b, fp_number_type_d *tmp); /* _fpadd_parts */

/* sp-bit entry points */
SFtype func_0899B45C(fp_number_type_f *src);                 /* __pack_f */
void func_0899AC24(FLO_union_type_f *src, fp_number_type_f *dst); /* __unpack_f */
SFtype func_0899ACF0(fp_class_type class, unsigned int sign, int exp, USItype frac); /* __make_fp */

/* ---- Image library: pooled allocator (0x0899B6B0-0x0899B9EC) ----
 * Three heaps (0, 1, 2), each with a malloc/free pair and lists of pools
 * that small allocations are carved from. */

typedef void *(*SdkAllocFn)(unsigned int size);
typedef void (*SdkFreeFn)(void *ptr);

typedef struct SdkPool {
    struct SdkPool *next;   /* 0x0 */
    unsigned short count;   /* 0x4: live allocations */
    unsigned short align;   /* 0x6: alignment mask */
    char *end;              /* 0x8 */
    char *cur;              /* 0xC: next free byte */
} SdkPool;

extern SdkAllocFn D_089DA4C0[3]; /* malloc per heap */
extern SdkFreeFn D_089DA4CC[3];  /* free per heap */
extern SdkPool *D_09AF6790[3];   /* pool being allocated from, per heap */
extern SdkPool *D_09AF679C[3];   /* all pools, per heap */

void *func_0899B888(int heap, unsigned int size);
void func_0899B920(int heap, void *ptr);
void *func_0899B75C(unsigned int size); /* heap 0 */
void *func_0899B794(unsigned int size); /* heap 1 */
void *func_0899B7CC(unsigned int size); /* heap 2 */
void func_0899B804(void *ptr);          /* heap 0 */
void func_0899B830(void *ptr);          /* heap 1 */
void func_0899B85C(void *ptr);          /* heap 2 */

/* ---- Image library: image objects (GIM, TIM2, BMP, ...) ---- */

#define SDK_GIM_MAGIC 0x2E47494D  /* "MIG." */
#define SDK_TIM2_MAGIC 0x324D4954 /* "TIM2" */
#define SDK_BMP_MAGIC 0x4D42      /* "BM" */

/* A GIM chunk header (picture, image or palette block). */
typedef struct SdkGimChunk {
    unsigned short id;      /* 0x00: 2 file, 3 picture, 4 image, 5 palette, 6 sequence */
    unsigned short refs;    /* 0x02 */
    unsigned int next;      /* 0x04: offset of the next chunk */
    unsigned int child;     /* 0x08: offset of the first child chunk */
    unsigned int data;      /* 0x0C: offset of the chunk data */
} SdkGimChunk;

/* GIM image / palette info header, copied out of the file; offsets are
 * relocated to pointers after the copy. */
typedef struct SdkGimImage {
    unsigned short header_size; /* 0x00 */
    unsigned short refs;        /* 0x02: reused as a reference count */
    unsigned short format;      /* 0x04 */
    unsigned short order;       /* 0x06 */
    unsigned short width;       /* 0x08 */
    unsigned short height;      /* 0x0A */
    unsigned short bpp;         /* 0x0C */
    unsigned short pitch_align; /* 0x0E */
    unsigned short height_align;/* 0x10 */
    unsigned short dim_count;   /* 0x12 */
    unsigned short reserved[2]; /* 0x14 */
    unsigned int index_start;   /* 0x18: offset of the level/frame offset table */
    unsigned int pixels_start;  /* 0x1C */
    unsigned int pixels_end;    /* 0x20 */
    unsigned int plane_mask;    /* 0x24 */
    unsigned short level_type;  /* 0x28 */
    unsigned short level_count; /* 0x2A */
    unsigned short frame_type;  /* 0x2C */
    unsigned short frame_count; /* 0x2E */
} SdkGimImage;

/* GIM sequence info header, copied out of the file. */
typedef struct SdkGimSequence {
    unsigned short header_size; /* 0x00 */
    unsigned short refs;        /* 0x02: reused as a reference count */
    unsigned short f_04;        /* 0x04 */
    unsigned short f_06;        /* 0x06 */
    unsigned short f_08;        /* 0x08 */
    unsigned short f_0A;        /* 0x0A */
    unsigned short f_0C;        /* 0x0C */
    unsigned short f_0E;        /* 0x0E */
} SdkGimSequence;

typedef struct SdkImage {
    int refs;                 /* 0x00 */
    unsigned int flags;       /* 0x04 */
    int f_08;                 /* 0x08 */
    SdkGimImage *image;       /* 0x0C: image chunk data copy (heap 1) */
    SdkGimImage *palette;     /* 0x10: palette chunk data copy (heap 1) */
    int f_14;                 /* 0x14 */
    int f_18;                 /* 0x18 */
    unsigned short f_1C;      /* 0x1C */
    unsigned short f_1E;      /* 0x1E */
    int f_20;                 /* 0x20 */
    int f_24;                 /* 0x24 */
    float scale_u;            /* 0x28 */
    float scale_v;            /* 0x2C */
    float f_30;               /* 0x30 */
    float f_34;               /* 0x34 */
    float f_38;               /* 0x38 */
    int f_3C;                 /* 0x3C */
    int f_40;                 /* 0x40 */
    int f_44;                 /* 0x44 */
    int f_48;                 /* 0x48 */
    int f_4C;                 /* 0x4C */
    int f_50;                 /* 0x50 */
    SdkGimSequence **sequences; /* 0x54: sequence chunk data copies (heap 0) */
    int num_sequences;        /* 0x58 */
    int f_5C;                 /* 0x5C */
    int f_60;                 /* 0x60 */
    int f_64;                 /* 0x64 */
    int f_68;                 /* 0x68 */
    int f_6C;                 /* 0x6C */
    int f_70;                 /* 0x70 */
    int f_74;                 /* 0x74 */
    int f_78;                 /* 0x78 */
    int f_7C;                 /* 0x7C */
    int f_80;                 /* 0x80 */
    int f_84;                 /* 0x84 */
    int f_88;                 /* 0x88 */
    int f_8C;                 /* 0x8C */
} SdkImage;

/* TGA file header as read field by field. */
typedef struct SdkTgaHeader {
    unsigned char id_length;        /* 0x00 */
    unsigned char colormap_type;    /* 0x01 */
    unsigned char image_type;       /* 0x02 */
    unsigned short colormap_first;  /* 0x04 */
    unsigned short colormap_length; /* 0x06 */
    unsigned char colormap_bpp;     /* 0x08 */
    unsigned short x_origin;        /* 0x0A */
    unsigned short y_origin;        /* 0x0C */
    unsigned short width;           /* 0x0E */
    unsigned short height;          /* 0x10 */
    unsigned char bpp;              /* 0x12 */
    unsigned char descriptor;       /* 0x13 */
} SdkTgaHeader;

/* ---- SHA-1 (0x0899F3FC-) ---- */

typedef struct SdkSha1Context {
    unsigned int h[5];            /* 0x00 */
    unsigned short remains;       /* 0x14: bytes waiting in buf */
    unsigned short computed;      /* 0x16 */
    unsigned long long total;     /* 0x18: message length */
    unsigned char buf[64];        /* 0x20 */
} SdkSha1Context;

/* ---- Graphics library (sceGu-like), state at 0x09AF67E8 ---- */

/* A graphics context: its display list and the state the GE commands mirror. */
typedef struct SdkGuList {
    int f_00;
    unsigned int *start;    /* 0x04 */
    unsigned int *cur;      /* 0x08 */
    unsigned int size;      /* 0x0C: bytes */
    int f_10[(0xA8 - 0x10) / 4];
    int scissor_enable;     /* 0xA8 */
    int scissor_x0;         /* 0xAC */
    int scissor_y0;         /* 0xB0 */
    int scissor_x1;         /* 0xB4 */
    int scissor_y1;         /* 0xB8 */
    int f_BC;               /* 0xBC */
    int f_C0;               /* 0xC0 */
    int f_C4;               /* 0xC4 */
    int f_C8;               /* 0xC8 */
    int f_CC;               /* 0xCC */
    int f_D0;               /* 0xD0 */
    int f_D4;               /* 0xD4 */
    int f_D8;               /* 0xD8 */
    int f_DC;               /* 0xDC */
    int f_E0;               /* 0xE0 */
    int tex_func_mode;      /* 0xE4 */
    int tex_func;           /* 0xE8 */
    int tex_func_alpha;     /* 0xEC */
} SdkGuList;

typedef struct SdkGuState {
    int initialized;        /* 0x00 */
    int f_04;               /* 0x04 */
    int f_08;               /* 0x08 */
    int f_0C;               /* 0x0C */
    int f_10;               /* 0x10 */
    int f_14;               /* 0x14 */
    int f_18;               /* 0x18 */
    int f_1C;               /* 0x1C */
    int f_20;               /* 0x20 */
    int f_24;               /* 0x24 */
    int f_28;               /* 0x28 */
    int f_2C;               /* 0x2C */
    int f_30;               /* 0x30 */
    int f_34;               /* 0x34 */
    int f_38;               /* 0x38 */
    int f_3C;               /* 0x3C */
    int f_40;               /* 0x40 */
    unsigned int states;    /* 0x44: enabled-state bits */
    SdkGuList *list;        /* 0x48: list being built */
} SdkGuState;

extern SdkGuState D_09AF67E8;

#define SDK_GU_SEND(cmd, arg) (*D_09AF67E8.list->cur++ = ((cmd) << 24) | (arg))

int func_0899BC64(SdkImage *img, SdkGimChunk *picture);
void func_0899BB1C(SdkImage *img);
void func_0899BEE0(SdkImage *img);
void func_0899BFE0(SdkImage *img);

#endif
