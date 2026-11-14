#ifndef DISSIDIA_BATTLE_AUTO_REGION3_H
#define DISSIDIA_BATTLE_AUTO_REGION3_H

/* Classes behind the automatically matched functions in battle
 * 0x08A15144..0x08A80884 (readability pass, region 3).
 *
 * Same conventions as auto_region1.h and auto_region2.h, which this header
 * builds on: "Part" structs keep their vtable pointer after their data,
 * "Unit" structs derive from the class built by func_08A157D0 (Unit158B8) or
 * func_089F12F0 (UnitF13D8) and keep the vtable at offset 0, and each struct is
 * named after the address of its vtable (or, without one, of its first
 * function). A part's first byte is cleared by its reset and tested first by
 * its getters, so it is called `active`. Objects of classes outside the
 * region are byte arrays named after the function that builds them.
 *
 * The units here reset field_08 to -1; their "step" virtuals move field_08 to
 * the next phase once a member reports that it has finished. Parts with a
 * phase of their own (Part17C90, Part25AD8, Part268C8) keep it in field_04.
 *
 * The fighter callbacks (BattleFighter) take the state object first and the
 * fighter second; the fighter keeps the current state's data at field_3A8. */

#include "battle/auto_region2.h"

#ifdef __cplusplus
extern "C" {
#endif

#define REGION3_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define REGION3_ASSERT(name, cond) typedef char region3_assert_##name[(cond) ? 1 : -1]
#define REGION3_SIZE(type, size) REGION3_ASSERT(size_##type, sizeof(type) == (size))
#define REGION3_AT(type, field, offset) \
    REGION3_ASSERT(at_##type##_##field, REGION3_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ vectors */

/* Cleared by func_089A34E0. */
typedef struct BattleVec3 {
    float x;
    float y;
    float z;
} BattleVec3;
REGION3_SIZE(BattleVec3, 0xC);

/* Copied by func_089A3868, combined by func_089A3B30. */
typedef struct BattleVec4 {
    float x;
    float y;
    float z;
    float w;
} BattleVec4;
REGION3_SIZE(BattleVec4, 0x10);

/* ------------------------------------------------------------ parts */

/* 0x0C bytes. Reset func_08A155D0, release func_08A155DC frees field_04. */
typedef struct Part157B8 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    void *vtable;
} Part157B8;
REGION3_SIZE(Part157B8, 0xC);

/* 0x08 bytes. Reset func_08A159F8, release func_08A15A04 calls
 * func_08A15AF8 while field_01 is 1. */
typedef struct Part15B58 {
    unsigned char active;
    unsigned char field_01;
    char pad_02[2];
    void *vtable;
} Part15B58;
REGION3_SIZE(Part15B58, 0x8);

/* 0x30 bytes. Reset func_08A167E8, release func_08A1681C. */
typedef struct Part16D18 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    unsigned char field_1C;
    char pad_1D[3];
    int field_20;
    int field_24;
    int field_28;
    void *vtable;
} Part16D18;
REGION3_SIZE(Part16D18, 0x30);
REGION3_AT(Part16D18, field_1C, 0x1C);

/* 0x88 bytes. Reset func_08A16DE0, release func_08A16E50. */
typedef struct Part174B8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    char pad_08[0x7C];
    void *vtable;
} Part174B8;
REGION3_SIZE(Part174B8, 0x88);

/* 0x118 bytes (arrays of it use that stride). Built by func_08A174F0.
 * Reset func_08A175BC, release func_08A175E8. */
typedef struct Part17C90 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    float field_08;
    unsigned char field_0C;
    char pad_0D[3];
    int field_10;
    int field_14;
    PartF8670 field_18;
    Part174B8 field_50;
    Part08388 field_D8;
    int field_10C;
    int field_110;
    void *vtable;
} Part17C90;
REGION3_SIZE(Part17C90, 0x118);
REGION3_AT(Part17C90, field_50, 0x50);
REGION3_AT(Part17C90, field_D8, 0xD8);

/* 0x24 bytes. Reset func_08A18DD8, release func_08A18E18. */
typedef struct Part19278 {
    unsigned char active;
    unsigned char field_01;
    char pad_02[2];
    int field_04[2];
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    void *vtable;
} Part19278;
REGION3_SIZE(Part19278, 0x24);

/* 0x38 bytes. Reset func_08A201AC, release func_08A201C8. */
typedef struct Part20668 {
    unsigned char active;
    char pad_01[3];
    PartF81A8 field_04;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    void *vtable;
} Part20668;
REGION3_SIZE(Part20668, 0x38);
REGION3_AT(Part20668, field_24, 0x24);

/* 0x24 bytes. Reset func_08A20730, release func_08A20764. */
typedef struct Part20B58 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08[4];
    int field_18;
    int field_1C;
    void *vtable;
} Part20B58;
REGION3_SIZE(Part20B58, 0x24);

/* 0x78 bytes. Reset func_08A21508, release func_08A21578. */
typedef struct Part21C08 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    char pad_08[0x6C];
    void *vtable;
} Part21C08;
REGION3_SIZE(Part21C08, 0x78);

/* 0x14 bytes. Built by func_08A21C40. Reset func_08A21CD0, release
 * func_08A21CE4. */
typedef struct Part21F38 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} Part21F38;
REGION3_SIZE(Part21F38, 0x14);

/* 0xFC bytes. Reset func_08A23298, release func_08A232D4; func_08A23930
 * releases both cell sets. */
typedef struct Part24BA0 {
    unsigned char active;
    char pad_01[3];
    float field_04;
    unsigned char field_08[2];
    unsigned char field_0A[2];
    unsigned char field_0C;
    char pad_0D[3];
    PartEF178 field_10;
    PartEF178 field_80;
    int field_F0;
    int field_F4;
    void *vtable;
} Part24BA0;
REGION3_SIZE(Part24BA0, 0xFC);
REGION3_AT(Part24BA0, field_80, 0x80);

/* 0x490 bytes. Reset func_08A22000, release func_08A2200C. */
typedef struct Part22AA0 {
    unsigned char active;
    char pad_01[3];
    Part24BA0 field_04;
    Part08158 field_100;
    int field_488;
    void *vtable;
} Part22AA0;
REGION3_SIZE(Part22AA0, 0x490);
REGION3_AT(Part22AA0, field_100, 0x100);

/* Reset func_08A22B50, release func_08A22B58. */
typedef struct Part22C60 {
    unsigned char active;
    char pad_01[7];
    void *vtable;
} Part22C60;
REGION3_AT(Part22C60, vtable, 0x8);

/* 0x88 bytes. Reset func_08A24CE4, release func_08A24D40. */
typedef struct Part254D0 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    PartF81A8 field_08;
    char pad_28[0x5C];
    void *vtable;
} Part254D0;
REGION3_SIZE(Part254D0, 0x88);

/* 0x74 bytes. Reset func_08A25B94, release func_08A25BDC frees field_04 and
 * field_28[][]. */
typedef struct Part26140 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    PartF81A8 field_08;
    void *field_28[4][4];
    int field_68;
    int field_6C;
    void *vtable;
} Part26140;
REGION3_SIZE(Part26140, 0x74);

/* 0xC8 bytes. Reset func_08A25668, release func_08A2568C. */
typedef struct Part25AD8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    float field_08;
    int field_0C;
    int field_10;
    PartF8670 field_14;
    Part26140 field_4C;
    int field_C0;
    void *vtable;
} Part25AD8;
REGION3_SIZE(Part25AD8, 0xC8);
REGION3_AT(Part25AD8, field_C0, 0xC0);

/* 0x11C bytes (arrays of it use that stride). Built by func_08A261C8.
 * Reset func_08A26294, release func_08A262C4. */
typedef struct Part268C8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    float field_08;
    int field_0C;
    unsigned char field_10;
    char pad_11[3];
    int field_14;
    int field_18;
    PartF8670 field_1C;
    Part254D0 field_54;
    Part08388 field_DC;
    int field_110;
    int field_114;
    void *vtable;
} Part268C8;
REGION3_SIZE(Part268C8, 0x11C);
REGION3_AT(Part268C8, field_DC, 0xDC);

/* Reset func_08A61F84 (layout beyond `active` unknown). */
typedef struct Part61F84 {
    unsigned char active;
} Part61F84;

/* Reset func_08A622FC clears `active` and the 0x120 bytes at field_10. */
typedef struct Part622FC {
    unsigned char active;
    char pad_01[0xF];
    unsigned char field_10[0x120];
} Part622FC;

/* 0x0C bytes. Reset func_08A64F38, release func_08A64F44. */
typedef struct Part65650 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    void *vtable;
} Part65650;
REGION3_SIZE(Part65650, 0xC);

/* 0x330 bytes. Reset func_08A65768, release func_08A65778. */
typedef struct Part65E48 {
    unsigned char active;
    char pad_01[0x323];
    int field_324;
    int field_328;
    void *vtable;
} Part65E48;
REGION3_SIZE(Part65E48, 0x330);

/* 0x10 bytes. Reset func_08A7E7D0, release func_08A7E7E0. */
typedef struct Part7E948 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    void *vtable;
} Part7E948;
REGION3_SIZE(Part7E948, 0x10);

/* 0x1E4 bytes. Reset func_08A65F24, release func_08A65F70. */
typedef struct Part7ABD0 {
    unsigned char active;
    char pad_01[3];
    Part7E948 field_04;
    Part7E948 field_14;
    unsigned char field_24[0x1BC];      /* built by func_08A94228 */
    void *vtable;
} Part7ABD0;
REGION3_SIZE(Part7ABD0, 0x1E4);

/* Read by the getters func_08A663E4, func_08A66404 and func_08A66424; the
 * class is not known. */
typedef struct Part663E4 {
    unsigned char active;
    char pad_01;
    unsigned char field_02;
    char pad_03[0x49];
    unsigned char field_4C;
    unsigned char field_4D;
} Part663E4;
REGION3_AT(Part663E4, field_4C, 0x4C);

/* 0x24 bytes. Reset func_08A7B260, release func_08A7B288. */
typedef struct Part7E728 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    unsigned char field_14;
    char pad_15[3];
    int field_18;
    float field_1C;
    void *vtable;
} Part7E728;
REGION3_SIZE(Part7E728, 0x24);

/* 0xA4 bytes. Reset func_08A7E9E8, release func_08A7EA74. */
typedef struct Part7FF38 {
    unsigned char active;
    char pad_01[0x23];
    unsigned char field_24;
    char pad_25[0x77];
    float field_9C;
    void *vtable;
} Part7FF38;
REGION3_SIZE(Part7FF38, 0xA4);
REGION3_AT(Part7FF38, field_9C, 0x9C);

/* 0x2D0 bytes. Derives from the class built by func_08AAAE10 (destroyed by
 * func_08AAB86C); its own reset func_08A8014C is empty. */
typedef struct Part80208 {
    unsigned char base[0x2CC];          /* built by func_08AAAE10 */
    void *vtable;
} Part80208;
REGION3_SIZE(Part80208, 0x2D0);

/* 0x70 bytes. Reset func_08A80800, release func_08A80884. */
typedef struct Part815C8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    unsigned char field_10;
    unsigned char field_11;
    char pad_12[2];
    int field_14;
    BattleVec3 field_18;
    BattleVec3 field_24;
    BattleVec3 field_30;
    BattleVec3 field_3C;
    BattleVec3 field_48;
    int field_54;
    int field_58;
    int field_5C;
    unsigned char field_60;
    unsigned char field_61;
    char pad_62[2];
    int field_64;
    unsigned char field_68;
    char pad_69[3];
    void *vtable;
} Part815C8;
REGION3_SIZE(Part815C8, 0x70);
REGION3_AT(Part815C8, field_54, 0x54);

/* 0x1424 bytes. Reset func_08A63950, release func_08A6396C; operator new
 * func_08A639F4, operator delete func_08A63A10. Most methods forward to the
 * member at field_20. */
typedef struct Part64E48 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    char pad_14[0xC];
    unsigned char field_20[0x510];      /* built by func_08AB2EF0 */
    unsigned char field_530[0x80];      /* built by func_08A96398 */
    unsigned char field_5B0[0x660];     /* built by func_08A870F0 */
    Part815C8 field_C10;
    unsigned char field_C80[0x1C];      /* built by func_08AA4CB0 */
    Part65E48 field_C9C;
    char pad_FCC[4];
    Part7ABD0 field_FD0;
    char pad_11B4[0xC];
    unsigned char field_11C0[0x170];    /* built by func_08A9AC18 */
    unsigned char field_1330[0x2C];     /* built by func_08A9A538 */
    unsigned char field_135C[0x30];     /* built by func_08AA1ED0 */
    unsigned char field_138C[0x14];     /* built by func_08A9A1B8 */
    unsigned char field_13A0[0x80];     /* built by func_08AA29F0 */
    void *vtable;
} Part64E48;
REGION3_SIZE(Part64E48, 0x1424);
REGION3_AT(Part64E48, field_C10, 0xC10);
REGION3_AT(Part64E48, field_C9C, 0xC9C);
REGION3_AT(Part64E48, field_FD0, 0xFD0);
REGION3_AT(Part64E48, field_11C0, 0x11C0);
REGION3_AT(Part64E48, field_13A0, 0x13A0);

/* ------------------------------------------------------------ units */

/* Base of the units: constructor func_08A157D0, destructor func_08A157E4,
 * operator new func_08A15818, operator delete func_08A15834. */
typedef struct Unit158B8 {
    void *vtable;
} Unit158B8;

/* Built by func_08A15B68. Reset func_08A15C58, release func_08A15C88. */
typedef struct Unit165C8 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    Part10F98 field_18;
    PartF8670 field_44;
    Part0EB78 field_7C;
    Part08388 field_114;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
} Unit165C8;
REGION3_SIZE(Unit165C8, 0x158);
REGION3_AT(Unit165C8, field_114, 0x114);

/* Built by func_08A17CA8. Reset func_08A17DD4, release func_08A17E04. */
typedef struct Unit18BD8 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_0E[2];
    float field_10;
    Part01DA8 field_14;
    Part19278 field_3C;
    Part02198 field_60;
    Part16D18 field_84;
    Part02198 field_B4;
    Part00CE0 field_D8;
    Part08388 field_114;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
} Unit18BD8;
REGION3_SIZE(Unit18BD8, 0x158);
REGION3_AT(Unit18BD8, field_84, 0x84);
REGION3_AT(Unit18BD8, field_114, 0x114);

/* Built by func_08A19288 on the UnitF13D8 base. Reset func_08A19408,
 * release func_08A19434. */
typedef struct Unit1A350 {
    UnitF13D8 base;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_0E[2];
    float field_10;
    Part01DA8 field_14[2];
    Part19278 field_64[2];
    Part02198 field_AC[2];
    Part17C90 field_F4[2];
    int field_324;
    int field_328;
    int field_32C;
} Unit1A350;
REGION3_SIZE(Unit1A350, 0x330);
REGION3_AT(Unit1A350, field_F4, 0xF4);

/* Built by func_08A1A528. Reset func_08A1A654, release func_08A1A68C. */
typedef struct Unit1B3F8 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_0E[2];
    float field_10;
    int field_14;
    int field_18;
    Part01DA8 field_1C;
    Part19278 field_44;
    Part02198 field_68;
    Part10F98 field_8C;
    PartF8670 field_B8;
    PartF63E0 field_F0;
    Part08388 field_138;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
} Unit1B3F8;
REGION3_SIZE(Unit1B3F8, 0x17C);
REGION3_AT(Unit1B3F8, field_F0, 0xF0);
REGION3_AT(Unit1B3F8, field_138, 0x138);

/* Built by func_08A1B520. Reset func_08A1B638, release func_08A1B66C.
 * field_14 is handed to the Part21C08 when field_64 has finished. */
typedef struct Unit1C398 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_0E[2];
    float field_10;
    int field_14;
    Part01DA8 field_18;
    Part19278 field_40;
    Part02198 field_64;
    Part21C08 field_88;
    PartF8670 field_100;
    Part08388 field_138;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
} Unit1C398;
REGION3_SIZE(Unit1C398, 0x17C);
REGION3_AT(Unit1C398, field_88, 0x88);
REGION3_AT(Unit1C398, field_138, 0x138);

/* Built by func_08A1C540. Reset func_08A1C658, release func_08A1C688. */
typedef struct Unit1D3F8 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_0E[2];
    float field_10;
    Part01DA8 field_14;
    Part01DA8 field_3C;
    Part01DA8 field_64;
    PartF8670 field_8C;
    Part10F98 field_C4;
    Part08388 field_F0;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
} Unit1D3F8;
REGION3_SIZE(Unit1D3F8, 0x134);
REGION3_AT(Unit1D3F8, field_C4, 0xC4);

/* Built by func_08A1D4E8. Reset func_08A1D614, release func_08A1D64C. */
typedef struct Unit1E058 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    int field_18;
    unsigned char field_1C;
    char pad_1D[3];
    Part10F98 field_20;
    PartF8670 field_4C;
    PartF63E0 field_84;
    Part01DA8 field_CC;
    Part16D18 field_F4;
    PartFE400 field_124;
    PartFC658 field_158;
    int field_264;
    int field_268;
    int field_26C;
    int field_270;
} Unit1E058;
REGION3_SIZE(Unit1E058, 0x274);
REGION3_AT(Unit1E058, field_CC, 0xCC);
REGION3_AT(Unit1E058, field_158, 0x158);

/* Built by func_08A1E1E8; same members as UnitFB568. Reset func_08A1E2D8,
 * release func_08A1E30C. */
typedef struct Unit1EB40 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    int field_18;
    Part10F98 field_1C;
    PartF8670 field_48;
    PartF63E0 field_80;
    Part08388 field_C8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
} Unit1EB40;
REGION3_SIZE(Unit1EB40, 0x10C);
REGION3_AT(Unit1EB40, field_80, 0x80);

/* Built by func_08A1EC00. Reset func_08A1ED04, release func_08A1ED38. */
typedef struct Unit1F628 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    int field_18;
    Part10F98 field_1C;
    Part10F98 field_48;
    PartF8670 field_74;
    PartF63E0 field_AC;
    Part08388 field_F4;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
} Unit1F628;
REGION3_SIZE(Unit1F628, 0x138);
REGION3_AT(Unit1F628, field_AC, 0xAC);

/* Built by func_08A1F718; same members as Unit1EB40. Reset func_08A1F808,
 * release func_08A1F83C. */
typedef struct Unit20040 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    int field_18;
    Part10F98 field_1C;
    PartF8670 field_48;
    PartF63E0 field_80;
    Part08388 field_C8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
} Unit20040;
REGION3_SIZE(Unit20040, 0x10C);

/* Built by func_08A20B90. Reset func_08A20C94, release func_08A20CC0. */
typedef struct Unit213C0 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part01DA8 field_14;
    Part16D18 field_3C;
    Part02198 field_6C;
    Part20668 field_90;
    Part20B58 field_C8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
} Unit213C0;
REGION3_SIZE(Unit213C0, 0xFC);
REGION3_AT(Unit213C0, field_C8, 0xC8);

/* Built by func_08A268E0. Reset func_08A269A8, release func_08A269DC. */
typedef struct Unit27200 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    unsigned char field_14;
    char pad_15[3];
    int field_18;
    Part08388 field_1C;
    PartF8670 field_50;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
} Unit27200;
REGION3_SIZE(Unit27200, 0x98);

/* Built by func_08A272C0. Reset func_08A27374, release func_08A273A0. */
typedef struct Unit279D0 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part08388 field_14;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
} Unit279D0;
REGION3_SIZE(Unit279D0, 0x58);

/* Built by func_08A27A90. Reset func_08A27B80, release func_08A27BB4. */
typedef struct Unit28520 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    int field_18;
    Part01DA8 field_1C;
    Part16D18 field_44;
    PartFE400 field_74;
    PartFCB00 field_A8;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
} Unit28520;
REGION3_SIZE(Unit28520, 0xD4);
REGION3_AT(Unit28520, field_A8, 0xA8);

/* Built by func_08A28608 on the UnitF13D8 base. Reset func_08A286BC,
 * release func_08A286E4. */
typedef struct Unit28BB8 {
    UnitF13D8 base;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part08388 field_14;
    int field_48;
    int field_4C;
    int field_50;
} Unit28BB8;
REGION3_SIZE(Unit28BB8, 0x54);

/* Built by func_08A28C70 on the UnitF13D8 base. Reset func_08A28D5C,
 * release func_08A28D7C. */
typedef struct Unit29370 {
    UnitF13D8 base;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    Part268C8 field_10[2];
    Part25AD8 field_248;
    int field_310;
    int field_314;
    int field_318;
} Unit29370;
REGION3_SIZE(Unit29370, 0x31C);
REGION3_AT(Unit29370, field_248, 0x248);

/* Built by func_08A29420. Reset func_08A29510, release func_08A29544. */
typedef struct Unit29D08 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    int field_18;
    Part01DA8 field_1C;
    Part16D18 field_44;
    PartF8670 field_74;
    PartF63E0 field_AC;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
} Unit29D08;
REGION3_SIZE(Unit29D08, 0x104);

/* Built by func_08A29DC8; same members as Unit0D608. Reset func_08A29E90,
 * release func_08A29EBC. */
typedef struct Unit2A4B8 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part01DA8 field_14;
    Part16D18 field_3C;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
} Unit2A4B8;
REGION3_SIZE(Unit2A4B8, 0x7C);

/* Built by func_08A2A570 on the UnitF13D8 base. Reset func_08A2A64C,
 * release func_08A2A67C. */
typedef struct Unit2AC40 {
    UnitF13D8 base;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    int field_18;
    Part10F98 field_1C;
    PartF8670 field_48;
    PartF63E0 field_80;
    int field_C8;
    int field_CC;
    int field_D0;
} Unit2AC40;
REGION3_SIZE(Unit2AC40, 0xD4);

/* Owner of two models that func_08A3E874 loads (field_220, field_224) and of
 * nine handles (field_228[]); func_08A3E418 takes five of them from the model
 * at field_224 and func_08A3DB7C frees them all. */
typedef struct Model3E874 {
    char pad_000[0x220];
    void *field_220;
    void *field_224;
    void *field_228[9];
    char pad_24C[4];
    unsigned char field_250;            /* set once the handles are taken */
    char pad_251[0x37];
    unsigned char field_288[4];         /* passed to func_0887AD1C */
} Model3E874;
REGION3_AT(Model3E874, field_250, 0x250);
REGION3_AT(Model3E874, field_288, 0x288);

/* Derives from the class destroyed by func_08B2EDC4 and keeps its vtable
 * after that base. */
typedef struct Unit61CF8 {
    unsigned char base[0x544];
    void *vtable;
    char pad_548[0x28];
    BattleVec4 field_570;
    BattleVec4 field_580;
} Unit61CF8;
REGION3_AT(Unit61CF8, field_570, 0x570);

/* ------------------------------------------------------------ fighters */

/* The data of the fighter's current state, at field_3A8. Each state has its
 * own layout; only the fields used here are named. */
typedef struct FighterWork {
    char pad_00[0x10];
    unsigned char field_10;
    char pad_11[3];
    float field_14;
    char pad_18[8];
    void *field_20;
    char pad_24[0xA];
    signed char field_2E;               /* tested as a signed byte */
    char pad_2F;
    void *field_30;
    char pad_34[0xC];
    void *field_40;
    void *field_44;
    void *field_48;
} FighterWork;
REGION3_AT(FighterWork, field_2E, 0x2E);
REGION3_AT(FighterWork, field_48, 0x48);

/* Pointed to by field_314; func_08A5BF84 and func_08A5BE70 fill it. */
typedef struct FighterMotion {
    char pad_00[0xF4];
    int field_F4;
} FighterMotion;

typedef struct BattleFighter {
    char pad_000[0x180];
    short motion;                       /* compared with motion ids */
    char pad_182[6];
    unsigned int field_188;
    char pad_18C[0x188];
    FighterMotion *field_314;
    char pad_318[0x6C];
    unsigned int flags;
    char pad_388[0x20];
    FighterWork *work;
    char pad_3AC[0x18C];
    int field_538;
    char pad_53C[0xB38];
    unsigned char field_1074;
    char pad_1075[2];
    unsigned char field_1077;
} BattleFighter;
REGION3_AT(BattleFighter, field_314, 0x314);
REGION3_AT(BattleFighter, flags, 0x384);
REGION3_AT(BattleFighter, work, 0x3A8);
REGION3_AT(BattleFighter, field_538, 0x538);
REGION3_AT(BattleFighter, field_1074, 0x1074);

/* ------------------------------------------------------------ globals */

/* The table D_08A50568 points to: a header of four words, then 0x10-byte
 * entries. */
typedef struct Table50568Entry {
    unsigned char data[0x10];
} Table50568Entry;

typedef struct Table50568 {
    int field_00;
    int field_04;
    char pad_08[8];
    Table50568Entry entries[1];         /* length unknown */
} Table50568;

typedef struct Global89D6AC8 {
    char pad_000[0x3A0];
    unsigned char field_3A0[4];
} Global89D6AC8;

extern Table50568 *D_08A50568;
extern Global89D6AC8 *D_089D6AC8;
extern void *D_08A405E0;
extern void *D_089D4D80;
extern void *D_089D7830;
extern void *D_08AAAD80;
extern void *D_08AA5828;
extern BattleVec4 D_09AF84E0;

/* ------------------------------------------------------------ vtables */

extern unsigned char D_08A157B8[];
extern unsigned char D_08A158B8[];
extern unsigned char D_08A15B58[];
extern unsigned char D_08A165C8[];
extern unsigned char D_08A16D18[];
extern unsigned char D_08A174B8[];
extern unsigned char D_08A17C90[];
extern unsigned char D_08A18BD8[];
extern unsigned char D_08A19278[];
extern unsigned char D_08A1A350[];
extern unsigned char D_08A1B3F8[];
extern unsigned char D_08A1C398[];
extern unsigned char D_08A1D3F8[];
extern unsigned char D_08A1E058[];
extern unsigned char D_08A1EB40[];
extern unsigned char D_08A1F628[];
extern unsigned char D_08A20040[];
extern unsigned char D_08A20668[];
extern unsigned char D_08A20B58[];
extern unsigned char D_08A213C0[];
extern unsigned char D_08A21C08[];
extern unsigned char D_08A21F38[];
extern unsigned char D_08A22AA0[];
extern unsigned char D_08A22C60[];
extern unsigned char D_08A24BA0[];
extern unsigned char D_08A254D0[];
extern unsigned char D_08A25AD8[];
extern unsigned char D_08A26140[];
extern unsigned char D_08A268C8[];
extern unsigned char D_08A27200[];
extern unsigned char D_08A279D0[];
extern unsigned char D_08A28520[];
extern unsigned char D_08A28BB8[];
extern unsigned char D_08A29370[];
extern unsigned char D_08A29D08[];
extern unsigned char D_08A2A4B8[];
extern unsigned char D_08A2AC40[];
extern unsigned char D_08A61CF8[];
extern unsigned char D_08A64E48[];
extern unsigned char D_08A65650[];
extern unsigned char D_08A65E48[];
extern unsigned char D_08A7ABD0[];
extern unsigned char D_08A7E728[];
extern unsigned char D_08A7E948[];
extern unsigned char D_08A7FF38[];
extern unsigned char D_08A80208[];
extern unsigned char D_08A815C8[];

/* ------------------------------------------------------------ functions */

/* Unit15448 (region 2): more steps. */
int func_08A1533C(Unit15448 *self);
int func_08A1537C(Unit15448 *self);
int func_08A153C4(Unit15448 *self);
int func_08A15404(Unit15448 *self);
void func_08A020C8(Part02198 *self);

/* Part157B8 (constructor func_08A15540, destructor func_08A15578) */
void func_08A155D0(Part157B8 *self);
void func_08A155DC(Part157B8 *self);
int func_08A1561C(Part157B8 *self);
unsigned char func_08A156C0(Part157B8 *self);

/* Unit158B8 (constructor func_08A157D0, operator delete func_08A15834) */
void *func_08A15818(unsigned int size);

/* Part15B58 (constructor func_08A15968, destructor func_08A159A0) */
void func_08A159F8(Part15B58 *self);
void func_08A15A04(Part15B58 *self);
int func_08A15A40(Part15B58 *self);
unsigned char func_08A15A70(Part15B58 *self);
void func_08A15AF8(Part15B58 *self);

/* Unit165C8 */
Unit165C8 *func_08A15B68(Unit165C8 *self);
void func_08A15BC4(Unit165C8 *self, int flags);
void func_08A15C58(Unit165C8 *self);
void func_08A15C88(Unit165C8 *self);
int func_08A164E0(Unit165C8 *self);
int func_08A165A8(Unit165C8 *self);

/* Part16D18 (constructor func_08A16758, destructor func_08A16790) */
void func_08A167E8(Part16D18 *self);
void func_08A1681C(Part16D18 *self);
void func_08A16B78(Part16D18 *self);

/* Part174B8 */
Part174B8 *func_08A16D50(Part174B8 *self);
void func_08A16D88(Part174B8 *self, int flags);
void func_08A16DE0(Part174B8 *self);
void func_08A16E50(Part174B8 *self);
unsigned char func_08A17264(Part174B8 *self);

/* Part17C90 (constructor func_08A174F0, destructor func_08A17540) */
void func_08A175BC(Part17C90 *self);
void func_08A175E8(Part17C90 *self);
unsigned char func_08A179D8(Part17C90 *self);
int func_08A17B84(Part17C90 *self);
int func_08A17C50(Part17C90 *self);

/* Unit18BD8 */
Unit18BD8 *func_08A17CA8(Unit18BD8 *self);
void func_08A17D1C(Unit18BD8 *self, int flags);
void func_08A17DD4(Unit18BD8 *self);
void func_08A17E04(Unit18BD8 *self);
int func_08A189B0(Unit18BD8 *self);
int func_08A18AAC(Unit18BD8 *self);
int func_08A18AEC(Unit18BD8 *self);
int func_08A18B34(Unit18BD8 *self);
int func_08A18B88(Unit18BD8 *self);
int func_08A18BC8(Unit18BD8 *self);

/* Part19278 */
Part19278 *func_08A18D48(Part19278 *self);
void func_08A18D80(Part19278 *self, int flags);
void func_08A18DD8(Part19278 *self);
void func_08A18E18(Part19278 *self);

/* Unit1A350 */
Unit1A350 *func_08A19288(Unit1A350 *self);
void func_08A19324(Unit1A350 *self, int flags);
void func_08A19408(Unit1A350 *self);
void func_08A19434(Unit1A350 *self);
int func_08A1A33C(Unit1A350 *self);

/* Unit1B3F8 */
Unit1B3F8 *func_08A1A528(Unit1B3F8 *self);
void func_08A1A59C(Unit1B3F8 *self, int flags);
void func_08A1A654(Unit1B3F8 *self);
void func_08A1A68C(Unit1B3F8 *self);
int func_08A1B250(Unit1B3F8 *self);
int func_08A1B29C(Unit1B3F8 *self);
int func_08A1B2DC(Unit1B3F8 *self);
int func_08A1B310(Unit1B3F8 *self);
int func_08A1B358(Unit1B3F8 *self);
int func_08A1B3A4(Unit1B3F8 *self);
int func_08A1B3E4(Unit1B3F8 *self);
unsigned char func_089F6388(PartF63E0 *self);

/* Unit1C398 */
Unit1C398 *func_08A1B520(Unit1C398 *self);
void func_08A1B58C(Unit1C398 *self, int flags);
void func_08A1B638(Unit1C398 *self);
void func_08A1B66C(Unit1C398 *self);
int func_08A1C1C4(Unit1C398 *self);
int func_08A1C218(Unit1C398 *self);
int func_08A1C264(Unit1C398 *self);
int func_08A1C2A4(Unit1C398 *self);
int func_08A1C2B4(Unit1C398 *self);
int func_08A1C2FC(Unit1C398 *self);
int func_08A1C348(Unit1C398 *self);
int func_08A1C388(Unit1C398 *self);

/* Unit1D3F8 */
Unit1D3F8 *func_08A1C540(Unit1D3F8 *self);
void func_08A1C5AC(Unit1D3F8 *self, int flags);
void func_08A1C658(Unit1D3F8 *self);
void func_08A1C688(Unit1D3F8 *self);
int func_08A1D27C(Unit1D3F8 *self);
int func_08A1D2D0(Unit1D3F8 *self);
int func_08A1D360(Unit1D3F8 *self);
int func_08A1D3B4(Unit1D3F8 *self);
int func_08A01CF4(Part01DA8 *self);

/* Unit1E058 */
Unit1E058 *func_08A1D4E8(Unit1E058 *self);
void func_08A1D55C(Unit1E058 *self, int flags);
void func_08A1D614(Unit1E058 *self);
void func_08A1D64C(Unit1E058 *self);
int func_08A1E018(Unit1E058 *self);

/* Unit1EB40 */
Unit1EB40 *func_08A1E1E8(Unit1EB40 *self);
void func_08A1E244(Unit1EB40 *self, int flags);
void func_08A1E2D8(Unit1EB40 *self);
void func_08A1E30C(Unit1EB40 *self);
int func_08A1EA34(Unit1EB40 *self);
int func_08A1EB1C(Unit1EB40 *self);

/* Unit1F628 */
Unit1F628 *func_08A1EC00(Unit1F628 *self);
void func_08A1EC64(Unit1F628 *self, int flags);
void func_08A1ED04(Unit1F628 *self);
void func_08A1ED38(Unit1F628 *self);
int func_08A1F51C(Unit1F628 *self);
int func_08A1F604(Unit1F628 *self);

/* Unit20040 */
Unit20040 *func_08A1F718(Unit20040 *self);
void func_08A1F774(Unit20040 *self, int flags);
void func_08A1F808(Unit20040 *self);
void func_08A1F83C(Unit20040 *self);
int func_08A1FF88(Unit20040 *self);
int func_08A20020(Unit20040 *self);

/* Part20668 */
Part20668 *func_08A20108(Part20668 *self);
void func_08A20148(Part20668 *self, int flags);
void func_08A201AC(Part20668 *self);
void func_08A201C8(Part20668 *self);

/* Part20B58 */
Part20B58 *func_08A206A0(Part20B58 *self);
void func_08A206D8(Part20B58 *self, int flags);
void func_08A20730(Part20B58 *self);
void func_08A20764(Part20B58 *self);

/* Unit213C0 */
Unit213C0 *func_08A20B90(Unit213C0 *self);
void func_08A20BF4(Unit213C0 *self, int flags);
void func_08A20C94(Unit213C0 *self);
void func_08A20CC0(Unit213C0 *self);
int func_08A2137C(Unit213C0 *self);

/* Part21C08 (constructor func_08A21478, destructor func_08A214B0) */
void func_08A21508(Part21C08 *self);
void func_08A21578(Part21C08 *self);
unsigned char func_08A219D8(Part21C08 *self);
int func_08A219FC(Part21C08 *self, int value);

/* Part21F38 (constructor func_08A21C40, release func_08A21CE4) */
void func_08A21C78(Part21F38 *self, int flags);
void func_08A21CD0(Part21F38 *self);

/* Part22AA0 */
Part22AA0 *func_08A21F48(Part22AA0 *self);
void func_08A21F90(Part22AA0 *self, int flags);
void func_08A22000(Part22AA0 *self);
void func_08A2200C(Part22AA0 *self);
void func_08A2247C(Part22AA0 *self);
void func_08A22518(Part22AA0 *self);
void func_08A22534(Part22AA0 *self, unsigned int arg1, unsigned char arg2);
void func_08A22554(Part22AA0 *self, unsigned int arg1, unsigned int arg2, unsigned char arg3);
Part08158 *func_08A22970(Part22AA0 *self);
void func_08A22A68(Part22AA0 *self);
void func_08A22A84(Part22AA0 *self);
int func_08A072A8(Part08158 *self);
int func_08A07900(Part08158 *self);

/* Part22C60 */
void func_08A22AF8(Part22C60 *self, int flags);
void func_08A22B50(Part22C60 *self);
void func_08A22B58(Part22C60 *self);

/* Part24BA0 */
Part24BA0 *func_08A231D8(Part24BA0 *self);
void func_08A23220(Part24BA0 *self, int flags);
void func_08A23298(Part24BA0 *self);
void func_08A232D4(Part24BA0 *self);
unsigned int func_08A23488(Part24BA0 *self, unsigned int arg1, unsigned char arg2);
unsigned int func_08A2352C(Part24BA0 *self, unsigned int arg1, int arg2, unsigned char arg3);
int func_08A23870(Part24BA0 *self);
void func_08A23930(Part24BA0 *self);
void func_089EEBC0(PartEF178 *self, int flags);

/* Part254D0 */
Part254D0 *func_08A24C40(Part254D0 *self);
void func_08A24C80(Part254D0 *self, int flags);
void func_08A24CE4(Part254D0 *self);
void func_08A24D40(Part254D0 *self);
unsigned char func_08A25194(Part254D0 *self);

/* Part25AD8 */
Part25AD8 *func_08A255B0(Part25AD8 *self);
void func_08A255F8(Part25AD8 *self, int flags);
void func_08A25668(Part25AD8 *self);
void func_08A2568C(Part25AD8 *self);
int func_08A25A98(Part25AD8 *self);

/* Part26140 */
Part26140 *func_08A25AF0(Part26140 *self);
void func_08A25B30(Part26140 *self, int flags);
void func_08A25B94(Part26140 *self);
void func_08A25BDC(Part26140 *self);
unsigned char func_08A25F3C(Part26140 *self);

/* Part268C8 */
Part268C8 *func_08A261C8(Part268C8 *self);
void func_08A26218(Part268C8 *self, int flags);
void func_08A26294(Part268C8 *self);
void func_08A262C4(Part268C8 *self);
int func_08A267E8(Part268C8 *self);
int func_08A26888(Part268C8 *self);

/* Unit27200 */
Unit27200 *func_08A268E0(Unit27200 *self);
void func_08A2692C(Unit27200 *self, int flags);
void func_08A269A8(Unit27200 *self);
void func_08A269DC(Unit27200 *self);
int func_08A271DC(Unit27200 *self);

/* Unit279D0 */
Unit279D0 *func_08A272C0(Unit279D0 *self);
void func_08A27304(Unit279D0 *self, int flags);
void func_08A27374(Unit279D0 *self);
void func_08A273A0(Unit279D0 *self);
int func_08A27934(Unit279D0 *self);
int func_08A279AC(Unit279D0 *self);

/* Unit28520 */
Unit28520 *func_08A27A90(Unit28520 *self);
void func_08A27AEC(Unit28520 *self, int flags);
void func_08A27B80(Unit28520 *self);
void func_08A27BB4(Unit28520 *self);
int func_08A2832C(Unit28520 *self);

/* Unit28BB8 */
Unit28BB8 *func_08A28608(Unit28BB8 *self);
void func_08A2864C(Unit28BB8 *self, int flags);
void func_08A286BC(Unit28BB8 *self);
void func_08A286E4(Unit28BB8 *self);
int func_08A28B98(Unit28BB8 *self);

/* Unit29370 */
Unit29370 *func_08A28C70(Unit29370 *self);
void func_08A28CCC(Unit29370 *self, int flags);
void func_08A28D5C(Unit29370 *self);
void func_08A28D7C(Unit29370 *self);
int func_08A290BC(Unit29370 *self);

/* Unit29D08 */
Unit29D08 *func_08A29420(Unit29D08 *self);
void func_08A2947C(Unit29D08 *self, int flags);
void func_08A29510(Unit29D08 *self);
void func_08A29544(Unit29D08 *self);
int func_08A29CC4(Unit29D08 *self);

/* Unit2A4B8 */
Unit2A4B8 *func_08A29DC8(Unit2A4B8 *self);
void func_08A29E14(Unit2A4B8 *self, int flags);
void func_08A29E90(Unit2A4B8 *self);
void func_08A29EBC(Unit2A4B8 *self);
int func_08A2A478(Unit2A4B8 *self);

/* Unit2AC40 */
Unit2AC40 *func_08A2A570(Unit2AC40 *self);
void func_08A2A5C4(Unit2AC40 *self, int flags);
void func_08A2A64C(Unit2AC40 *self);
void func_08A2A67C(Unit2AC40 *self);
int func_08A2AC00(Unit2AC40 *self);

/* Fighter state callbacks and their helpers. */
void func_08A2C7EC(void *self, BattleFighter *fighter, const BattleVec4 *vector);
void func_08A2C80C(void *self, BattleFighter *fighter, float speed);
void func_08A2C8DC(void *self, BattleFighter *fighter);
void func_08A2E8B4(void *self, BattleFighter *fighter, const BattleVec4 *vector);
void func_08A2E8D4(void *self, BattleFighter *fighter, float speed);
void func_08A505B4(void *self, BattleFighter *fighter, const BattleVec4 *vector);
void func_08A50640(void *self, BattleFighter *fighter, float speed);
void func_08A507B8(void *self, BattleFighter *fighter);
void func_08A5153C(void *self, BattleFighter *fighter);
void func_08A52AD8(void *self, BattleFighter *fighter, const BattleVec4 *vector);
void func_08A52B70(void *self, BattleFighter *fighter, float speed);
void func_08A52B8C(void *self, BattleFighter *fighter);
void func_08A54CD0(BattleFighter *fighter);
void func_08A54E0C(void *self, BattleFighter *fighter);
void func_08A5680C(void *self, BattleFighter *fighter);
void func_08A573CC(BattleFighter *fighter);
void func_08A58064(void *self, BattleFighter *fighter);
void func_08A58098(BattleFighter *fighter, float scale);
void func_08A58718(void *self, BattleFighter *fighter);
void func_08A588FC(BattleFighter *fighter);
void func_08A58A5C(BattleFighter *fighter, const BattleVec4 *vector);
void func_08A58B3C(void *self, BattleFighter *fighter);
void func_08A58B58(void);
void func_08A5914C(void);
void func_08A59538(void *self, BattleFighter *fighter, const BattleVec4 *vector);
void func_08A59558(void *self, BattleFighter *fighter, float speed);
void func_08A5B00C(void *self, BattleFighter *fighter);
void func_08A5C354(void *self, BattleFighter *fighter, const BattleVec4 *vector);
void func_08A5C5CC(void *self, BattleFighter *fighter);
void func_08A5C60C(void *self, BattleFighter *fighter);
void func_08A5DFC4(void *self, BattleFighter *fighter, const BattleVec4 *vector);
void func_08A5E22C(void *self, BattleFighter *fighter, float speed);
void func_08A5BF84(BattleFighter *fighter, const BattleVec4 *vector);
void func_08A5BE70(BattleFighter *fighter, float speed);
void func_08A519D4(void *self, BattleFighter *fighter);
void func_08A59134(void *self, BattleFighter *fighter);
void func_08834C10(BattleFighter *fighter, int state, int arg);
void func_08834934(BattleFighter *fighter, int motion1, int motion2);
void func_08834948(BattleFighter *fighter);
int func_0883752C(BattleFighter *fighter);
void func_08837A38(BattleFighter *fighter);
void func_08837A5C(BattleFighter *fighter);
unsigned int func_08B3459C(BattleFighter *fighter);
void func_088A6AA4(void *handle);

/* Float factors from func_08A3EB2C. */
float func_08A3EB2C(void *object);

/* Unit61CF8 */
void func_08A61BF4(Unit61CF8 *self, const BattleVec4 *value);
void func_08A61C10(Unit61CF8 *self, const BattleVec4 *value);
void func_08A61C98(Unit61CF8 *self, int flags);
void func_08B2EDC4(void *self, int flags);
void func_0883133C(void *object);
void *func_089A3868(void *dst, const void *src);

/* Part61F84 */
Part61F84 *func_08A61F10(Part61F84 *self);
void func_08A61F38(Part61F84 *self, int flags);
void func_08A61F84(Part61F84 *self);
void func_08A61FAC(Part61F84 *self);
unsigned char func_08A6217C(Part61F84 *self);

/* Part622FC */
Part622FC *func_08A62288(Part622FC *self);
void func_08A622B0(Part622FC *self, int flags);
void func_08A622FC(Part622FC *self);
void func_08A62324(Part622FC *self);
unsigned char func_08A62EF8(Part622FC *self);
void *func_08935828(void *dst, int value, int size);

/* Part64E48 */
Part64E48 *func_08A637B8(Part64E48 *self, int arg);
void func_08A63868(Part64E48 *self, int flags);
void func_08A63950(Part64E48 *self);
void func_08A6396C(Part64E48 *self);
void *func_08A639F4(unsigned int size);
void func_08A63A10(void *object);
int func_08A63A2C(Part64E48 *self, int arg);
void func_08A64078(Part64E48 *self, int arg);
int func_08A640AC(Part64E48 *self);
void func_08A640CC(Part64E48 *self, int arg);
void func_08A64100(Part64E48 *self, int arg);
void func_08A64134(Part64E48 *self, int arg);
void func_08A64168(Part64E48 *self, int arg);
int func_08A6419C(Part64E48 *self, int arg);
void func_08A641E8(Part64E48 *self);
void func_08A64244(Part64E48 *self);
void func_08A64260(Part64E48 *self);
void func_08A6427C(Part64E48 *self);
void func_08A64298(Part64E48 *self);
Part815C8 *func_08A642B4(Part64E48 *self);
void func_08A642BC(Part64E48 *self);
void func_08A6433C(Part64E48 *self);
void func_08A64360(Part64E48 *self);
void func_08A64384(Part64E48 *self);
void func_08A643A8(Part64E48 *self);
void func_08A643C4(Part64E48 *self, unsigned char flag);
void func_08A643E4(Part64E48 *self);
void *func_08842100(unsigned int size);
void func_08842138(void *object);

/* Members of Part64E48 outside this region. */
int func_08AB2EF0(void *object);
void func_08AB2F60(void *object, int flags);
void func_08AB3014(void *object);
void *func_08AB3264(void *object);
void *func_08AB326C(void *object);
void *func_08AB327C(void *object);
int func_08AB3294(void *object);
int func_08A96398(void *object);
void func_08A963E0(void *object, int flags);
void func_08A9649C(void *object);
int func_08A870F0(void *object);
void func_08A871B8(void *object, int flags);
void func_08A87328(void *object);
int func_08AA4CB0(void *object);
void func_08AA4CE8(void *object, int flags);
void func_08AA4D64(void *object);
int func_08A9AC18(void *object);
void func_08A9AC58(void *object, int flags);
void func_08A9ADA8(void *object);
int func_08A9A538(void *object);
void func_08A9A578(void *object, int flags);
void func_08A9A5EC(void *object);
int func_08AA1ED0(void *object);
void func_08AA1F10(void *object, int flags);
void func_08AA1F8C(void *object);
int func_08A9A1B8(void *object);
void func_08A9A1F0(void *object, int flags);
void func_08A9A25C(void *object);
int func_08AA29F0(void *object);
void func_08AA2A30(void *object, int flags);
void func_08AA2AB8(void *object);
int func_08AA2E04(void *object);
int func_08A976E4(void *object, unsigned int arg);
int func_08A97404(void *object, int arg);
int func_08A97448(void *object, int arg);
int func_08A979D0(void *object, int arg);
int func_08A97878(void *object, int arg);
int func_08A978F4(void *object, int arg);
int func_08A97908(void *object);
void func_08A970B4(void *object);
void func_08A970D0(void *object);
void func_08A97060(void *object);
void func_08A9707C(void *object);
int func_08AAE57C(void *object);
int func_08AB29B0(void *object);
float func_08AB29F4(void *object);
int func_08A9A468(void *object);
unsigned int func_08A9A4AC(void *object, unsigned char flag);
int func_08A9A4E0(void *object);

/* Part65650 */
Part65650 *func_08A64EA8(Part65650 *self);
void func_08A64EE0(Part65650 *self, int flags);
void func_08A64F38(Part65650 *self);
void func_08A64F44(Part65650 *self);

/* Part65E48 */
Part65E48 *func_08A656D8(Part65E48 *self);
void func_08A65710(Part65E48 *self, int flags);
void func_08A65768(Part65E48 *self);
void func_08A65778(Part65E48 *self);
int func_08A65794(Part65E48 *self);
unsigned char func_08A65904(Part65E48 *self);

/* Part7ABD0 */
Part7ABD0 *func_08A65E58(Part7ABD0 *self);
void func_08A65EA8(Part7ABD0 *self, int flags);
void func_08A65F24(Part7ABD0 *self);
void func_08A65F70(Part7ABD0 *self);
int func_08A94228(void *object);
void func_08A94260(void *object, int flags);
void func_08A94300(void *object);

/* Part663E4 */
unsigned char func_08A663E4(Part663E4 *self);
unsigned char func_08A66404(Part663E4 *self);
unsigned char func_08A66424(Part663E4 *self);

/* Part7E728 */
Part7E728 *func_08A7B1D0(Part7E728 *self);
void func_08A7B208(Part7E728 *self, int flags);
void func_08A7B260(Part7E728 *self);
void func_08A7B288(Part7E728 *self);
unsigned char func_08A7B448(Part7E728 *self);

/* Part7E948 */
Part7E948 *func_08A7E740(Part7E948 *self);
void func_08A7E778(Part7E948 *self, int flags);
void func_08A7E7D0(Part7E948 *self);
void func_08A7E7E0(Part7E948 *self);
int func_08A7E7FC(Part7E948 *self);

/* Part7FF38 */
Part7FF38 *func_08A7E958(Part7FF38 *self);
void func_08A7E990(Part7FF38 *self, int flags);
void func_08A7E9E8(Part7FF38 *self);
void func_08A7EA74(Part7FF38 *self);
unsigned char func_08A7EEF4(Part7FF38 *self);
int func_08A7F12C(Part7FF38 *self);

/* Part80208 */
Part80208 *func_08A800A0(Part80208 *self);
void func_08A800E0(Part80208 *self, int flags);
void func_08A8014C(Part80208 *self);
void func_08A80154(Part80208 *self);
int func_08A80188(Part80208 *self);
void func_08A801E4(Part80208 *self);
Part80208 *func_08A80200(Part80208 *self);
int func_08AAAE10(void *object);
void func_08AAB86C(void *object, int flags);
void func_08AAD54C(void *object);
void func_08AA56C8(void *object);
int func_08AA56E4(void *object);

/* Vector helpers on D_09AF84E0. */
void func_08A806E8(BattleVec4 *out, const BattleVec4 *value);
void func_08A8070C(BattleVec4 *out, const BattleVec4 *value);
void func_089A3B30(BattleVec4 *out, const BattleVec4 *a, const BattleVec4 *b);

/* Part815C8 */
Part815C8 *func_08A80770(Part815C8 *self);
void func_08A807A8(Part815C8 *self, int flags);
void func_08A80800(Part815C8 *self);
void func_08A80884(Part815C8 *self);
BattleVec3 *func_089A34E0(BattleVec3 *vector);

#ifdef __cplusplus
}
#endif

#endif
