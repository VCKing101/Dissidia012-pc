#ifndef DISSIDIA_MAIN_AUTO_REGION4_H
#define DISSIDIA_MAIN_AUTO_REGION4_H

/* Types and prototypes behind the automatically matched functions in main
 * 0x088D96E4..0x0891C1FC (readability pass, main region 4). Builds on
 * auto_region2.h (and through it auto_region1.h), not on auto_region3.h,
 * and follows their naming: classes with a vtable pointer are named after
 * the vtable (Obj9CC290 stores D_089CC290), others after the first function
 * that shows their layout (Obj8DA8E8), objects reached through a global
 * pointer after the global. Fields whose use is not plain keep their offset
 * as their name. Every layout carries compile-time size/offset assertions
 * (MAIN4_SIZE / MAIN4_AT).
 *
 * Generated from a field spec (build/scratch/readability-main4/spec*.txt and
 * protos*.txt): pads and assertions are written by script. */

#include "main/auto_region2.h"

#define MAIN4_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define MAIN4_ASSERT(name, cond) typedef char main4_assert_##name[(cond) ? 1 : -1]
#define MAIN4_SIZE(type, size) MAIN4_ASSERT(size_##type, sizeof(type) == (size))
#define MAIN4_AT(type, field, offset) \
    MAIN4_ASSERT(at_##type##_##field, MAIN4_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ 0x088D96E4..0x088DCEF8 */

extern int D_089CC250, D_089CC290, D_089CC2A8, D_089CC2D0, D_089CC308, D_089CC330;
extern int D_089CC358, D_089CC538, D_089CC610, D_089CC710, D_089CC738, D_089CC7A0;
extern int D_089CC9A8, D_089CC9C8, D_089CC9E8, D_089CCAD0, D_089CCB08, D_089CCB48;

/* Vtable D_089CC250 at +0x10; reset by func_088D973C, released by func_088D9750. */
typedef struct Obj9CC250 {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} Obj9CC250;
MAIN4_SIZE(Obj9CC250, 0x14);
MAIN4_AT(Obj9CC250, active, 0x0);
MAIN4_AT(Obj9CC250, field_04, 0x4);
MAIN4_AT(Obj9CC250, field_08, 0x8);
MAIN4_AT(Obj9CC250, field_0C, 0xC);
MAIN4_AT(Obj9CC250, vtable, 0x10);

/* A word at +0xC (and +0x10) that is either an object or two shorts. */
typedef union Word8DA8E8 {
    void *object;
    int id;
    short half[2];
} Word8DA8E8;

/* What func_088DA8E8 is given: two words it copies as shorts. */
typedef struct Src8DA8E8 {
    char pad_00[0x24];
    unsigned int field_24;
    char pad_28[0x4];
    unsigned int field_2C;
} Src8DA8E8;
MAIN4_AT(Src8DA8E8, field_24, 0x24);
MAIN4_AT(Src8DA8E8, field_2C, 0x2C);

typedef int (*Callback8DB318)(void *arg, int value);

/* Functions at 0x088DA8E8..0x088DB634: a source at +0xC, flags at +0x14,
 * a callback and its argument at +0x1C / +0x20. */
typedef struct Obj8DA8E8 {
    char pad_00[0xC];
    Word8DA8E8 field_0C;
    Word8DA8E8 field_10;
    unsigned char flags;
    unsigned char field_15; /* 150 after a reset */
    unsigned char field_16;
    char pad_17[0x1];
    short field_18;
    short field_1A;
    Callback8DB318 callback;
    void *callback_arg;
} Obj8DA8E8;
MAIN4_SIZE(Obj8DA8E8, 0x24);
MAIN4_AT(Obj8DA8E8, field_0C, 0xC);
MAIN4_AT(Obj8DA8E8, field_10, 0x10);
MAIN4_AT(Obj8DA8E8, flags, 0x14);
MAIN4_AT(Obj8DA8E8, field_15, 0x15);
MAIN4_AT(Obj8DA8E8, field_16, 0x16);
MAIN4_AT(Obj8DA8E8, field_18, 0x18);
MAIN4_AT(Obj8DA8E8, field_1A, 0x1A);
MAIN4_AT(Obj8DA8E8, callback, 0x1C);
MAIN4_AT(Obj8DA8E8, callback_arg, 0x20);

/* Counters summed by func_088DC7A8 (0x088DC7A8..0x088DCEF8). */
typedef struct Obj8DC7A8 {
    char pad_00[0x3C];
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    char pad_54[0x4];
    int field_58; /* counted 16 times */
    char pad_5C[0x8];
    short field_64;
} Obj8DC7A8;
MAIN4_AT(Obj8DC7A8, field_3C, 0x3C);
MAIN4_AT(Obj8DC7A8, field_40, 0x40);
MAIN4_AT(Obj8DC7A8, field_44, 0x44);
MAIN4_AT(Obj8DC7A8, field_48, 0x48);
MAIN4_AT(Obj8DC7A8, field_4C, 0x4C);
MAIN4_AT(Obj8DC7A8, field_50, 0x50);
MAIN4_AT(Obj8DC7A8, field_58, 0x58);
MAIN4_AT(Obj8DC7A8, field_64, 0x64);

/* A short counter at +4. */
typedef struct Obj8DE198 {
    char pad_00[0x4];
    short count;
} Obj8DE198;
MAIN4_AT(Obj8DE198, count, 0x4);

/* ------------------------------------------------------------ 0x088DE25C..0x088DF7E4 */

/* Vtable D_089CC2A8 at +0x14; reset by func_088DE9C0, released by func_088DE9D8. */
typedef struct Obj9CC2A8 {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    void *vtable;
} Obj9CC2A8;
MAIN4_SIZE(Obj9CC2A8, 0x18);
MAIN4_AT(Obj9CC2A8, active, 0x0);
MAIN4_AT(Obj9CC2A8, field_04, 0x4);
MAIN4_AT(Obj9CC2A8, field_08, 0x8);
MAIN4_AT(Obj9CC2A8, field_0C, 0xC);
MAIN4_AT(Obj9CC2A8, field_10, 0x10);
MAIN4_AT(Obj9CC2A8, vtable, 0x14);

/* Vtable D_089CC2D0 at +0x18; reset by func_088DED08, released by func_088DED28. */
typedef struct Obj9CC2D0 {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    void *vtable;
} Obj9CC2D0;
MAIN4_SIZE(Obj9CC2D0, 0x1C);
MAIN4_AT(Obj9CC2D0, active, 0x0);
MAIN4_AT(Obj9CC2D0, field_04, 0x4);
MAIN4_AT(Obj9CC2D0, field_08, 0x8);
MAIN4_AT(Obj9CC2D0, field_0C, 0xC);
MAIN4_AT(Obj9CC2D0, field_10, 0x10);
MAIN4_AT(Obj9CC2D0, field_14, 0x14);
MAIN4_AT(Obj9CC2D0, vtable, 0x18);

/* Vtable D_089CC308 at +0x18; reset by func_088DF0DC, released by func_088DF100. */
typedef struct Obj9CC308 {
    unsigned char active;
    unsigned char field_01;
    char pad_02[0x2];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    char field_14;
    char pad_15[0x3];
    void *vtable;
} Obj9CC308;
MAIN4_SIZE(Obj9CC308, 0x1C);
MAIN4_AT(Obj9CC308, active, 0x0);
MAIN4_AT(Obj9CC308, field_01, 0x1);
MAIN4_AT(Obj9CC308, field_04, 0x4);
MAIN4_AT(Obj9CC308, field_08, 0x8);
MAIN4_AT(Obj9CC308, field_0C, 0xC);
MAIN4_AT(Obj9CC308, field_10, 0x10);
MAIN4_AT(Obj9CC308, field_14, 0x14);
MAIN4_AT(Obj9CC308, vtable, 0x18);

/* Vtable D_089CC330 at +0x1C; reset by func_088DF53C, released by func_088DF54C. */
typedef struct Obj9CC330 {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    void *handle; /* released with func_088DA060 */
    Obj9C9D30 field_0C;
    void *vtable;
} Obj9CC330;
MAIN4_SIZE(Obj9CC330, 0x20);
MAIN4_AT(Obj9CC330, active, 0x0);
MAIN4_AT(Obj9CC330, field_04, 0x4);
MAIN4_AT(Obj9CC330, handle, 0x8);
MAIN4_AT(Obj9CC330, field_0C, 0xC);
MAIN4_AT(Obj9CC330, vtable, 0x1C);

/* Vtable D_089CC290 at +0xA0; reset by func_088DE360, released by func_088DE378. */
typedef struct Obj9CC290 {
    unsigned char active;
    char pad_01[0x3];
    void *model; /* released with func_088DAB44 */
    char field_08;
    char pad_09[0x3];
    int state;
    int field_10;
    Obj9CC308 field_14;
    Obj9CC2A8 field_30;
    Obj9CC2D0 field_48[2];
    Obj9CC330 field_80;
    void *vtable;
} Obj9CC290;
MAIN4_SIZE(Obj9CC290, 0xA4);
MAIN4_AT(Obj9CC290, active, 0x0);
MAIN4_AT(Obj9CC290, model, 0x4);
MAIN4_AT(Obj9CC290, field_08, 0x8);
MAIN4_AT(Obj9CC290, state, 0xC);
MAIN4_AT(Obj9CC290, field_10, 0x10);
MAIN4_AT(Obj9CC290, field_14, 0x14);
MAIN4_AT(Obj9CC290, field_30, 0x30);
MAIN4_AT(Obj9CC290, field_48, 0x48);
MAIN4_AT(Obj9CC290, field_80, 0x80);
MAIN4_AT(Obj9CC290, vtable, 0xA0);

/* ------------------------------------------------------------ 0x088DFB90..0x088E6678 */

extern BlockRef *D_089D7E78;

/* Vtable D_089CC538 at +0x7BC; built by func_088E2A88. */
typedef struct Obj9CC538 {
    char pad_00[0x670];
    int field_670;
    int field_674;
    char pad_678[0x20];
    int field_698;
    int field_69C;
    int field_6A0;
    int field_6A4;
    char pad_6A8[0x114];
    void *vtable;
} Obj9CC538;
MAIN4_SIZE(Obj9CC538, 0x7C0);
MAIN4_AT(Obj9CC538, field_670, 0x670);
MAIN4_AT(Obj9CC538, field_674, 0x674);
MAIN4_AT(Obj9CC538, field_698, 0x698);
MAIN4_AT(Obj9CC538, field_69C, 0x69C);
MAIN4_AT(Obj9CC538, field_6A0, 0x6A0);
MAIN4_AT(Obj9CC538, field_6A4, 0x6A4);
MAIN4_AT(Obj9CC538, vtable, 0x7BC);

/* Vtable D_089CC610 at +0x80; its 32 words are cleared by func_088E5B38. */
typedef struct Obj9CC610 {
    int field_00[32];
    void *vtable;
} Obj9CC610;
MAIN4_SIZE(Obj9CC610, 0x84);
MAIN4_AT(Obj9CC610, field_00, 0x0);
MAIN4_AT(Obj9CC610, vtable, 0x80);

/* Vtable D_089CC710 at +0x24; built by func_088E6678. */
typedef struct Obj9CC710 {
    char pad_00[0x4];
    int field_04;
    int field_08;
    char pad_0C[0x4];
    int field_10;
    int field_14;
    char pad_18[0xC];
    void *vtable;
} Obj9CC710;
MAIN4_SIZE(Obj9CC710, 0x28);
MAIN4_AT(Obj9CC710, field_04, 0x4);
MAIN4_AT(Obj9CC710, field_08, 0x8);
MAIN4_AT(Obj9CC710, field_10, 0x10);
MAIN4_AT(Obj9CC710, field_14, 0x14);
MAIN4_AT(Obj9CC710, vtable, 0x24);

/* An element built by func_088E6D50 (four in each Obj8E6D78). */
typedef struct Elem8E6D50 {
    char field_00[0x20];
} Elem8E6D50;
MAIN4_SIZE(Elem8E6D50, 0x20);
MAIN4_AT(Elem8E6D50, field_00, 0x0);

/* Built by func_088E6D78 (four in each Obj9CC738). */
typedef struct Obj8E6D78 {
    char pad_00[0x70];
    Elem8E6D50 field_70[4];
    char pad_F0[0x10];
} Obj8E6D78;
MAIN4_SIZE(Obj8E6D78, 0x100);
MAIN4_AT(Obj8E6D78, field_70, 0x70);

/* Vtable D_089CC738 at +0x418; built by func_088E6DC0. */
typedef struct Obj9CC738 {
    char pad_00[0x10];
    Obj8E6D78 field_10[4];
    char pad_410[0x8];
    void *vtable;
} Obj9CC738;
MAIN4_SIZE(Obj9CC738, 0x41C);
MAIN4_AT(Obj9CC738, field_10, 0x10);
MAIN4_AT(Obj9CC738, vtable, 0x418);

/* Vtable D_089CC7A0 at +0x4C; built by func_088E81C0. */
typedef struct Obj9CC7A0 {
    char pad_00[0x4C];
    void *vtable;
} Obj9CC7A0;
MAIN4_SIZE(Obj9CC7A0, 0x50);
MAIN4_AT(Obj9CC7A0, vtable, 0x4C);

/* Built by func_088E86F0: a Holder, then six words. */
typedef struct Obj8E86F0 {
    Holder field_00;
    void *field_04; /* released with func_088A0CD4 */
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
} Obj8E86F0;
MAIN4_SIZE(Obj8E86F0, 0x1C);
MAIN4_AT(Obj8E86F0, field_00, 0x0);
MAIN4_AT(Obj8E86F0, field_04, 0x4);
MAIN4_AT(Obj8E86F0, field_08, 0x8);
MAIN4_AT(Obj8E86F0, field_0C, 0xC);
MAIN4_AT(Obj8E86F0, field_10, 0x10);
MAIN4_AT(Obj8E86F0, field_14, 0x14);
MAIN4_AT(Obj8E86F0, field_18, 0x18);

/* Vtable D_089CC9C8 at +0x20; func_088EA794 sets its eight words to -1. */
typedef struct Obj9CC9C8 {
    int field_00[8];
    void *vtable;
} Obj9CC9C8;
MAIN4_SIZE(Obj9CC9C8, 0x24);
MAIN4_AT(Obj9CC9C8, field_00, 0x0);
MAIN4_AT(Obj9CC9C8, vtable, 0x20);

/* An 8-byte entry of Obj9CC9E8. */
typedef struct Entry8EAF88 {
    unsigned char field_00;
    unsigned char field_01;
    char pad_02[0x6];
} Entry8EAF88;
MAIN4_SIZE(Entry8EAF88, 0x8);
MAIN4_AT(Entry8EAF88, field_00, 0x0);
MAIN4_AT(Entry8EAF88, field_01, 0x1);

/* Vtable D_089CC9E8 at +0x80; built by func_088EA9E8. */
typedef struct Obj9CC9E8 {
    char pad_00[0x30];
    Entry8EAF88 entries[9];
    int index;
    char pad_7C[0x4];
    void *vtable;
} Obj9CC9E8;
MAIN4_SIZE(Obj9CC9E8, 0x84);
MAIN4_AT(Obj9CC9E8, entries, 0x30);
MAIN4_AT(Obj9CC9E8, index, 0x78);
MAIN4_AT(Obj9CC9E8, vtable, 0x80);

/* Vtable D_089CC9A8 at +0x54; built by func_088EA2A0. */
typedef struct Obj9CC9A8 {
    int state;
    char pad_04[0x44];
    float field_48;
    char pad_4C[0x8];
    void *vtable;
} Obj9CC9A8;
MAIN4_SIZE(Obj9CC9A8, 0x58);
MAIN4_AT(Obj9CC9A8, state, 0x0);
MAIN4_AT(Obj9CC9A8, field_48, 0x48);
MAIN4_AT(Obj9CC9A8, vtable, 0x54);

/* A 0x40-byte entry of the table func_088FE944 fills. */
typedef struct Entry8E0714 {
    int field_00;
    char pad_04[0x3C];
} Entry8E0714;
MAIN4_SIZE(Entry8E0714, 0x40);
MAIN4_AT(Entry8E0714, field_00, 0x0);

/* What Obj9CC358's field_08 points to: 0x40-byte entries at +0x48C. */
typedef struct Table8E0714 {
    char pad_00[0x48C];
    Entry8E0714 entries[1];
} Table8E0714;
MAIN4_AT(Table8E0714, entries, 0x48C);

/* Vtable D_089CC358 at +0x17C4; built by func_088DFCF8. */
typedef struct Obj9CC358 {
    char pad_00[0x4];
    void *heap;
    Table8E0714 *field_08;
    char pad_0C[0x5B0];
    Obj9CC538 field_5BC;
    Obj9CC610 field_D7C;
    Obj9CC738 field_E00;
    char pad_121C[0x4];
    Obj9CC7A0 field_1220;
    Obj8E86F0 field_1270;
    Obj9CC9C8 field_128C;
    Obj9CC710 field_12B0;
    Obj9CC9E8 field_12D8;
    Obj9CC9A8 field_135C;
    char pad_13B4[0xC];
    char field_13C0[0x26C]; /* a track, built by func_08873880 (src/main/track_init_08873880.c) */
    void *field_162C;
    char pad_1630[0x50];
    char field_1680;
    char pad_1681[0x8B];
    void *blocks[20];
    int sizes[20];
    short count;
    char field_17AE;
    char pad_17AF[0xB];
    char field_17BA;
    char pad_17BB[0x2];
    char field_17BD;
    char pad_17BE[0x2];
    char field_17C0;
    char pad_17C1[0x3];
    void *vtable;
} Obj9CC358;
MAIN4_SIZE(Obj9CC358, 0x17C8);
MAIN4_AT(Obj9CC358, heap, 0x4);
MAIN4_AT(Obj9CC358, field_08, 0x8);
MAIN4_AT(Obj9CC358, field_5BC, 0x5BC);
MAIN4_AT(Obj9CC358, field_D7C, 0xD7C);
MAIN4_AT(Obj9CC358, field_E00, 0xE00);
MAIN4_AT(Obj9CC358, field_1220, 0x1220);
MAIN4_AT(Obj9CC358, field_1270, 0x1270);
MAIN4_AT(Obj9CC358, field_128C, 0x128C);
MAIN4_AT(Obj9CC358, field_12B0, 0x12B0);
MAIN4_AT(Obj9CC358, field_12D8, 0x12D8);
MAIN4_AT(Obj9CC358, field_135C, 0x135C);
MAIN4_AT(Obj9CC358, field_13C0, 0x13C0);
MAIN4_AT(Obj9CC358, field_162C, 0x162C);
MAIN4_AT(Obj9CC358, field_1680, 0x1680);
MAIN4_AT(Obj9CC358, blocks, 0x170C);
MAIN4_AT(Obj9CC358, sizes, 0x175C);
MAIN4_AT(Obj9CC358, count, 0x17AC);
MAIN4_AT(Obj9CC358, field_17AE, 0x17AE);
MAIN4_AT(Obj9CC358, field_17BA, 0x17BA);
MAIN4_AT(Obj9CC358, field_17BD, 0x17BD);
MAIN4_AT(Obj9CC358, field_17C0, 0x17C0);
MAIN4_AT(Obj9CC358, vtable, 0x17C4);

/* ------------------------------------------------------------ 0x088E6764..0x088E9BF4 */

extern void *D_089D6C30;
extern void *D_089D6AF0;
extern void *D_089D6AC8;

/* A callback block (0x34 bytes, as in include/main/callback_runtime.h):
 * callbacks set by func_0886503C, started by func_088650A4, polled by
 * func_0886514C and waited for by func_08865158. */
typedef void (*CallbackFn)(void);
typedef struct Callbacks34 {
    void *field_00;
    char pad_04[0x30];
} Callbacks34;
MAIN4_SIZE(Callbacks34, 0x34);
MAIN4_AT(Callbacks34, field_00, 0x0);

/* The object at *D_089D7E88 (functions 0x088E8DC4..0x088E9B68): a block,
 * a callback block, a name, an Obj9CC358 and a state. */
typedef struct Obj9D7E88 {
    void *block; /* freed with func_08882158 */
    Callbacks34 callbacks;
    char pad_38[0x4];
    char name[0x40];
    unsigned int field_7C;
    unsigned char field_80;
    char pad_81[0xF];
    Obj9CC358 field_90;
    char pad_1858[0x8];
    int state;
    int field_1864;
    int field_1868[3];
    char field_1874[0x27C]; /* built and polled by func_088D6F24 / func_088D7684 */
    char field_1AF0[0x14]; /* built and polled by func_088CE500 / func_088CE724 */
    char field_1B04;
    char pad_1B05[0x1];
    char field_1B06;
} Obj9D7E88;
MAIN4_AT(Obj9D7E88, block, 0x0);
MAIN4_AT(Obj9D7E88, callbacks, 0x4);
MAIN4_AT(Obj9D7E88, name, 0x3C);
MAIN4_AT(Obj9D7E88, field_7C, 0x7C);
MAIN4_AT(Obj9D7E88, field_80, 0x80);
MAIN4_AT(Obj9D7E88, field_90, 0x90);
MAIN4_AT(Obj9D7E88, state, 0x1860);
MAIN4_AT(Obj9D7E88, field_1864, 0x1864);
MAIN4_AT(Obj9D7E88, field_1868, 0x1868);
MAIN4_AT(Obj9D7E88, field_1874, 0x1874);
MAIN4_AT(Obj9D7E88, field_1AF0, 0x1AF0);
MAIN4_AT(Obj9D7E88, field_1B04, 0x1B04);
MAIN4_AT(Obj9D7E88, field_1B06, 0x1B06);
extern Obj9D7E88 *D_089D7E88;

/* A view of *D_089D5EC0 (declared void * in auto_region2.h). */
typedef struct View9D5EC0 {
    char pad_00[0x720];
    Obj85EA88 field_720;
    char pad_790[0x2541];
    char field_2CD1;
} View9D5EC0;
MAIN4_AT(View9D5EC0, field_720, 0x720);
MAIN4_AT(View9D5EC0, field_2CD1, 0x2CD1);
extern float D_09AF84C0[4];
extern float D_09AF8AB0[4];

/* ------------------------------------------------------------ 0x088EA3CC..0x088EFDC8 */

extern void *D_089D6C88;
extern int D_089D7EC8;

typedef struct Obj8EB29C {
    char pad_00[0x21];
    char field_21;
} Obj8EB29C;
MAIN4_AT(Obj8EB29C, field_21, 0x21);

/* Built by func_088EBFB8. */
typedef struct Obj8EBFB8 {
    char field_00[0x20];
    int field_20;
    int field_24;
    int field_28;
    char field_2C[0x24]; /* built by func_088EB0C0 */
    char field_50;
    char field_51;
    char field_52;
    char field_53;
} Obj8EBFB8;
MAIN4_SIZE(Obj8EBFB8, 0x54);
MAIN4_AT(Obj8EBFB8, field_00, 0x0);
MAIN4_AT(Obj8EBFB8, field_20, 0x20);
MAIN4_AT(Obj8EBFB8, field_24, 0x24);
MAIN4_AT(Obj8EBFB8, field_28, 0x28);
MAIN4_AT(Obj8EBFB8, field_2C, 0x2C);
MAIN4_AT(Obj8EBFB8, field_50, 0x50);
MAIN4_AT(Obj8EBFB8, field_51, 0x51);
MAIN4_AT(Obj8EBFB8, field_52, 0x52);
MAIN4_AT(Obj8EBFB8, field_53, 0x53);

/* Three model handles read by func_088DAA68 (functions 0x088EC278..0x088EC370). */
typedef struct Handles8EC278 {
    void *handles[3];
} Handles8EC278;
MAIN4_SIZE(Handles8EC278, 0xC);
MAIN4_AT(Handles8EC278, handles, 0x0);

/* Two model handles and three words (functions 0x088EC4C4..0x088EC6BC). */
typedef struct Handles8EC4C4 {
    void *handles[2];
    int field_08;
    int field_0C;
    int field_10;
} Handles8EC4C4;
MAIN4_SIZE(Handles8EC4C4, 0x14);
MAIN4_AT(Handles8EC4C4, handles, 0x0);
MAIN4_AT(Handles8EC4C4, field_08, 0x8);
MAIN4_AT(Handles8EC4C4, field_0C, 0xC);
MAIN4_AT(Handles8EC4C4, field_10, 0x10);

/* The base released by func_088EC234: a vtable at +0, then both handle sets. */
typedef struct Base8EC234 {
    void *vtable;
    Handles8EC4C4 field_04;
    Handles8EC278 field_18;
} Base8EC234;
MAIN4_SIZE(Base8EC234, 0x24);
MAIN4_AT(Base8EC234, vtable, 0x0);
MAIN4_AT(Base8EC234, field_04, 0x4);
MAIN4_AT(Base8EC234, field_18, 0x18);

/* What func_088EC72C / func_088EC994 read the handles from. */
typedef struct Src8EC72C {
    char pad_00[0x8];
    void *field_08;
    char pad_0C[0x10];
    void *field_1C;
} Src8EC72C;
MAIN4_AT(Src8EC72C, field_08, 0x8);
MAIN4_AT(Src8EC72C, field_1C, 0x1C);

/* Vtable D_089CCAD0 (functions 0x088EC72C..0x088EC8E8). */
typedef struct Obj9CCAD0 {
    Base8EC234 base;
    unsigned char field_24;
} Obj9CCAD0;
MAIN4_AT(Obj9CCAD0, base, 0x0);
MAIN4_AT(Obj9CCAD0, field_24, 0x24);

/* Vtable D_089CCB08 (functions 0x088EC994..0x088ECDB4). */
typedef struct Obj9CCB08 {
    Base8EC234 base;
    void *handle; /* released with func_088DA060 */
    void *text; /* released with func_0887BA5C */
    int field_2C;
} Obj9CCB08;
MAIN4_SIZE(Obj9CCB08, 0x30);
MAIN4_AT(Obj9CCB08, base, 0x0);
MAIN4_AT(Obj9CCB08, handle, 0x24);
MAIN4_AT(Obj9CCB08, text, 0x28);
MAIN4_AT(Obj9CCB08, field_2C, 0x2C);

/* Vtable D_089CCB48. */
typedef struct Obj9CCB48 {
    Base8EC234 base;
} Obj9CCB48;
MAIN4_AT(Obj9CCB48, base, 0x0);

/* The object at *D_089D7F00 (functions 0x088ECF10..0x088EE688). */
typedef struct Obj9D7F00 {
    char pad_00[0x1C];
    char field_1C[0x84];
    void *field_A0;
    void *field_A4;
    void *field_A8;
    char pad_AC[0x110];
    void *field_1BC;
    void *field_1C0;
    char pad_1C4[0x14];
    float field_1D8;
} Obj9D7F00;
MAIN4_AT(Obj9D7F00, field_1C, 0x1C);
MAIN4_AT(Obj9D7F00, field_A0, 0xA0);
MAIN4_AT(Obj9D7F00, field_A4, 0xA4);
MAIN4_AT(Obj9D7F00, field_A8, 0xA8);
MAIN4_AT(Obj9D7F00, field_1BC, 0x1BC);
MAIN4_AT(Obj9D7F00, field_1C0, 0x1C0);
MAIN4_AT(Obj9D7F00, field_1D8, 0x1D8);
extern Obj9D7F00 *D_089D7F00;

/* Three shorts, cleared with the rest of 16 bytes first. */
typedef struct Obj8EFDC8 {
    unsigned short field_00;
    unsigned short field_02;
    unsigned short field_04;
    char pad_06[0xA];
} Obj8EFDC8;
MAIN4_SIZE(Obj8EFDC8, 0x10);
MAIN4_AT(Obj8EFDC8, field_00, 0x0);
MAIN4_AT(Obj8EFDC8, field_02, 0x2);
MAIN4_AT(Obj8EFDC8, field_04, 0x4);

/* ------------------------------------------------------------ 0x088F0700..0x088F258C */

/* A view of the object at *D_089D712C: flag bytes past +0x1DE88. */
typedef struct Data9D712C {
    char pad_00[0xF58];
    char field_F58[1]; /* a table read by func_088CB164 and func_088F64DC */
    char pad_F59[0x1CEAB];
    char field_1DE04[0x78]; /* copied in and out by func_088F475C / func_088F472C */
    char pad_1DE7C[0xC];
    unsigned char field_1DE88;
    unsigned char field_1DE89;
    unsigned char field_1DE8A;
    char pad_1DE8B[0x4];
    unsigned char field_1DE8F;
    char pad_1DE90[0x48];
    unsigned char field_1DED8;
    char pad_1DED9[0x2];
    unsigned char field_1DEDB;
    unsigned char field_1DEDC; /* in tens */
    unsigned char field_1DEDD;
    char pad_1DEDE[0x1];
    unsigned char field_1DEDF;
    char pad_1DEE0[0x5];
    unsigned char field_1DEE5;
} Data9D712C;
MAIN4_AT(Data9D712C, field_F58, 0xF58);
MAIN4_AT(Data9D712C, field_1DE04, 0x1DE04);
MAIN4_AT(Data9D712C, field_1DE88, 0x1DE88);
MAIN4_AT(Data9D712C, field_1DE89, 0x1DE89);
MAIN4_AT(Data9D712C, field_1DE8A, 0x1DE8A);
MAIN4_AT(Data9D712C, field_1DE8F, 0x1DE8F);
MAIN4_AT(Data9D712C, field_1DED8, 0x1DED8);
MAIN4_AT(Data9D712C, field_1DEDB, 0x1DEDB);
MAIN4_AT(Data9D712C, field_1DEDC, 0x1DEDC);
MAIN4_AT(Data9D712C, field_1DEDD, 0x1DEDD);
MAIN4_AT(Data9D712C, field_1DEDF, 0x1DEDF);
MAIN4_AT(Data9D712C, field_1DEE5, 0x1DEE5);
extern Data9D712C *D_089D712C;

/* ------------------------------------------------------------ 0x088F27E4..0x088F2D24 */

extern int D_09B698D8;            /* a static object */

/* A record from func_088C0F8C. */
typedef struct Record8C0F8C {
    char pad_00[0xA];
    unsigned char flags;
} Record8C0F8C;
MAIN4_AT(Record8C0F8C, flags, 0xA);

/* Functions 0x088F2B84..0x088F2D24: four tables of 100 words and counters. */
typedef struct Obj8F2B84 {
    char pad_00[0x156C];
    int field_156C; /* a maximum (func_088F2BF4) */
    int field_1570;
    int field_1574;
    int field_1578;
    int field_157C[100];
    int field_170C[100];
    int field_189C[100];
    int field_1A2C[100];
    char field_1BBC;
    char field_1BBD;
    char pad_1BBE[0x2];
    short field_1BC0;
    short field_1BC2;
    int field_1BC4;
    int field_1BC8;
    int field_1BCC;
} Obj8F2B84;
MAIN4_AT(Obj8F2B84, field_156C, 0x156C);
MAIN4_AT(Obj8F2B84, field_1570, 0x1570);
MAIN4_AT(Obj8F2B84, field_1574, 0x1574);
MAIN4_AT(Obj8F2B84, field_1578, 0x1578);
MAIN4_AT(Obj8F2B84, field_157C, 0x157C);
MAIN4_AT(Obj8F2B84, field_170C, 0x170C);
MAIN4_AT(Obj8F2B84, field_189C, 0x189C);
MAIN4_AT(Obj8F2B84, field_1A2C, 0x1A2C);
MAIN4_AT(Obj8F2B84, field_1BBC, 0x1BBC);
MAIN4_AT(Obj8F2B84, field_1BBD, 0x1BBD);
MAIN4_AT(Obj8F2B84, field_1BC0, 0x1BC0);
MAIN4_AT(Obj8F2B84, field_1BC2, 0x1BC2);
MAIN4_AT(Obj8F2B84, field_1BC4, 0x1BC4);
MAIN4_AT(Obj8F2B84, field_1BC8, 0x1BC8);
MAIN4_AT(Obj8F2B84, field_1BCC, 0x1BCC);

/* A record from func_088CB164. */
typedef struct Record8CB164 {
    char pad_00[0x6];
    short field_06;
    char pad_08[0x4];
    int field_0C;
} Record8CB164;
MAIN4_AT(Record8CB164, field_06, 0x6);
MAIN4_AT(Record8CB164, field_0C, 0xC);

/* ------------------------------------------------------------ 0x088F4AEC..0x088F676C */

/* Twelve bytes: a word and two byte runs (func_088F4B24). */
typedef struct Obj8F4B24 {
    int field_00;
    unsigned char field_04[6];
    unsigned char field_0A[2];
} Obj8F4B24;
MAIN4_SIZE(Obj8F4B24, 0xC);
MAIN4_AT(Obj8F4B24, field_00, 0x0);
MAIN4_AT(Obj8F4B24, field_04, 0x4);
MAIN4_AT(Obj8F4B24, field_0A, 0xA);

/* Functions 0x088F4C4C..0x088F4DE8: six id bytes and two packed dates
 * (year << 20 | month << 16 | day << 11 | hour << 6 | minute). */
typedef struct Obj8F4C4C {
    char pad_00[0xC];
    int field_0C;
    unsigned char id[6];
    char pad_16[0x2];
    unsigned int date_18;
    unsigned int date_1C;
} Obj8F4C4C;
MAIN4_AT(Obj8F4C4C, field_0C, 0xC);
MAIN4_AT(Obj8F4C4C, id, 0x10);
MAIN4_AT(Obj8F4C4C, date_18, 0x18);
MAIN4_AT(Obj8F4C4C, date_1C, 0x1C);

typedef struct Obj8F4EE8 {
    char pad_00[0x2D4];
    int field_2D4;
} Obj8F4EE8;
MAIN4_AT(Obj8F4EE8, field_2D4, 0x2D4);

/* Eight shorts (func_088F4F90); func_088F5804 sets the first byte. */
typedef union Block8F4F90 {
    short values[8];
    unsigned char first;
} Block8F4F90;

/* Four shorts (func_088F4FB4). */
typedef struct Shorts4 {
    short values[4];
} Shorts4;
MAIN4_SIZE(Shorts4, 0x8);
MAIN4_AT(Shorts4, values, 0x0);

/* A 0x10A8-byte record (func_088F4FC8 clears it; func_088F54DC and
 * func_088F56B0 index arrays of them). */
typedef struct Rec8F4FC8 {
    char pad_00[0x27];
    unsigned char flags;
    char pad_28[0xBC];
    Block8F4F90 field_E4;
    Shorts4 field_F4;
    char field_FC[0xE58];
    char field_F54[0x154]; /* built by func_0892A13C */
} Rec8F4FC8;
MAIN4_SIZE(Rec8F4FC8, 0x10A8);
MAIN4_AT(Rec8F4FC8, flags, 0x27);
MAIN4_AT(Rec8F4FC8, field_E4, 0xE4);
MAIN4_AT(Rec8F4FC8, field_F4, 0xF4);
MAIN4_AT(Rec8F4FC8, field_FC, 0xFC);
MAIN4_AT(Rec8F4FC8, field_F54, 0xF54);

extern int D_089D7F30;
extern int D_089D7F34;

/* ------------------------------------------------------------ 0x088F69F8..0x088FB9F4 */

extern void *D_089D8364;

/* A stack object whose only field is its vtable D_089CCDB8. */
typedef struct Obj9CCDB8 {
    void *vtable;
} Obj9CCDB8;
MAIN4_SIZE(Obj9CCDB8, 0x4);
MAIN4_AT(Obj9CCDB8, vtable, 0x0);
extern int D_089CCDB8;

extern char (*D_089D7F40)[5];   /* rows of five cells */
extern int D_089D7F44;

/* A 0x1270-byte element of Obj8F7920's table at +0x14C80; func_088F5198
 * treats it as a Rec8F4FC8. */
typedef struct Elem8F7CD4 {
    Rec8F4FC8 record;
    char pad_10A8[0x1C8];
} Elem8F7CD4;
MAIN4_SIZE(Elem8F7CD4, 0x1270);
MAIN4_AT(Elem8F7CD4, record, 0x0);

typedef struct Obj8F7920 {
    char pad_00[0x14C80];
    Elem8F7CD4 field_14C80[1];
    char pad_15EF0[0x1BA7C];
    int field_3196C;
} Obj8F7920;
MAIN4_AT(Obj8F7920, field_14C80, 0x14C80);
MAIN4_AT(Obj8F7920, field_3196C, 0x3196C);

/* Forwarders 0x088F848C..0x088F855C: an object at +4. */
typedef struct Obj8F848C {
    char pad_00[0x4];
    void *field_04;
} Obj8F848C;
MAIN4_AT(Obj8F848C, field_04, 0x4);

/* Built by func_088F9388 (two at D_09AFB8A0). */
typedef struct Obj8F9388 {
    char pad_00[0xCE4];
    int field_CE4[20];
    int field_D34[128];
    char pad_F34[0x12C];
} Obj8F9388;
MAIN4_SIZE(Obj8F9388, 0x1060);
MAIN4_AT(Obj8F9388, field_CE4, 0xCE4);
MAIN4_AT(Obj8F9388, field_D34, 0xD34);
extern Obj8F9388 D_09AFB8A0[2];

/* ------------------------------------------------------------ 0x088FBBE0..0x088FCCE0 */

/* A 0x1C-byte row of Work8FBBE0. */
typedef struct Row8FC3C4 {
    char pad_00[0x2];
    short field_02;
    char pad_04[0x18];
} Row8FC3C4;
MAIN4_SIZE(Row8FC3C4, 0x1C);
MAIN4_AT(Row8FC3C4, field_02, 0x2);

/* The work block of Obj8FBBE0: flags, rows, the layout and handles the
 * scripts play, and texts. */
typedef struct Work8FBBE0 {
    unsigned int flags;
    char pad_04[0x4];
    int state;
    char pad_0C[0x6];
    short field_12;
    char pad_14[0x8];
    Row8FC3C4 rows[1];
    char pad_38[0x624];
    int layout[0x100];
    void *handles[0x125];
    void *texts[0x1D];
    char pad_F64[0x140];
    int field_10A4;
    char pad_10A8[0x8];
    int field_10B0;
    char pad_10B4[0x60];
    int text_count; /* texts handed to func_0887BA80 and not yet done */
} Work8FBBE0;
MAIN4_AT(Work8FBBE0, flags, 0x0);
MAIN4_AT(Work8FBBE0, state, 0x8);
MAIN4_AT(Work8FBBE0, field_12, 0x12);
MAIN4_AT(Work8FBBE0, rows, 0x1C);
MAIN4_AT(Work8FBBE0, layout, 0x65C);
MAIN4_AT(Work8FBBE0, handles, 0xA5C);
MAIN4_AT(Work8FBBE0, texts, 0xEF0);
MAIN4_AT(Work8FBBE0, field_10A4, 0x10A4);
MAIN4_AT(Work8FBBE0, field_10B0, 0x10B0);
MAIN4_AT(Work8FBBE0, text_count, 0x1114);

typedef struct Obj8FBBE0 {
    char pad_00[0x30];
    Work8FBBE0 *work;
} Obj8FBBE0;
MAIN4_AT(Obj8FBBE0, work, 0x30);
extern Obj8FBBE0 *D_089D7F90;
extern const unsigned short D_089CCE20[];
extern const unsigned short D_089CCE28[];
extern const unsigned short D_089CCE38[];
extern const unsigned short D_089CCE9C[];

/* ------------------------------------------------------------ 0x088FE9AC..0x088FFDA0 */

extern int D_089D7F98;
extern int D_089D812C;
extern int D_089D8134;
typedef int (*Callback8FFAB8)(int arg1, int arg2);

typedef struct Obj9D8128 {
    char pad_00[0x3E04];
    void *field_3E04;
} Obj9D8128;
MAIN4_AT(Obj9D8128, field_3E04, 0x3E04);
extern Obj9D8128 *D_089D8128;

/* ------------------------------------------------------------ prototypes */

void func_08844588(Table9D4960 *table);
void *func_0885C6CC(void *heap, int size, int align, int arg3, int arg4, char arg5);
void func_0885E860(Obj85EA88 *self);
void func_0886503C(Callbacks34 *block, CallbackFn first, CallbackFn second, CallbackFn third);
void func_088650A4(Callbacks34 *block);
unsigned char func_0886514C(Callbacks34 *block);
int func_0886658C(int id);
void *func_08873880(void *self);
int func_0887BA80(void *text, void (*done)(void *text), int arg);
void func_0887E630(void *manager, int text, int id);
void func_088837EC(void);
void func_08883850(void);
unsigned char func_08893CF8(void *manager);
void func_08896AB0(void *manager);
void func_0889F2B8(void *manager);
void func_0889F340(void *manager, int arg);
void func_088A015C(void);
void func_088A0CD4(void *object);
void func_088A0D1C(void);
int func_088A1638(void *manager, int id);
void func_088A2780(void);
short func_088B6BA0(int index);
char func_088B6BE0(int index);
void func_088C01C0(int *value);
Record8C0F8C *func_088C0F8C(int id);
short func_088C50B0(void);
char func_088C6A7C(void);
short func_088C6AC8();
Record8CB164 *func_088CB164(void *table, int id);
int func_088CB9E8();
char func_088CBAD0(void *object);
int func_088CE500(void *self, void *owner, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9);
void *func_088CE724(void *self);
void func_088CFA94(void *manager, int arg, int value);
void func_088D3B88(void *layout, void *handles, const unsigned short *script, int arg, int count);
void func_088D3C24(void *layout, void *handles, const unsigned short *script, int count);
void func_088D3CF0(void *layout, void *handles, const unsigned short *script);
void func_088D3EA0(void *layout, void *handles, const unsigned short *script);
void func_088D3F00(void *layout, void *handles, const unsigned short *script);
int func_088D6F24(void *self, void *owner, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10);
void *func_088D7684(void *self);
void func_088D96E4(Obj9CC250 *self, unsigned int flags);
void func_088D973C(Obj9CC250 *self);
void func_088D9750(Obj9CC250 *self);
void func_088D9CFC(void *handle);
void func_088D9F5C(void *handle, int motion, char arg);
void func_088DA0A0(void);
void func_088DA8E8(Obj8DA8E8 *self, Src8DA8E8 *source);
void func_088DA924(Obj8DA8E8 *self, void *object);
void func_088DA92C(Obj8DA8E8 *self);
void *func_088DAA68(void *source);
void func_088DB21C(Obj8DA8E8 *self, int id, short high, short low);
void func_088DB254(Obj8DA8E8 *self, int id, short high, short low);
void func_088DB30C(Obj8DA8E8 *self, Callback8DB318 callback, void *arg);
void func_088DB318(Obj8DA8E8 *self, short value);
void func_088DB618(Obj8DA8E8 *self);
void func_088DB634(Obj8DA8E8 *self);
int func_088DC7A8(Obj8DC7A8 *self);
void func_088DCD50(Obj8DC7A8 *self);
void func_088DCE6C(void *object);
int func_088DCEF8(Obj8DC7A8 *self);
void func_088DDE4C(void *object);
void func_088DE198(Obj8DE198 *self);
Obj9CC290 *func_088DE25C(Obj9CC290 *self);
void func_088DE2C4(Obj9CC290 *self, unsigned int flags);
void func_088DE360(Obj9CC290 *self);
void func_088DE378(Obj9CC290 *self);
unsigned char func_088DE6CC(Obj9CC290 *self);
void func_088DE7B0(Obj9CC290 *self);
void func_088DE7CC(Obj9CC290 *self);
void func_088DE7E8(Obj9CC290 *self);
int func_088DE87C(Obj9CC290 *self);
int func_088DE8B8(Obj9CC290 *self);
int func_088DE8F4(Obj9CC290 *self);
Obj9CC2A8 *func_088DE930(Obj9CC2A8 *self);
void func_088DE968(Obj9CC2A8 *self, unsigned int flags);
void func_088DE9C0(Obj9CC2A8 *self);
void func_088DE9D8(Obj9CC2A8 *self);
unsigned char func_088DEC0C(Obj9CC2A8 *self);
Obj9CC2D0 *func_088DEC78(Obj9CC2D0 *self);
void func_088DECB0(Obj9CC2D0 *self, unsigned int flags);
void func_088DED08(Obj9CC2D0 *self);
void func_088DED28(Obj9CC2D0 *self);
unsigned char func_088DEF74(Obj9CC2D0 *self);
Obj9CC308 *func_088DF04C(Obj9CC308 *self);
void func_088DF084(Obj9CC308 *self, unsigned int flags);
void func_088DF0DC(Obj9CC308 *self);
void func_088DF100(Obj9CC308 *self);
unsigned int func_088DF11C(Obj9CC308 *self, unsigned char value);
unsigned char func_088DF2B8(Obj9CC308 *self);
int func_088DF2C0(Obj9CC308 *self);
int func_088DF2F8(Obj9CC308 *self);
int func_088DF3F8(Obj9CC308 *self);
int func_088DF43C(Obj9CC308 *self);
Obj9CC330 *func_088DF498(Obj9CC330 *self);
void func_088DF4D8(Obj9CC330 *self, unsigned int flags);
void func_088DF53C(Obj9CC330 *self);
void func_088DF54C(Obj9CC330 *self);
void func_088DF590(Obj9CC330 *self);
unsigned char func_088DF7E4(Obj9CC330 *self);
unsigned char func_088DF824(Obj9CC330 *self);
int func_088DFAC8(Obj9CC330 *self);
BlockRef *func_088DFB90(BlockRef *self);
void func_088DFBF8(BlockRef *self);
void *func_088DFC30(void);
void func_088DFC78(void *block);
Obj9CC358 *func_088DFCF8(Obj9CC358 *self);
void func_088DFE58(Obj9CC358 *self, int arg);
void func_088E0078(Obj9CC358 *self);
void func_088E0094(Obj9CC358 *self);
void func_088E0714(int size, Obj9CC358 *self);
Obj9CC538 *func_088E2A88(Obj9CC538 *self);
void func_088E2BFC(Obj9CC538 *self);
void func_088E2DD4(Obj9CC538 *self);
void func_088E33E8(Obj9CC538 *self);
void func_088E37D4(Obj9CC538 *self);
void func_088E39D4(Obj9CC538 *self, int arg1, int arg2);
Obj9CC610 *func_088E5AC8(Obj9CC610 *self);
void func_088E5B38(Obj9CC610 *self, int arg);
Obj9CC710 *func_088E6678(Obj9CC710 *self);
void func_088E66E8(Obj9CC710 *self, int arg);
void func_088E6764(Obj9CC710 *self);
void func_088E67DC(Obj9CC710 *self);
void func_088E6BEC(Obj9CC710 *self, int value);
Elem8E6D50 *func_088E6D50(Elem8E6D50 *self);
Obj8E6D78 *func_088E6D78(Obj8E6D78 *self);
Obj9CC738 *func_088E6DC0(Obj9CC738 *self);
void func_088E6E4C(Obj9CC738 *self, int arg1, int arg2);
void func_088E6F0C(void);
void func_088E7088(Obj8E6D78 *self, int arg);
void func_088E715C(Elem8E6D50 *self);
Obj9CC7A0 *func_088E81C0(Obj9CC7A0 *self);
void func_088E8230(Obj9CC7A0 *self, int arg);
Obj8E86F0 *func_088E86F0(Obj8E86F0 *self);
void func_088E8730(Obj8E86F0 *self, int value);
void func_088E8738(Obj8E86F0 *self);
void func_088E8914(Obj8E86F0 *self, int arg1, int arg2);
void func_088E8CFC(void);
void func_088E8D1C(void);
void func_088E8DC4(Obj9D7E88 *self);
void func_088E9008(Obj9D7E88 *self);
void func_088E913C(Obj9D7E88 *self);
void func_088E91C4(Obj9D7E88 *self);
void func_088E91CC(Obj9D7E88 *self);
void func_088E9228(Obj9D7E88 *self);
void func_088E93EC(Obj9D7E88 *self);
void func_088E9474(Obj9D7E88 *self);
void func_088E9704(Obj9D7E88 *self, int arg);
void func_088E979C(Obj9D7E88 *self, const char *name, int value);
void func_088E980C(const char *name, int value);
void func_088E9848(Obj9D7E88 *self, unsigned int arg1, unsigned char arg2, unsigned int value);
void func_088E98B4(unsigned int arg1, unsigned char arg2, unsigned int value);
void func_088E98F4(void);
void func_088E9908(void);
void func_088E991C(int index, int value);
void func_088E9934(void);
void func_088E9968(void);
void func_088E99DC(void);
void func_088E99FC(void);
void func_088E9A2C(void);
void func_088E9A4C(void);
void func_088E9A6C(void);
void func_088E9A90(void);
void func_088E9B68(void);
void func_088E9B94(void);
void func_088E9BB4(void);
void func_088E9BD4(void);
void func_088E9BF4(void);
Obj9CC9A8 *func_088EA2A0(Obj9CC9A8 *self);
void func_088EA310(Obj9CC9A8 *self, int arg);
void func_088EA3CC(void);
void func_088EA4A4(Obj9CC9A8 *self);
void func_088EA4D8(Obj9CC9A8 *self);
void func_088EA504(Obj9CC9A8 *self);
unsigned char func_088EA530(Obj9CC9A8 *self);
Obj9CC9C8 *func_088EA728(Obj9CC9C8 *self);
void func_088EA794(Obj9CC9C8 *self);
void func_088EA868(void);
Obj9CC9E8 *func_088EA9E8(Obj9CC9E8 *self);
void func_088EAA58(Obj9CC9E8 *self, char arg);
void func_088EAC08(void);
unsigned char func_088EAF88(Obj9CC9E8 *self);
void *func_088EB0C0(void *self);
void func_088EB29C(Obj8EB29C *self);
void func_088EB370(void);
void func_088EB3C4(Obj8EB29C *self);
void func_088EBA98(void);
Obj8EBFB8 *func_088EBFB8(Obj8EBFB8 *self);
int func_088EC010(void);
void func_088EC218(void);
void func_088EC234(Base8EC234 *self, unsigned int flags);
Handles8EC278 *func_088EC278(Handles8EC278 *self);
void func_088EC2A8(Handles8EC278 *self, void *source);
void func_088EC2F8(Handles8EC278 *self);
void func_088EC34C(Handles8EC278 *self);
void func_088EC370(Handles8EC278 *self);
Handles8EC4C4 *func_088EC4C4(Handles8EC4C4 *self);
void func_088EC500(Handles8EC4C4 *self, void *source);
void func_088EC544(Handles8EC4C4 *self);
void func_088EC598(Handles8EC4C4 *self);
void func_088EC634(Handles8EC4C4 *self);
void func_088EC658(Handles8EC4C4 *self);
void func_088EC67C(Handles8EC4C4 *self);
void func_088EC6BC(Handles8EC4C4 *self, int arg1, int arg2);
void func_088EC72C(Obj9CCAD0 *self, Src8EC72C *source);
void func_088EC770(Obj9CCAD0 *self);
unsigned char func_088EC88C(Obj9CCAD0 *self);
void func_088EC8C0(Obj9CCAD0 *self);
void func_088EC8CC(Obj9CCAD0 *self);
void func_088EC8E8(Obj9CCAD0 *self, unsigned int flags);
void func_088EC994(Obj9CCB08 *self, Src8EC72C *source);
void func_088EC9D8(Obj9CCB08 *self);
void func_088ECAD4(Obj9CCB08 *self);
void func_088ECC9C(void);
void func_088ECD18(void);
void func_088ECD7C(void);
void func_088ECD98(void);
void func_088ECDB4(Obj9CCB08 *self, unsigned int flags);
void func_088ECE54(Obj9CCB48 *self, unsigned int flags);
void func_088ECF10(void);
int func_088ED208(void);
int func_088ED26C(void);
void func_088ED2EC(void);
void func_088ED318(void);
void func_088ED340(void);
void func_088EDA0C(void);
void func_088EE58C(void);
void func_088EE5B8(void);
float func_088EE688(void);
int func_088EF1E0(void);
int func_088EFCD0(void);
void func_088EFDAC(void);
void func_088EFDC8(Obj8EFDC8 *self, unsigned short arg1, unsigned short arg2, unsigned short arg3);
void *func_088F0700(void *self);
void func_088F0734(void);
float func_088F0C18(int index);
unsigned char func_088F0C80(void);
void func_088F0C98(char arg);
void func_088F0D2C(void);
unsigned char func_088F1094(void);
unsigned char func_088F1258(void);
void func_088F1270(char arg);
void func_088F12F4(int index, char arg);
unsigned char func_088F1444(void);
void func_088F145C(char arg);
unsigned char func_088F1474(void);
void func_088F1E28(unsigned int arg);
void *func_088F2220(void *self);
void func_088F2254(void);
void func_088F2300(int arg);
void func_088F2324(void);
void func_088F258C(unsigned char value);
unsigned char func_088F27E4(void);
void func_088F27FC(void);
int func_088F2818(void);
unsigned char func_088F2854(void);
void func_088F286C(void);
void func_088F2888(void);
unsigned char func_088F29D0(void);
void func_088F2A78(Obj8F848C *self);
void func_088F2B84(Obj8F2B84 *self, int id);
int func_088F2BD4(Obj8F2B84 *self);
int func_088F2BEC(Obj8F2B84 *self);
void func_088F2BF4(Obj8F2B84 *self, int value);
void func_088F2C28(Obj8F2B84 *self);
void func_088F2CF8(Obj8F2B84 *self);
void func_088F2D24(Obj8F2B84 *self);
void func_088F2DD8(Obj8F2B84 *self, int index);
void func_088F34B0(Obj8F2B84 *self, int index);
void func_088F3B18(void);
void func_088F3DD8(Obj8F2B84 *self, unsigned int arg);
void func_088F3E38(Obj8F2B84 *self, unsigned int arg);
int func_088F414C(void *self, int id, char arg);
void func_088F42A8(void *self);
void *func_088F43A8(void *self);
void func_088F472C(void *buffer);
void func_088F475C(void *buffer);
void func_088F47C4(void);
unsigned int func_088F4AC8(int year, int month, int day, int hour, int minute);
void func_088F4AEC(unsigned int *year, unsigned int *month, unsigned int *day, unsigned int *hour, unsigned int *minute, unsigned int date);
void func_088F4B24(Obj8F4B24 *self);
void func_088F4B68();
void func_088F4BD8(Rec8F4FC8 *self);
void func_088F4C4C(Obj8F4C4C *self);
void func_088F4CB4(Obj8F4C4C *self, int year, int month, int day, int hour, int minute);
void func_088F4D34(Obj8F4C4C *self, int year, int month, int day, int hour, int minute);
void func_088F4DB4(Obj8F4C4C *self, unsigned int *year, unsigned int *month, unsigned int *day, unsigned int *hour, unsigned int *minute);
void func_088F4DE8(Obj8F4C4C *self, unsigned int *year, unsigned int *month, unsigned int *day, unsigned int *hour, unsigned int *minute);
void func_088F4E3C();
void func_088F4EE8(Obj8F4EE8 *self);
void func_088F4F14(Obj8F4EE8 *self);
void func_088F4F90(Block8F4F90 *self);
void func_088F4FB4(Shorts4 *self);
void func_088F4FC8(Rec8F4FC8 *self);
void func_088F5014();
void func_088F5148(Rec8F4FC8 *self, Rec8F4FC8 *other);
void func_088F5188(Rec8F4FC8 *self);
void func_088F5198(Rec8F4FC8 *self);
void func_088F520C(Shorts4 *self);
int func_088F54DC(Rec8F4FC8 *records, int index);
void func_088F56B0(Rec8F4FC8 *records, int index);
void func_088F5790();
void func_088F5804(Rec8F4FC8 *self);
void func_088F5844(Rec8F4FC8 *self);
int func_088F5D40();
void func_088F5E80(Obj8F4C4C *self, Obj8F4C4C *other);
int func_088F5EA0(int *a, int *b);
void func_088F5FC0(int arg1, int arg2);
unsigned char func_088F64DC(char *result, char *data, void *table, Rec8F4FC8 *record);
void func_088F676C(void *unused, Rec8F4FC8 *record, short arg1, short arg2);
void func_088F69F8(void);
void func_088F6A24(int arg);
void func_088F6FB0(void);
int func_088F7030(int row, int column);
unsigned char func_088F7050(int row, int column);
void func_088F7354(int arg);
void func_088F7398(int arg);
void func_088F73DC(void);
void func_088F7418(void);
void func_088F7454(void);
void func_088F7490(void);
void func_088F74CC(void);
void func_088F7508(void);
void func_088F7524(void);
void func_088F7540(void *unused, int arg);
void func_088F755C(void *unused, int arg);
void func_088F7578(void);
void func_088F7594(void);
void func_088F75B0(void);
void func_088F7920(Obj8F7920 *self);
void func_088F795C(Obj8F7920 *self);
void func_088F7B4C(void);
void func_088F7B68(void);
void func_088F7B84(void);
int func_088F7CD4(Obj8F7920 *self, int index);
void func_088F80A8(void *object);
void func_088F80E0(void *object);
void func_088F8118(void *object);
void func_088F8254(void *object);
void func_088F828C(void *object);
void func_088F82C4(void *object);
void func_088F82FC(void *object, unsigned int arg1, unsigned int arg2, unsigned char arg3);
void func_088F8454(void *object);
void func_088F848C(Obj8F848C *self);
void func_088F84A8(Obj8F848C *self);
void func_088F84C4(Obj8F848C *self);
void func_088F84E0(Obj8F848C *self);
void func_088F84FC(Obj8F848C *self);
void func_088F8518(Obj8F848C *self);
void func_088F853C(Obj8F848C *self, unsigned int arg1, unsigned int arg2, unsigned char arg3);
void func_088F855C(Obj8F848C *self);
void func_088F8984();
Obj8F9388 *func_088F9388(Obj8F9388 *self);
void func_088FA5F4(int arg1, short *arg2, unsigned int arg3);
void func_088FA98C(int arg1, short *arg2, unsigned char arg3);
void func_088FB9F4(void);
void func_088FBA24(Obj8FBBE0 *self);
void func_088FBBE0(Obj8FBBE0 *self);
void func_088FBC08();
void func_088FBCC0(Obj8FBBE0 *self);
void func_088FBE60(Obj8FBBE0 *self);
void func_088FBF0C(Obj8FBBE0 *self);
void func_088FBF34(Obj8FBBE0 *self);
void func_088FBF5C(Obj8FBBE0 *self);
void func_088FBF8C(Obj8FBBE0 *self);
void func_088FC070(Obj8FBBE0 *self);
void func_088FC0BC(Obj8FBBE0 *self);
void func_088FC15C(Obj8FBBE0 *self);
void func_088FC184(Obj8FBBE0 *self);
void func_088FC194(Obj8FBBE0 *self);
void func_088FC220(Obj8FBBE0 *self);
void func_088FC298(Obj8FBBE0 *self);
void func_088FC2DC(Obj8FBBE0 *self);
void func_088FC364(Obj8FBBE0 *self);
void func_088FC3C4(Obj8FBBE0 *self, int index);
void func_088FC588(Obj8FBBE0 *self);
void func_088FC684(Obj8FBBE0 *self, short value, unsigned short arg);
void func_088FC87C(Obj8FBBE0 *self, void *text);
void func_088FC8CC(Obj8FBBE0 *self, void *text);
void *func_088FC904(void *self);
void func_088FCBEC(Obj8FBBE0 *self, int arg);
void func_088FCCE0(void *text);
void func_088FD314();
void func_088FE944(void *entry, void *data, int size);
void func_088FE9AC(void);
void func_088FEA74(void);
int func_088FF78C(int value);
void func_088FF84C(void);
void func_088FF868(void);
void func_088FF894(void);
void func_088FF8C8(void);
int func_088FF908(void);
int func_088FFAB8(int arg1, int arg2);
int func_088FFDA0(int arg);
void func_0890043C(int arg);
unsigned char func_08906F6C();
void func_089072DC(void *object, int arg);
void func_089097B8();
void func_089097E4();
unsigned char func_08909ABC();
unsigned char func_08909B04();
unsigned char func_08909B60();
void func_0890EA18(int id);
int func_0891A44C(void *object);
void func_0891B8C0(void *manager, Obj9CCDB8 *object);
void func_0891B96C(void *manager, int arg1, int arg2);
void func_0892A13C(void *self);
int func_08935490(const void *a, const void *b, unsigned int size);   /* memcmp */
char *func_089359A4(char *dst, const char *src);     /* strcpy */
int func_089359E8(const char *text);                 /* strlen */
void func_0899014C(Callback8FFAB8 callback);
void func_0899015C(void);
void func_089C5918(unsigned char *id);
void func_089C5B20(void *player);
void func_089C5B28(void *player, void *entry);
void func_089C5D98(void);
void func_089C5FC0(void *handle, int arg);
void func_089C5FD8(void *handle, int arg1, int arg2);
int func_08B53454(int arg);
int func_08B5354C(int arg);
int func_08B535C4(int arg);
int func_08B5363C(int arg);

extern int D_08B54330;

#ifdef __cplusplus
}
#endif

#endif /* DISSIDIA_MAIN_AUTO_REGION4_H */
