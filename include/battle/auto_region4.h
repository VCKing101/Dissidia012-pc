#ifndef DISSIDIA_BATTLE_AUTO_REGION4_H
#define DISSIDIA_BATTLE_AUTO_REGION4_H

/* Classes behind the automatically matched functions in battle
 * 0x08A810A0..0x08AF4CDC (readability pass, region 4).
 *
 * Same conventions as auto_region1.h .. auto_region3.h, which this header
 * builds on: "Part" structs keep their vtable pointer after their data and
 * are named after the address of their vtable (or, without one, of their
 * first function); a part's first byte is cleared by its reset and tested
 * first by its getters, so it is called `active`. "Obj" structs are classes
 * with the vtable at offset 0 and no base constructor; their sizes come from
 * the spacing of the members of Part88B28, which holds one of each. Objects of
 * classes outside the region are byte arrays named after the function that
 * builds them.
 *
 * Classes holding a BattleVec4 (Part86C80, Part870D8, Part97108, Obj90E60,
 * Obj92330, Part86940) are placed on 16-byte boundaries by their owners and
 * padded to a multiple of 16; the padding is spelled out here.
 *
 * Several small classes are built inline: Inline2 (two words, then the
 * vtable) and Inline4 (four words, then the vtable). Each instance gets the
 * vtable of its own template instantiation from the owner's constructor.
 *
 * Constructors, destructors and getters that auto_region3.h already declares
 * with `void *` parameters (func_08A870F0, func_08A96398, func_08AB2EF0 ...)
 * keep that signature; their definitions take a typed alias. */

#include "battle/auto_region3.h"

#ifdef __cplusplus
extern "C" {
#endif

#define REGION4_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define REGION4_ASSERT(name, cond) typedef char region4_assert_##name[(cond) ? 1 : -1]
#define REGION4_SIZE(type, size) REGION4_ASSERT(size_##type, sizeof(type) == (size))
#define REGION4_AT(type, field, offset) \
    REGION4_ASSERT(at_##type##_##field, REGION4_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ small shapes */

/* Two words, then the vtable (built inline by its owner's constructor). */
typedef struct Inline2 {
    int field_00;
    int field_04;
    void *vtable;
} Inline2;
REGION4_SIZE(Inline2, 0xC);

/* Four words, then the vtable (built inline by its owner's constructor). */
typedef struct Inline4 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} Inline4;
REGION4_SIZE(Inline4, 0x14);

/* Head of a linked list: cleared second word first (func_08AE9638 and the
 * inline copies in PartC03B8); func_08AE96D4 walks it from `first`. */
typedef struct ListHead {
    void *first;
    int field_04;
    int field_08;
} ListHead;
REGION4_SIZE(ListHead, 0xC);

/* ------------------------------------------------------------ parts */

/* 0x14 bytes. Reset func_08A81668, release func_08A8167C. */
typedef struct Part81E28 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} Part81E28;
REGION4_SIZE(Part81E28, 0x14);

/* 0x14 bytes. Reset func_08A81EC8, release func_08A81EDC. */
typedef struct Part82158 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} Part82158;
REGION4_SIZE(Part82158, 0x14);

/* 0x10 bytes. Reset func_08A821F8, release func_08A82220. */
typedef struct Part823D8 {
    unsigned char active;
    char pad_01[0xB];
    void *vtable;
} Part823D8;
REGION4_SIZE(Part823D8, 0x10);

/* 0x1B4 bytes. Reset func_08A82478, release func_08A824BC. */
typedef struct Part83A38 {
    unsigned char active;
    char pad_01[0x1AF];
    void *vtable;
} Part83A38;
REGION4_SIZE(Part83A38, 0x1B4);

/* 0x60 bytes. Reset func_08A85C90, release func_08A85D14. */
typedef struct Part86940 {
    unsigned char active;
    char pad_01[0xB];
    unsigned char field_0C;
    char pad_0D[3];
    BattleVec4 field_10;
    int field_20;
    char pad_24[0x38];
    void *vtable;
} Part86940;
REGION4_SIZE(Part86940, 0x60);
REGION4_AT(Part86940, field_20, 0x20);

/* 0x30 bytes (0x2C and padding). Constructor func_08A86958, reset
 * func_08A869E8, release func_08A86A20. */
typedef struct Part86C80 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    char pad_08[8];
    BattleVec4 field_10;
    int field_20;
    unsigned char field_24;
    char pad_25[3];
    void *vtable;
    char pad_2C[4];
} Part86C80;
REGION4_SIZE(Part86C80, 0x30);
REGION4_AT(Part86C80, vtable, 0x28);

/* 0xC0 bytes (0xB8 and padding). Reset func_08A86D58, release func_08A86D94. */
typedef struct Part870D8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    char pad_0C[4];
    BattleVec4 field_10;
    Part86C80 field_20[3];
    int field_B0;
    void *vtable;
    char pad_B8[8];
} Part870D8;
REGION4_SIZE(Part870D8, 0xC0);
REGION4_AT(Part870D8, field_B0, 0xB0);

/* 0x1C bytes. Reset func_08A94E70, release func_08A94E8C. field_04 points to
 * 0xC-byte entries whose first word is a state (1 while listed, 2 once
 * removed); field_08 lists entry indices, field_0C counts them. */
typedef struct Entry956C8 {
    int state;
    char pad_04[8];
} Entry956C8;

typedef struct Part956C8 {
    unsigned char active;
    char pad_01[3];
    Entry956C8 *entries;
    int *list;
    int count;
    int field_10;
    int field_14;
    void *vtable;
} Part956C8;
REGION4_SIZE(Part956C8, 0x1C);

/* 0x28 bytes. Reset func_08A942B8, release func_08A94300. */
typedef struct Part94DB8 {
    unsigned char active;
    char pad_01[0x23];
    void *vtable;
} Part94DB8;
REGION4_SIZE(Part94DB8, 0x28);

/* 0xF0 bytes. Reset func_08A95770, release func_08A95874. */
typedef struct Part96380 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    char pad_08[0xE4];
    void *vtable;
} Part96380;
REGION4_SIZE(Part96380, 0xF0);

/* 0x14 bytes. Reset func_08AB37E8, release func_08AB37FC. */
typedef struct PartB3A30 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} PartB3A30;
REGION4_SIZE(PartB3A30, 0x14);

/* 0x80 bytes (0x78 and padding). Reset func_08A96444, release func_08A9649C.
 * Most methods forward to the PartB3A30 at field_0C. */
typedef struct Part97108 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    PartB3A30 field_0C;
    unsigned char field_20;
    unsigned char field_21;
    char pad_22[0xE];
    BattleVec4 field_30;
    BattleVec4 field_40;
    unsigned char field_50;
    unsigned char field_51;
    unsigned char field_52;
    char pad_53[0xD];
    BattleVec4 field_60;
    int field_70;
    void *vtable;
    char pad_78[8];
} Part97108;
REGION4_SIZE(Part97108, 0x80);
REGION4_AT(Part97108, field_70, 0x70);

/* 0x194 bytes. Reset func_08A971A8, release func_08A97240. */
typedef struct Part98058 {
    unsigned char active;
    char pad_01[0x13];
    unsigned int field_14;
    unsigned int field_18;
    int field_1C;
    char pad_20[0xC];
    unsigned int field_2C;
    char pad_30[4];
    unsigned char field_34;
    char pad_35[0x127];
    unsigned char field_15C[0x28];
    int field_184;
    unsigned char field_188;
    char pad_189[3];
    int field_18C;
    void *vtable;
} Part98058;
REGION4_SIZE(Part98058, 0x194);
REGION4_AT(Part98058, field_15C, 0x15C);

/* A node of the list Part9A1A0 keeps at field_64. */
typedef struct Node9A1A0 {
    char pad_00[2];
    unsigned char field_02;
    char pad_03[0x2D];
    struct Node9A1A0 *next;
} Node9A1A0;
REGION4_AT(Node9A1A0, next, 0x30);

/* 0x94 bytes. Reset func_08A98120, release func_08A981C0. */
typedef struct Part9A1A0 {
    unsigned char active;
    char pad_01[0x57];
    int field_58;
    int field_5C;
    int field_60;
    Node9A1A0 *field_64;
    int field_68;
    int field_6C;
    char pad_70[0x20];
    void *vtable;
} Part9A1A0;
REGION4_SIZE(Part9A1A0, 0x94);

/* 0x14 bytes. Reset func_08A9A248, release func_08A9A25C. */
typedef struct Part9A528 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} Part9A528;
REGION4_SIZE(Part9A528, 0x14);

/* 0x2C bytes. Reset func_08A9A5DC, release func_08A9A5EC. */
typedef struct Part9ABE8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    Part7E948 field_0C;
    char pad_1C[0xC];
    void *vtable;
} Part9ABE8;
REGION4_SIZE(Part9ABE8, 0x2C);

/* 0x170 bytes. Reset func_08A9ACBC, release func_08A9ADA8. */
typedef struct PartA1BD8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    char pad_08[0xB0];
    int field_B8;
    char pad_BC[4];
    int field_C0;
    char pad_C4[0x84];
    Part7E948 field_148;
    char pad_158[0x14];
    void *vtable;
} PartA1BD8;
REGION4_SIZE(PartA1BD8, 0x170);
REGION4_AT(PartA1BD8, field_C0, 0xC0);

/* 0x30 bytes. Reset func_08AA1F74, release func_08AA1F8C. */
typedef struct PartA29D8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    unsigned char field_10;
    char pad_11[3];
    Part7E948 field_14;
    char pad_24[8];
    void *vtable;
} PartA29D8;
REGION4_SIZE(PartA29D8, 0x30);

/* 0x74 bytes. Reset func_08AA2A94, release func_08AA2AB8. */
typedef struct PartA4250 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    Part7E948 field_08;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    char pad_2C[0x44];
    void *vtable;
} PartA4250;
REGION4_SIZE(PartA4250, 0x74);

/* 0x28 bytes. Reset func_08AA4300, release func_08AA4328. field_0C points to
 * two-byte slots whose first byte is a state (1 while listed, 2 once
 * removed); field_10 lists slot indices (field_14 of them), field_1C records
 * every index added (field_18 of them). */
typedef struct SlotA4CA0 {
    char state;
    char pad_01;
} SlotA4CA0;

typedef struct PartA4CA0 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    unsigned char field_08;
    char pad_09[3];
    SlotA4CA0 *slots;
    short *list;
    int count;
    int added;
    short *history;
    unsigned char field_20;
    char pad_21[3];
    void *vtable;
} PartA4CA0;
REGION4_SIZE(PartA4CA0, 0x28);

/* 0x1C bytes. Reset func_08AA4D40, release func_08AA4D64. */
typedef struct PartA5610 {
    unsigned char active;
    char pad_01[0x17];
    void *vtable;
} PartA5610;
REGION4_SIZE(PartA5610, 0x1C);

/* 0x0C bytes. Reset func_08AA56C0, release func_08AA56C8. */
typedef struct PartA5818 {
    unsigned char active;
    char pad_01[7];
    void *vtable;
} PartA5818;
REGION4_SIZE(PartA5818, 0xC);

/* 0x48 bytes, no vtable. Constructor func_08AA5830, reset func_08AA58A4,
 * release func_08AA5914. field_08 points to 0x14-byte entries whose first
 * word is a state (1 while listed, 2 once removed); field_30 lists entry
 * indices, field_34 counts them. */
typedef struct EntryA5830 {
    int state;
    char pad_04[0x10];
} EntryA5830;

typedef struct PartA5830 {
    unsigned char active;
    unsigned char field_01;
    unsigned char field_02;
    char pad_03[5];
    EntryA5830 *entries;
    char pad_0C[0x24];
    int *list;
    int count;
    char pad_38[0x10];
} PartA5830;
REGION4_SIZE(PartA5830, 0x48);
REGION4_AT(PartA5830, list, 0x30);

/* 0xD4 bytes. Reset func_08AA6B6C, release func_08AA6B84, destructor
 * func_08AA69DC. The members are built inline by the constructor. */
typedef struct PartA76A0 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    Inline4 field_0C;
    Inline2 field_20;
    Inline2 field_2C;
    Inline4 field_38;
    Inline4 field_4C;
    Inline2 field_60;
    Inline4 field_6C;
    Inline2 field_80;
    Inline2 field_8C;
    Inline2 field_98;
    Inline2 field_A4;
    Inline2 field_B0;
    Inline2 field_BC;
    int field_C8;
    int field_CC;
    void *vtable;
} PartA76A0;
REGION4_SIZE(PartA76A0, 0xD4);
REGION4_AT(PartA76A0, field_BC, 0xBC);

/* 0x1B4 bytes. Reset func_08AA7840 points field_04 at the first PartA76A0. */
typedef struct PartAAD68 {
    unsigned char active;
    char pad_01[3];
    PartA76A0 *current;
    PartA76A0 field_08;
    PartA76A0 field_DC;
    void *vtable;
} PartAAD68;
REGION4_SIZE(PartAAD68, 0x1B4);

/* 0x2CC bytes: the base of Part80208 (auto_region3.h). Reset func_08AABD2C,
 * destructor func_08AAB86C, release func_08AAD54C. Most members are built
 * inline by the constructor; the two arrays hold 0x24-byte objects built by
 * func_08AAADCC and func_08AAAD88. */
typedef struct PartADF68 {
    unsigned char active;
    char pad_01[3];
    Part82158 field_04;
    Inline2 field_18;
    Inline2 field_24;
    Inline2 field_30;
    Inline2 field_3C;
    Inline2 field_48;
    Inline2 field_54;
    Inline2 field_60;
    Inline2 field_6C;
    Inline2 field_78;
    Inline2 field_84;
    Inline2 field_90;
    Inline2 field_9C;
    Inline2 field_A8;
    Inline2 field_B4;
    Inline2 field_C0;
    Inline2 field_CC;
    Inline2 field_D8;
    Inline2 field_E4;
    Inline2 field_F0;
    Inline2 field_FC;
    unsigned char field_108[2][0x24];   /* built by func_08AAADCC */
    Inline2 field_150;
    Inline2 field_15C;
    Inline2 field_168;
    Inline2 field_174;
    Inline2 field_180;
    int field_18C;
    Inline2 field_190;
    Inline2 field_19C;
    Inline2 field_1A8;
    Inline2 field_1B4;
    Inline2 field_1C0;
    Inline2 field_1CC;
    Inline2 field_1D8;
    Inline2 field_1E4;
    Inline2 field_1F0;
    unsigned char field_1FC[3][0x24];   /* built by func_08AAAD88 */
    Inline2 field_268;
    Inline2 field_274;
    Inline2 field_280;
    Inline2 field_28C;
    Inline2 field_298;
    Inline2 field_2A4;
    Inline2 field_2B0;
    Inline2 field_2BC;
    void *vtable;
} PartADF68;
REGION4_SIZE(PartADF68, 0x2CC);
REGION4_AT(PartADF68, field_108, 0x108);
REGION4_AT(PartADF68, field_18C, 0x18C);
REGION4_AT(PartADF68, field_1FC, 0x1FC);
REGION4_AT(PartADF68, field_280, 0x280);

/* 0x14 bytes. Reset func_08AAE218, release func_08AAE22C. */
typedef struct PartB2EB8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} PartB2EB8;
REGION4_SIZE(PartB2EB8, 0x14);

/* 0x510 bytes (0x504 and padding): Part64E48's field_20. Reset
 * func_08AB300C, release func_08AB3014; the getters at 0x08AB3264..
 * 0x08AB329C return its members. */
typedef struct PartB32A8 {
    unsigned char active;
    char pad_01[3];
    Part98058 field_04;
    Part83A38 field_198;
    PartB2EB8 field_34C;
    Part65650 field_360;
    Part7E728 field_36C;
    Part7FF38 field_390;
    char pad_434[0xC];
    Part870D8 field_440;
    void *vtable;
    char pad_504[0xC];
} PartB32A8;
REGION4_SIZE(PartB32A8, 0x510);
REGION4_AT(PartB32A8, field_198, 0x198);
REGION4_AT(PartB32A8, field_390, 0x390);
REGION4_AT(PartB32A8, field_440, 0x440);
REGION4_AT(PartB32A8, vtable, 0x500);

/* 0x0C bytes. Reset func_08AB3AD0, release func_08AB3AE0. */
typedef struct PartB3C90 {
    unsigned char active;
    char pad_01[7];
    void *vtable;
} PartB3C90;
REGION4_SIZE(PartB3C90, 0xC);

/* 0x44 bytes. Reset func_08AB3D48, release func_08AB3D80. */
typedef struct PartB4240 {
    unsigned char active;
    char pad_01[0x3F];
    void *vtable;
} PartB4240;
REGION4_SIZE(PartB4240, 0x44);

/* Derives from the class built by func_0883975C (destroyed by func_088310BC)
 * and keeps its vtable after that base. Reset func_08AB6884. */
typedef struct PartB71A0 {
    unsigned char base[0x544];
    void *vtable;
    char pad_548[8];
    unsigned char field_550;
    char pad_551[3];
    int field_554;
    int field_558;
    int field_55C;
    int field_560;
    unsigned char field_564;
    unsigned char field_565;
    char pad_566[2];
    int field_568;
} PartB71A0;
REGION4_AT(PartB71A0, field_550, 0x550);
REGION4_AT(PartB71A0, field_568, 0x568);

/* 0x08 bytes. Reset func_08AB7470, release func_08AB7498. */
typedef struct PartB7560 {
    unsigned char active;
    char pad_01[3];
    void *vtable;
} PartB7560;
REGION4_SIZE(PartB7560, 0x8);

/* 0x10 bytes. Reset func_08AB7600, release func_08AB7610. */
typedef struct PartB7ED0 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    void *vtable;
} PartB7ED0;
REGION4_SIZE(PartB7ED0, 0x10);

/* 0xAC bytes, no vtable. Constructor func_08AB8390, reset func_08AB8404,
 * release func_08AB843C. */
typedef struct PartB8390 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    unsigned char field_08;
    char pad_09[3];
    int field_0C[10];
    int field_34[10];
    int field_5C[10];
    int field_84[10];
} PartB8390;
REGION4_SIZE(PartB8390, 0xAC);

/* 0x10 bytes. Reset func_08AB88F8, release func_08AB8908. */
typedef struct PartB97D0 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    void *vtable;
} PartB97D0;
REGION4_SIZE(PartB97D0, 0x10);

/* 0x60 bytes. Reset func_08AB9B18, release func_08AB9B24, destructor
 * func_08AB9A18. The members are built inline by the constructor. */
typedef struct PartB9F28 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    Inline2 field_08;
    Inline2 field_14;
    Inline2 field_20;
    Inline2 field_2C;
    Inline2 field_38;
    Inline2 field_44;
    Inline2 field_50;
    void *vtable;
} PartB9F28;
REGION4_SIZE(PartB9F28, 0x60);

/* 0x14 bytes. Reset func_08ABA098, release func_08ABA0B4. */
typedef struct PartBA7C8 {
    unsigned char active;
    char pad_01[0xF];
    void *vtable;
} PartBA7C8;
REGION4_SIZE(PartBA7C8, 0x14);

/* 0x24 bytes. Reset func_08ABA878, release func_08ABA8AC. */
typedef struct PartBAC98 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10[2];
    int field_18[2];
    void *vtable;
} PartBAC98;
REGION4_SIZE(PartBAC98, 0x24);

/* 0x54 bytes. Reset func_08ABC0D0, release func_08ABC0DC, destructor
 * func_08ABBFE8. The members are built inline by the constructor. */
typedef struct PartBC580 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    Inline2 field_08;
    Inline2 field_14;
    Inline2 field_20;
    Inline2 field_2C;
    Inline2 field_38;
    Inline2 field_44;
    void *vtable;
} PartBC580;
REGION4_SIZE(PartBC580, 0x54);

/* No vtable. Set up by func_08ABC608 (owner and position), reset by
 * func_08ABC6B0. */
typedef struct PartBC608 {
    void *owner;
    char pad_04[4];
    BattleVec3 field_08;
    char pad_14[0x14];
    int field_28;
    int field_2C;
    char pad_30[0x30];
    unsigned char field_60;
} PartBC608;
REGION4_AT(PartBC608, field_28, 0x28);
REGION4_AT(PartBC608, field_60, 0x60);

/* 0x10 bytes. Reset func_08ABD660, release func_08ABD670. */
typedef struct PartBDE18 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    char pad_08[4];
    void *vtable;
} PartBDE18;
REGION4_SIZE(PartBDE18, 0x10);

/* 0x80 bytes. Reset func_08ABDF14, release func_08ABDF94. */
typedef struct PartC03B8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    ListHead field_1C;
    ListHead field_28;
    ListHead field_34;
    ListHead field_40;
    ListHead field_4C[3];               /* built by func_08ABDE28 */
    int field_70[3];
    void *vtable;
} PartC03B8;
REGION4_SIZE(PartC03B8, 0x80);
REGION4_AT(PartC03B8, field_4C, 0x4C);

/* 0x10 bytes. Reset func_08AC0B60, release func_08AC0B70. */
typedef struct PartC0FA0 {
    unsigned char active;
    char pad_01[0xB];
    void *vtable;
} PartC0FA0;
REGION4_SIZE(PartC0FA0, 0x10);

/* 0x38 bytes. Reset func_08AC1F08, release func_08AC1F48. */
typedef struct PartC3C00 {
    unsigned char active;
    char pad_01[0x33];
    void *vtable;
} PartC3C00;
REGION4_SIZE(PartC3C00, 0x38);

/* 0x1F4 bytes. Reset func_08ABAE18, release func_08ABAE94. */
typedef struct PartBBDE8 {
    PartB9F28 field_00;
    PartBC580 field_60;
    PartBAC98 field_B4;
    char pad_D8[4];
    PartC03B8 field_DC;
    PartBDE18 field_15C;
    PartC3C00 field_16C;
    PartBA7C8 field_1A4;
    PartC0FA0 field_1B8;
    PartB7ED0 field_1C8;
    PartB7560 field_1D8;
    char pad_1E0[0x10];
    void *vtable;
} PartBBDE8;
REGION4_SIZE(PartBBDE8, 0x1F4);
REGION4_AT(PartBBDE8, field_DC, 0xDC);
REGION4_AT(PartBBDE8, field_1D8, 0x1D8);

/* 0x20 bytes. Reset func_08ACCE58. */
typedef struct PartCD0C8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    void *vtable;
} PartCD0C8;
REGION4_SIZE(PartCD0C8, 0x20);

/* No vtable. Constructor func_08ACEE28, reset func_08ACEE90, release
 * func_08ACEEA8. */
typedef struct PartCEE28 {
    unsigned char active;
} PartCEE28;

/* Derives from the class built by func_08AD29D8 (destroyed by
 * func_08AD2A10, freed by func_08AD2F38) and keeps its vtable after that
 * base. field_124..field_12C are handles freed with func_088A6C70. */
typedef struct PartD9580 {
    unsigned char base[0xF8];
    void *vtable;
    char pad_FC[4];
    int field_100;
    int field_104;
    char pad_108[0x1C];
    void *field_124;
    void *field_128;
    void *field_12C;
    int field_130;
    unsigned char field_134;
} PartD9580;
REGION4_AT(PartD9580, field_100, 0x100);
REGION4_AT(PartD9580, field_124, 0x124);
REGION4_AT(PartD9580, field_134, 0x134);

/* The same base as PartD9580, another derived class. */
typedef struct PartDB2A0 {
    unsigned char base[0xF8];
    void *vtable;
} PartDB2A0;

/* ------------------------------------------------------------ objects */

/* 0x1C bytes. Reset func_08A88C30, release func_08A88C58. */
typedef struct Obj891C8 {
    void *vtable;
    char pad_04[0x18];
} Obj891C8;
REGION4_SIZE(Obj891C8, 0x1C);

/* 0x14 bytes. Reset func_08A892D8, release func_08A892F0. */
typedef struct Obj89768 {
    void *vtable;
    char pad_04[0x10];
} Obj89768;
REGION4_SIZE(Obj89768, 0x14);

/* 0x1C bytes. Reset func_08A89880, release func_08A898A4. */
typedef struct Obj89D90 {
    void *vtable;
    char pad_04[0x18];
} Obj89D90;
REGION4_SIZE(Obj89D90, 0x1C);

/* 0x10 bytes. Reset func_08A89EA0, release func_08A89EB4. */
typedef struct Obj8A2C0 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    int field_0C;
} Obj8A2C0;
REGION4_SIZE(Obj8A2C0, 0x10);

/* 0x220 bytes. Reset func_08A8A484, release func_08A8A4B0. */
typedef struct Obj8EFC0 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    int field_0C;
    int field_10;
    unsigned char field_14;
    unsigned char field_15;
    unsigned char field_16;
    char pad_17[0x1D];
    int field_34;
    int field_38;
    Part7E948 field_3C;
    PartA4CA0 field_4C;
    Part81E28 field_74;
    Part9A1A0 field_88;
    PartA5830 field_11C;
    Part956C8 field_164;
    Part823D8 field_180;
    Part94DB8 field_190;
    char pad_1B8[8];
    Part86940 field_1C0;
} Obj8EFC0;
REGION4_SIZE(Obj8EFC0, 0x220);
REGION4_AT(Obj8EFC0, field_34, 0x34);
REGION4_AT(Obj8EFC0, field_88, 0x88);
REGION4_AT(Obj8EFC0, field_190, 0x190);

/* 0x20 bytes. Reset func_08A8F0E8, release func_08A8F108. */
typedef struct Obj8F738 {
    void *vtable;
    char pad_04[0x1C];
} Obj8F738;
REGION4_SIZE(Obj8F738, 0x20);

/* 0x40 bytes. Reset func_08A8F850, release func_08A8F8A0. */
typedef struct Obj90000 {
    void *vtable;
    char pad_04[0x3C];
} Obj90000;
REGION4_SIZE(Obj90000, 0x40);

/* 0x20 bytes. Reset func_08A90120, release func_08A90144. */
typedef struct Obj90688 {
    void *vtable;
    char pad_04[0x1C];
} Obj90688;
REGION4_SIZE(Obj90688, 0x20);

/* 0x50 bytes. Reset func_08A907AC, release func_08A907EC. */
typedef struct Obj90E60 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    int field_0C;
    int field_10;
    char pad_14[0xC];
    BattleVec4 field_20;
    int field_30;
    Part823D8 field_34;
    char pad_44[0xC];
} Obj90E60;
REGION4_SIZE(Obj90E60, 0x50);
REGION4_AT(Obj90E60, field_34, 0x34);

/* 0x20 bytes. Reset func_08A90F78, release func_08A90F9C. */
typedef struct Obj91508 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    int field_0C;
    int field_10;
    unsigned char field_14;
    char pad_15[3];
    int field_18;
    unsigned char field_1C;
    char pad_1D[3];
} Obj91508;
REGION4_SIZE(Obj91508, 0x20);

/* 0x20 bytes. Reset func_08A91618, release func_08A91634. */
typedef struct Obj91A50 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    int field_0C;
    char pad_10[4];
    unsigned char field_14;
    char pad_15[3];
    int field_18;
    unsigned char field_1C;
    char pad_1D[3];
} Obj91A50;
REGION4_SIZE(Obj91A50, 0x20);

/* 0x20 bytes. Reset func_08A91B60, release func_08A91B74. */
typedef struct Obj91E20 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    int field_10;
    char pad_14[0xC];
} Obj91E20;
REGION4_SIZE(Obj91E20, 0x20);

/* 0x30 bytes. Reset func_08A91F30, release func_08A91F68. */
typedef struct Obj92330 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    char pad_0C[4];
    BattleVec4 field_10;
    int field_20;
    int field_24;
    char pad_28[8];
} Obj92330;
REGION4_SIZE(Obj92330, 0x30);

/* 0x20 bytes. Reset func_08A92440, release func_08A92454. */
typedef struct Obj92890 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    int field_10;
    char pad_14[0xC];
} Obj92890;
REGION4_SIZE(Obj92890, 0x20);

/* 0x30 bytes. Reset func_08A929A0, release func_08A929B0. */
typedef struct Obj941F0 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    char pad_0C[0x1C];
    unsigned char field_28;
    char pad_29[7];
} Obj941F0;
REGION4_SIZE(Obj941F0, 0x30);

/* 0x660 bytes: Part64E48's field_5B0. Reset func_08A872E8, release
 * func_08A87328. */
typedef struct Part88B28 {
    unsigned char active;
    char pad_01[0x13];
    unsigned char field_14;
    char pad_15[0x17];
    Part7E948 field_2C;
    char pad_3C[4];
    Part96380 field_40;
    Part96380 field_130;
    char pad_220[0x10];
    Obj8EFC0 field_230;
    Obj90688 field_450;
    Obj941F0 field_470;
    Obj91508 field_4A0;
    Obj91E20 field_4C0;
    Obj92330 field_4E0;
    Obj91A50 field_510;
    Obj92890 field_530;
    Obj90000 field_550;
    Obj8F738 field_590;
    Obj90E60 field_5B0;
    Obj89D90 field_600;
    Obj8A2C0 field_61C;
    Obj891C8 field_62C;
    Obj89768 field_648;
    void *vtable;
} Part88B28;
REGION4_SIZE(Part88B28, 0x660);
REGION4_AT(Part88B28, field_2C, 0x2C);
REGION4_AT(Part88B28, field_40, 0x40);
REGION4_AT(Part88B28, field_230, 0x230);
REGION4_AT(Part88B28, field_5B0, 0x5B0);
REGION4_AT(Part88B28, field_648, 0x648);

/* ------------------------------------------------------------ fighters */

/* What the fighter's field_52C points to. */
typedef struct FighterModel52C {
    char pad_00[0x14];
    short field_14;
} FighterModel52C;

/* More fields of the fighter (BattleFighter in auto_region3.h has the same
 * motion and flags); merge the two when the headers are consolidated. */
typedef struct BattleFighter4 {
    char pad_000[0x30C];
    char *field_30C;
    char pad_310[0x74];
    unsigned int flags;
    char pad_388[0x1A4];
    FighterModel52C *field_52C;
    char pad_530[0xB9C];
    short field_10CC;
} BattleFighter4;
REGION4_AT(BattleFighter4, field_30C, 0x30C);
REGION4_AT(BattleFighter4, flags, 0x384);
REGION4_AT(BattleFighter4, field_52C, 0x52C);
REGION4_AT(BattleFighter4, field_10CC, 0x10CC);

/* ------------------------------------------------------------ PartE7B50 */

/* Keeps its vtable at field_04. field_08 is a phase, field_10 the fighter;
 * field_30 lists Node9520 objects; field_2A8 belongs to a class at
 * 0x08ADFB7C..0x08ADFDDC. */
typedef struct PartE7B50 {
    int field_00;
    void *vtable;
    int phase;
    char pad_0C[4];
    BattleFighter4 *fighter;
    char pad_14[8];
    int field_1C;
    char pad_20[0x10];
    ListHead field_30;
    char pad_3C[0x224];
    unsigned char field_260;
    unsigned char field_261;
    char pad_262[0x2A];
    int field_28C[3];
    char pad_298;
    unsigned char field_299;
    char pad_29A[2];
    int field_29C;
    char pad_2A0[8];
    unsigned char field_2A8[4];
} PartE7B50;
REGION4_AT(PartE7B50, field_30, 0x30);
REGION4_AT(PartE7B50, field_260, 0x260);
REGION4_AT(PartE7B50, field_28C, 0x28C);
REGION4_AT(PartE7B50, field_29C, 0x29C);
REGION4_AT(PartE7B50, field_2A8, 0x2A8);

/* An object on PartE7B50's list: flags at field_28, the next one at
 * field_2C. */
typedef struct Node9520 {
    char pad_00[0x28];
    unsigned int flags;
    struct Node9520 *next;
} Node9520;
REGION4_AT(Node9520, next, 0x2C);

/* ------------------------------------------------------------ small classes */

/* Cleared by func_08AB54C0 (no vtable). */
typedef struct PartB54C0 {
    int field_00;
    char pad_04[8];
    int field_0C;
    int field_10;
    char pad_14[8];
    int field_1C;
    int field_20;
} PartB54C0;
REGION4_AT(PartB54C0, field_20, 0x20);

/* Built by func_08AB7F18 (no vtable). */
typedef struct PartB7F18 {
    int field_00;
    char pad_04[0x20];
    int field_24[8];
} PartB7F18;
REGION4_AT(PartB7F18, field_24, 0x24);

/* What func_08ABC300 finds. */
typedef struct EntryBC300 {
    char pad_00[0x48];
    int field_48;
} EntryBC300;

/* Three handles from func_0899B9F0 (freed with func_0899BB1C) and one object
 * freed with func_08842138. */
typedef struct PartC6098 {
    void *handles[3];
    void *field_0C;
    int field_10;
} PartC6098;
REGION4_SIZE(PartC6098, 0x14);

/* Built by func_08AC8F00 (no vtable); the layout is not known. */
typedef struct PartC8F00 PartC8F00;

/* A node of the list walked by func_08AD04D8: state at field_80, flags at
 * field_84, the next node at field_F4. */
typedef struct NodeD2ABC {
    char pad_00[0x80];
    int state;
    unsigned int flags;
    char pad_88[0x6C];
    struct NodeD2ABC *next;
} NodeD2ABC;
REGION4_AT(NodeD2ABC, flags, 0x84);
REGION4_AT(NodeD2ABC, next, 0xF4);

/* Two small classes with the vtable at offset 0 and one word of data. */
typedef struct ObjD2988 {
    void *vtable;
    int field_04;
} ObjD2988;

typedef struct ObjD29B0 {
    void *vtable;
    int field_04;
} ObjD29B0;

/* Set up by func_08AF2CBC; field_04 and field_18 are objects of classes in
 * the main module (func_088EC500 / func_088EC658 / func_088EC67C and
 * func_088EC2A8), field_3C a name copied with func_089359A4. */
typedef struct PartF2CBC {
    char pad_00[4];
    unsigned char field_04[0x14];
    unsigned char field_18[0xC];
    void *field_24;
    char pad_28[0xC];
    int field_34;
    int field_38;
    char field_3C[0x20];
    unsigned int field_5C;
    char pad_60[4];
    int field_64;
    int field_68;
    int field_6C;
    char pad_70;
    unsigned char field_71;
    unsigned char field_72;
    unsigned char field_73;
    int field_74;
} PartF2CBC;
REGION4_AT(PartF2CBC, field_24, 0x24);
REGION4_AT(PartF2CBC, field_5C, 0x5C);
REGION4_AT(PartF2CBC, field_74, 0x74);

/* 0x14 bytes. Constructor func_08AF3E08, destructor func_08AF3E2C. The
 * classes with vtables D_08AFB5A0, D_08AFB5E8 and D_08AFB630 derive from it
 * and replace its vtable. */
typedef struct PartFB558 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} PartFB558;
REGION4_SIZE(PartFB558, 0x14);

typedef struct PartFB5A0 {
    PartFB558 base;
} PartFB5A0;

typedef struct PartFB5E8 {
    PartFB558 base;
} PartFB5E8;

typedef struct PartFB630 {
    PartFB558 base;
} PartFB630;

/* Vtable at offset 0, destroyed through the main-module base destructor
 * func_088EC234; the member at field_88 is destroyed by func_0887B1F4. */
typedef struct ObjFB520 {
    void *vtable;
    char pad_04[0x84];
    unsigned char field_88[4];
} ObjFB520;
REGION4_AT(ObjFB520, field_88, 0x88);

/* Holds a motion handle at field_04 and the object it drives at field_08. */
typedef struct PartF3D40 {
    char pad_00[4];
    void *field_04;
    void *field_08;
} PartF3D40;

/* A motion handle: func_088D9F5C plays a motion on it. */
typedef struct MotionF4458 {
    char pad_00[0xE];
    short field_0E;
} MotionF4458;

typedef struct PartF4458 {
    char pad_00[0x14];
    MotionF4458 *field_14;
} PartF4458;
REGION4_AT(PartF4458, field_14, 0x14);

/* ------------------------------------------------------------ globals */

typedef struct ManagerE7CA0 {
    char pad_00[8];
    int field_08;
} ManagerE7CA0;

typedef struct Global9D69F8 {
    char pad_00[0x18];
    PartE7B50 *field_18[1];             /* length unknown */
} Global9D69F8;

typedef struct Global9D6A10 {
    char pad_000[0x7A4];
    unsigned int field_7A4;
} Global9D6A10;

typedef struct Global405E0 {
    char pad_000[0x26C];
    int field_26C;
} Global405E0;

extern Part80208 *D_08A80218;
extern PartB4240 *D_08AB4250;
extern void *D_08AB6738;
extern PartBBDE8 *D_08ABBDF8;
extern PartC8F00 *D_08ACA350;
extern PartCEE28 *D_08ACF110;
extern ManagerE7CA0 *D_08AE7CA0;
extern void *D_08AEB9BC;
extern void *D_08AF2BD0;
extern int D_08AF2BD4;
extern Global9D69F8 *D_089D69F8;
extern Global9D6A10 *D_089D6A10;
extern int D_089D4928;
extern void *D_089D492C;
extern int D_089D4930;
extern void *D_089D4934;
extern int D_09AF84D0;                  /* random seed */
extern int D_09B6B824;                  /* random value / seed */
extern int D_09B6C274;
extern int D_09B6B828;
extern int D_09B6B82C;
extern int D_09B6C270;
extern int D_09B6C278;
extern int D_09B6C27C;
extern int D_09B6C280;
extern unsigned char D_09B6B830[];      /* object set up by func_08AEAD20 */
extern unsigned char D_09B49140[];      /* two 0x50-byte objects */

/* ------------------------------------------------------------ vtables */

extern unsigned char D_08A81E28[];
extern unsigned char D_08A82158[];
extern unsigned char D_08A823D8[];
extern unsigned char D_08A83A38[];
extern unsigned char D_08A86940[];
extern unsigned char D_08A86C80[];
extern unsigned char D_08A870D8[];
extern unsigned char D_08A88B28[];
extern unsigned char D_08A891C8[];
extern unsigned char D_08A89768[];
extern unsigned char D_08A89D90[];
extern unsigned char D_08A8A2C0[];
extern unsigned char D_08A8EFC0[];
extern unsigned char D_08A8F738[];
extern unsigned char D_08A90000[];
extern unsigned char D_08A90688[];
extern unsigned char D_08A90E60[];
extern unsigned char D_08A91508[];
extern unsigned char D_08A91A50[];
extern unsigned char D_08A91E20[];
extern unsigned char D_08A92330[];
extern unsigned char D_08A92890[];
extern unsigned char D_08A941F0[];
extern unsigned char D_08A94DB8[];
extern unsigned char D_08A956C8[];
extern unsigned char D_08A96380[];
extern unsigned char D_08A97108[];
extern unsigned char D_08A98058[];
extern unsigned char D_08A9A1A0[];
extern unsigned char D_08A9A528[];
extern unsigned char D_08A9ABE8[];
extern unsigned char D_08AA1BD8[];
extern unsigned char D_08AA29D8[];
extern unsigned char D_08AA4250[];
extern unsigned char D_08AA4CA0[];
extern unsigned char D_08AA5610[];
extern unsigned char D_08AA5818[];
extern unsigned char D_08AA76A0[];
extern unsigned char D_08AA76B0[];
extern unsigned char D_08AA76C0[];
extern unsigned char D_08AA76D0[];
extern unsigned char D_08AA76E0[];
extern unsigned char D_08AA76F0[];
extern unsigned char D_08AA7700[];
extern unsigned char D_08AA7710[];
extern unsigned char D_08AA7720[];
extern unsigned char D_08AA7730[];
extern unsigned char D_08AA7740[];
extern unsigned char D_08AA7750[];
extern unsigned char D_08AA7760[];
extern unsigned char D_08AAAD68[];
extern unsigned char D_08AADF68[];
extern unsigned char D_08AADF78[];
extern unsigned char D_08AADF88[];
extern unsigned char D_08AADF98[];
extern unsigned char D_08AADFA8[];
extern unsigned char D_08AADFB8[];
extern unsigned char D_08AADFC8[];
extern unsigned char D_08AADFD8[];
extern unsigned char D_08AADFE8[];
extern unsigned char D_08AADFF8[];
extern unsigned char D_08AAE008[];
extern unsigned char D_08AAE018[];
extern unsigned char D_08AAE028[];
extern unsigned char D_08AAE048[];
extern unsigned char D_08AAE068[];
extern unsigned char D_08AAE078[];
extern unsigned char D_08AAE088[];
extern unsigned char D_08AAE098[];
extern unsigned char D_08AAE0D8[];
extern unsigned char D_08AAE0E8[];
extern unsigned char D_08AAE0F8[];
extern unsigned char D_08AAE108[];
extern unsigned char D_08AAE118[];
extern unsigned char D_08AAE128[];
extern unsigned char D_08AAE138[];
extern unsigned char D_08AAE148[];
extern unsigned char D_08AAE158[];
extern unsigned char D_08AB2EB8[];
extern unsigned char D_08AB32A8[];
extern unsigned char D_08AB3A30[];
extern unsigned char D_08AB3C90[];
extern unsigned char D_08AB4240[];
extern unsigned char D_08AB71A0[];
extern unsigned char D_08AB7560[];
extern unsigned char D_08AB7ED0[];
extern unsigned char D_08AB97D0[];
extern unsigned char D_08AB9F28[];
extern unsigned char D_08AB9F38[];
extern unsigned char D_08AB9F48[];
extern unsigned char D_08AB9F58[];
extern unsigned char D_08AB9F68[];
extern unsigned char D_08AB9F78[];
extern unsigned char D_08AB9F88[];
extern unsigned char D_08AB9F98[];
extern unsigned char D_08ABA7C8[];
extern unsigned char D_08ABAC98[];
extern unsigned char D_08ABBDE8[];
extern unsigned char D_08ABC580[];
extern unsigned char D_08ABC590[];
extern unsigned char D_08ABC5A0[];
extern unsigned char D_08ABC5B0[];
extern unsigned char D_08ABC5C0[];
extern unsigned char D_08ABC5D0[];
extern unsigned char D_08ABC5E0[];
extern unsigned char D_08ABDE18[];
extern unsigned char D_08AC03B8[];
extern unsigned char D_08AC0FA0[];
extern unsigned char D_08AC3C00[];
extern unsigned char D_08ACD0C8[];
extern unsigned char D_08AD2988[];
extern unsigned char D_08AD29B0[];
extern unsigned char D_08AD9580[];
extern unsigned char D_08ADB2A0[];
extern unsigned char D_08AE7B50[];
extern unsigned char D_08AFB520[];
extern unsigned char D_08AFB558[];
extern unsigned char D_08AFB5A0[];
extern unsigned char D_08AFB5E8[];
extern unsigned char D_08AFB630[];

/* ------------------------------------------------------------ functions */

/* Outside the region. */
BattleVec4 *func_089A3890(BattleVec4 *vector);
void *func_088D0700(void *loader, char arg1, int id, int slot);
void func_088D0A1C(void *loader, void *model);
void func_088D9F5C(void *handle, int motion, char arg2);
void func_088A6C70(void *handle);
int func_088A751C(void *handle);
void *func_0899B9F0(int arg);
int func_0899BB68(void *handle, int arg1, int arg2, int arg3);
void func_0899BB1C(void *handle);
void *func_0883975C(void *object, char arg1, int arg2);
void func_088310BC(void *object, int flags);
int func_08838074(BattleFighter4 *fighter);
void func_08889F70(int id, int arg);
void func_08889FB8(int id, int arg1, char arg2);
int func_088BB708(int arg);
void func_089DDCEC(unsigned int arg);
char *func_089359A4(char *dst, const char *src);
void func_088EC2A8(void *object, int arg);
void func_088EC500(void *object, int arg);
void func_088EC658(void *object);
void func_088EC67C(void *object);
void func_088EC234(void *object, int flags);
void func_0887B1F4(void *object, int flags);
void func_0887B778(void *object, void *matrix);
void func_0887B828(void *object, void *value);
void func_0887B958(void *object);
void func_088DA0A8(void *handle, int arg1, void *matrix, void *value);
int func_08842D18(void *model, int arg);
int func_08B344E4(BattleFighter4 *fighter);

/* Part815C8 (auto_region3.h) */
unsigned char func_08A810A0(Part815C8 *self);
unsigned char func_08A812DC(Part815C8 *self);
int func_08A87AF8(int arg);

/* Part81E28 */
Part81E28 *func_08A815D8(Part81E28 *self);
void func_08A81610(Part81E28 *self, int flags);
void func_08A81668(Part81E28 *self);
void func_08A8167C(Part81E28 *self);

/* Part82158 */
Part82158 *func_08A81E38(Part82158 *self);
void func_08A81E70(Part82158 *self, int flags);
void func_08A81EC8(Part82158 *self);
void func_08A81EDC(Part82158 *self);

/* Part823D8 */
Part823D8 *func_08A82168(Part823D8 *self);
void func_08A821A0(Part823D8 *self, int flags);
void func_08A821F8(Part823D8 *self);
void func_08A82220(Part823D8 *self);
int func_08A8223C(Part823D8 *self);

/* Part83A38 */
Part83A38 *func_08A823E8(Part83A38 *self);
void func_08A82420(Part83A38 *self, int flags);
void func_08A82478(Part83A38 *self);
void func_08A824BC(Part83A38 *self);

/* Part86940 */
Part86940 *func_08A85C00(Part86940 *self);
void func_08A85C38(Part86940 *self, int flags);
void func_08A85C90(Part86940 *self);
void func_08A85D14(Part86940 *self);
unsigned char func_08A85EBC(Part86940 *self);
int func_08A85F80(Part86940 *self, const BattleVec4 *position, int value);
float func_08A860E4(const BattleVec3 *a, const BattleVec3 *b, const BattleVec3 *origin);
float func_08A86264(const BattleVec3 *a, const BattleVec3 *b);

/* Part86C80 */
Part86C80 *func_08A86958(Part86C80 *self);
void func_08A86990(Part86C80 *self, int flags);
void func_08A869E8(Part86C80 *self);
void func_08A86A20(Part86C80 *self);
unsigned char func_08A86C60(Part86C80 *self);

/* Part870D8 */
Part870D8 *func_08A86C90(Part870D8 *self);
void func_08A86CE0(Part870D8 *self, int flags);
void func_08A86D58(Part870D8 *self);
void func_08A86D94(Part870D8 *self);

/* Part88B28 (constructor, destructor and release declared in
 * auto_region3.h) */
void func_08A872E8(Part88B28 *self);
unsigned char func_08A87AD8(Part88B28 *self);

/* Part88B28's members */
Obj891C8 *func_08A88B8C(Obj891C8 *self);
void func_08A88BC4(Obj891C8 *self, int flags);
void func_08A88C30(Obj891C8 *self);
void func_08A88C58(Obj891C8 *self);
Obj89768 *func_08A89234(Obj89768 *self);
void func_08A8926C(Obj89768 *self, int flags);
void func_08A892D8(Obj89768 *self);
void func_08A892F0(Obj89768 *self);
Obj89D90 *func_08A897DC(Obj89D90 *self);
void func_08A89814(Obj89D90 *self, int flags);
void func_08A89880(Obj89D90 *self);
void func_08A898A4(Obj89D90 *self);
Obj8A2C0 *func_08A89DFC(Obj8A2C0 *self);
void func_08A89E34(Obj8A2C0 *self, int flags);
void func_08A89EA0(Obj8A2C0 *self);
void func_08A89EB4(Obj8A2C0 *self);
Obj8EFC0 *func_08A8A32C(Obj8EFC0 *self);
void func_08A8A3AC(Obj8EFC0 *self, int flags);
void func_08A8A484(Obj8EFC0 *self);
void func_08A8A4B0(Obj8EFC0 *self);
int func_08A8A830(void);
Obj8F738 *func_08A8F044(Obj8F738 *self);
void func_08A8F07C(Obj8F738 *self, int flags);
void func_08A8F0E8(Obj8F738 *self);
void func_08A8F108(Obj8F738 *self);
Obj90000 *func_08A8F7AC(Obj90000 *self);
void func_08A8F7E4(Obj90000 *self, int flags);
void func_08A8F850(Obj90000 *self);
void func_08A8F8A0(Obj90000 *self);
Obj90688 *func_08A9007C(Obj90688 *self);
void func_08A900B4(Obj90688 *self, int flags);
void func_08A90120(Obj90688 *self);
void func_08A90144(Obj90688 *self);
Obj90E60 *func_08A906F4(Obj90E60 *self);
void func_08A90734(Obj90E60 *self, int flags);
void func_08A907AC(Obj90E60 *self);
void func_08A907EC(Obj90E60 *self);
Obj91508 *func_08A90ED4(Obj91508 *self);
void func_08A90F0C(Obj91508 *self, int flags);
void func_08A90F78(Obj91508 *self);
void func_08A90F9C(Obj91508 *self);
Obj91A50 *func_08A91574(Obj91A50 *self);
void func_08A915AC(Obj91A50 *self, int flags);
void func_08A91618(Obj91A50 *self);
void func_08A91634(Obj91A50 *self);
Obj91E20 *func_08A91ABC(Obj91E20 *self);
void func_08A91AF4(Obj91E20 *self, int flags);
void func_08A91B60(Obj91E20 *self);
void func_08A91B74(Obj91E20 *self);
Obj92330 *func_08A91E8C(Obj92330 *self);
void func_08A91EC4(Obj92330 *self, int flags);
void func_08A91F30(Obj92330 *self);
void func_08A91F68(Obj92330 *self);
Obj92890 *func_08A9239C(Obj92890 *self);
void func_08A923D4(Obj92890 *self, int flags);
void func_08A92440(Obj92890 *self);
void func_08A92454(Obj92890 *self);
Obj941F0 *func_08A928FC(Obj941F0 *self);
void func_08A92934(Obj941F0 *self, int flags);
void func_08A929A0(Obj941F0 *self);
void func_08A929B0(Obj941F0 *self);

/* Part94DB8 (constructor, destructor and release declared in
 * auto_region3.h) */
void func_08A942B8(Part94DB8 *self);

/* Part956C8 */
Part956C8 *func_08A94DE0(Part956C8 *self);
void func_08A94E18(Part956C8 *self, int flags);
void func_08A94E70(Part956C8 *self);
void func_08A94E8C(Part956C8 *self);
void func_08A955B4(Part956C8 *self, int index);
void func_08A955F4(Part956C8 *self, int slot);
unsigned char func_08A9564C(Part956C8 *self);

/* Part96380 */
Part96380 *func_08A956E0(Part96380 *self);
void func_08A95718(Part96380 *self, int flags);
void func_08A95770(Part96380 *self);
void func_08A95874(Part96380 *self);
int func_08A95890(Part96380 *self);
unsigned char func_08A95C9C(Part96380 *self);

/* Part97108 (constructor, destructor, release and the forwarders
 * func_08A97060..func_08A970D0 declared in auto_region3.h) */
void func_08A96444(Part97108 *self);
unsigned char func_08A969AC(Part97108 *self);
BattleVec4 *func_08A969B4(Part97108 *self);
void func_08A970EC(Part97108 *self);

/* Part98058 (func_08A978F4 and func_08A97908 declared in auto_region3.h) */
Part98058 *func_08A97118(Part98058 *self);
void func_08A97150(Part98058 *self, int flags);
void func_08A971A8(Part98058 *self);
void func_08A97240(Part98058 *self);
unsigned int func_08A97578(Part98058 *self);
unsigned int func_08A97598(Part98058 *self);
float func_08A975B8(Part98058 *self);
unsigned int func_08A976C4(Part98058 *self);
unsigned char func_08A9771C(Part98058 *self);
unsigned char *func_08A978EC(Part98058 *self);
int func_08A97900(Part98058 *self);
unsigned int func_08A97910(Part98058 *self, unsigned char flag, int value);
unsigned char func_08A97924(Part98058 *self);
int func_08A9792C(Part98058 *self);

/* Part9A1A0 */
Part9A1A0 *func_08A98078(Part9A1A0 *self);
void func_08A980C8(Part9A1A0 *self, int flags);
void func_08A98120(Part9A1A0 *self);
void func_08A981C0(Part9A1A0 *self);
void func_08A993AC(Part9A1A0 *self);

/* Part9A528, Part9ABE8 and PartA1BD8 (constructors, destructors and releases
 * declared in auto_region3.h) */
void func_08A9A248(Part9A528 *self);
void func_08A9A5DC(Part9ABE8 *self);
void func_08A9ACBC(PartA1BD8 *self);
int func_08A9B928(PartA1BD8 *self);

/* PartA29D8 (constructor, destructor and release declared in
 * auto_region3.h) */
void func_08AA1F74(PartA29D8 *self);
unsigned char func_08AA20FC(PartA29D8 *self);

/* PartA4250 (constructor, destructor and release declared in
 * auto_region3.h) */
void func_08AA2A94(PartA4250 *self);

/* PartA4CA0 */
PartA4CA0 *func_08AA4270(PartA4CA0 *self);
void func_08AA42A8(PartA4CA0 *self, int flags);
void func_08AA4300(PartA4CA0 *self);
void func_08AA4328(PartA4CA0 *self);
void func_08AA4BD4(PartA4CA0 *self, int index);
void func_08AA4C38(PartA4CA0 *self, int slot);
unsigned char func_08AA4C88(PartA4CA0 *self);
int func_08AA4C94(void);

/* PartA5610 (constructor, destructor and release declared in
 * auto_region3.h) */
void func_08AA4D40(PartA5610 *self);

/* PartA5818 (release declared in auto_region3.h) */
void func_08AA5668(PartA5818 *self, int flags);
void func_08AA56C0(PartA5818 *self);

/* PartA5830 */
PartA5830 *func_08AA5830(PartA5830 *self);
void func_08AA5858(PartA5830 *self, int flags);
void func_08AA58A4(PartA5830 *self);
void func_08AA5914(PartA5830 *self);
unsigned char func_08AA5C78(PartA5830 *self);
unsigned char func_08AA5C98(PartA5830 *self);
void func_08AA61D8(PartA5830 *self, int index);
void func_08AA6228(PartA5830 *self, int slot);

/* PartA76A0 */
PartA76A0 *func_08AA6618(PartA76A0 *self);
void func_08AA69DC(PartA76A0 *self, int flags);
void func_08AA6B6C(PartA76A0 *self);
void func_08AA6B84(PartA76A0 *self);

/* PartAAD68 */
PartAAD68 *func_08AA7788(PartAAD68 *self);
void func_08AA77D0(PartAAD68 *self, int flags);
void func_08AA7840(PartAAD68 *self);
void func_08AA7850(PartAAD68 *self);
PartA76A0 *func_08AA79C4(PartAAD68 *self);

/* PartADF68 (constructor declared in auto_region3.h) */
void func_08AABD2C(PartADF68 *self);
void *func_08AAADCC(void *object);
void *func_08AAAD88(void *object);
Inline2 *func_08AACAF0(PartADF68 *self);
int func_08AACB10(PartADF68 *self);
int func_08AAD1C4(PartADF68 *self);
int func_08AABDC8(PartADF68 *self, int arg);
int func_08AABE2C(PartADF68 *self, int arg);
void func_08AAE2C8(void *unused, int arg);
void func_08AAE61C(void *unused, int arg);

/* PartB2EB8 */
PartB2EB8 *func_08AAE188(PartB2EB8 *self);
void func_08AAE1C0(PartB2EB8 *self, int flags);
void func_08AAE218(PartB2EB8 *self);
void func_08AAE22C(PartB2EB8 *self);

/* PartB32A8 (constructor, destructor, release and some getters declared in
 * auto_region3.h) */
void func_08AB300C(PartB32A8 *self);
Part83A38 *func_08AB3274(PartB32A8 *self);
Part65650 *func_08AB3284(PartB32A8 *self);
Part7E728 *func_08AB328C(PartB32A8 *self);
Part870D8 *func_08AB329C(PartB32A8 *self);

/* PartB3A30 */
PartB3A30 *func_08AB3758(PartB3A30 *self);
void func_08AB3790(PartB3A30 *self, int flags);
void func_08AB37E8(PartB3A30 *self);
void func_08AB37FC(PartB3A30 *self);
int func_08AB3818(PartB3A30 *self);
int func_08AB387C(PartB3A30 *self);
int func_08AB38C4(PartB3A30 *self);
int func_08AB390C(PartB3A30 *self);
unsigned char func_08AB3958(PartB3A30 *self);
unsigned char func_08AB399C(PartB3A30 *self);
unsigned char func_08AB39EC(PartB3A30 *self);

/* PartB3C90 */
PartB3C90 *func_08AB3A40(PartB3C90 *self);
void func_08AB3A78(PartB3C90 *self, int flags);
void func_08AB3AD0(PartB3C90 *self);
void func_08AB3AE0(PartB3C90 *self);
unsigned char func_08AB3C4C(PartB3C90 *self);

/* PartB4240 */
PartB4240 *func_08AB3CB8(PartB4240 *self);
void func_08AB3CF0(PartB4240 *self, int flags);
void func_08AB3D48(PartB4240 *self);
void func_08AB3D80(PartB4240 *self);
int func_08AB3EA0(PartB4240 *self);
void func_08AB3E80(void);
unsigned char func_08AB4190(PartB4240 *self);
unsigned char func_08AB421C(void *self, BattleFighter *fighter);

/* 0x08AB54C0..0x08AB5558 */
void func_08AB58A4(void *object, int arg);
void func_08AB5534(int arg);

/* PartB71A0 */
PartB71A0 *func_08AB67DC(PartB71A0 *self, int arg);
void func_08AB6820(PartB71A0 *self, int flags);
void func_08AB6884(PartB71A0 *self);
void func_08AB68A8(PartB71A0 *self);
void func_08AB6E5C(void);

/* PartB7560 */
PartB7560 *func_08AB73E0(PartB7560 *self);
void func_08AB7418(PartB7560 *self, int flags);
void func_08AB7470(PartB7560 *self);
void func_08AB7498(PartB7560 *self);
int func_08AB74B4(PartB7560 *self);

/* PartB7ED0 */
PartB7ED0 *func_08AB7570(PartB7ED0 *self);
void func_08AB75A8(PartB7ED0 *self, int flags);
void func_08AB7600(PartB7ED0 *self);
void func_08AB7610(PartB7ED0 *self);

/* PartB8390 */
PartB8390 *func_08AB8390(PartB8390 *self);
void func_08AB83B8(PartB8390 *self, int flags);
void func_08AB8404(PartB8390 *self);
void func_08AB843C(PartB8390 *self);
unsigned char func_08AB8860(PartB8390 *self);

/* PartB97D0 */
PartB97D0 *func_08AB8868(PartB97D0 *self);
void func_08AB88A0(PartB97D0 *self, int flags);
void func_08AB88F8(PartB97D0 *self);
void func_08AB8908(PartB97D0 *self);

/* PartB9F28 */
PartB9F28 *func_08AB97E8(PartB9F28 *self);
void func_08AB9A18(PartB9F28 *self, int flags);
void func_08AB9B18(PartB9F28 *self);
void func_08AB9B24(PartB9F28 *self);

/* PartBA7C8 */
PartBA7C8 *func_08ABA008(PartBA7C8 *self);
void func_08ABA040(PartBA7C8 *self, int flags);
void func_08ABA098(PartBA7C8 *self);
void func_08ABA0B4(PartBA7C8 *self);

/* PartBAC98 */
PartBAC98 *func_08ABA7E8(PartBAC98 *self);
void func_08ABA820(PartBAC98 *self, int flags);
void func_08ABA878(PartBAC98 *self);
void func_08ABA8AC(PartBAC98 *self);

/* PartBBDE8 */
PartBBDE8 *func_08ABACA8(PartBBDE8 *self);
void func_08ABAD30(PartBBDE8 *self, int flags);
void func_08ABAE18(PartBBDE8 *self);
void func_08ABAE40(PartBBDE8 *self);
void func_08ABAE5C(PartBBDE8 *self);
void func_08ABAE78(PartBBDE8 *self);
void func_08ABAE94(PartBBDE8 *self);
int func_08ABB8A4(PartBBDE8 *self);
int func_08ABB8E0(PartBBDE8 *self);
int func_08ABB990(PartBBDE8 *self);
int func_08ABBB78(PartBBDE8 *self);
int func_08ABBBE0(PartBBDE8 *self);
void func_08ABAEF4(void);
void func_08ABAF14(void);
void func_08ABAF34(void);
void func_08ABAF54(void);
void func_08ABAF74(void);
void func_08ABB43C(PartBBDE8 *self);
void func_08ABB458(PartBBDE8 *self);
PartB7ED0 *func_08ABB4B4(PartBBDE8 *self);

/* PartBC580 */
PartBC580 *func_08ABBE00(PartBC580 *self);
void func_08ABBFE8(PartBC580 *self, int flags);
void func_08ABC0D0(PartBC580 *self);
void func_08ABC0DC(PartBC580 *self);

/* PartBC608 */
void func_08ABC608(PartBC608 *self, void *owner);
void func_08ABC6B0(PartBC608 *self);
void func_08ABC88C(PartBC608 *self);
unsigned char func_08ABC714(PartBC608 *self);

/* PartBDE18 */
PartBDE18 *func_08ABD5D0(PartBDE18 *self);
void func_08ABD608(PartBDE18 *self, int flags);
void func_08ABD660(PartBDE18 *self);
void func_08ABD670(PartBDE18 *self);
EntryBC300 *func_08ABC300(void *object, int arg);
int func_08ABDD5C(PartBDE18 *self);
float func_08ABDD94(PartBDE18 *self, int value);

/* PartC03B8 */
PartC03B8 *func_08ABDE3C(PartC03B8 *self);
void func_08ABDEBC(PartC03B8 *self, int flags);
void func_08ABDF14(PartC03B8 *self);
void func_08ABDF94(PartC03B8 *self);
ListHead *func_08ABDE28(ListHead *self);

/* PartC0FA0 */
PartC0FA0 *func_08AC0AD0(PartC0FA0 *self);
void func_08AC0B08(PartC0FA0 *self, int flags);
void func_08AC0B60(PartC0FA0 *self);
void func_08AC0B70(PartC0FA0 *self);
int func_08AC0DE8(PartC0FA0 *self);
int func_08AC0E4C(PartC0FA0 *self);

/* PartC3C00 */
PartC3C00 *func_08AC1E78(PartC3C00 *self);
void func_08AC1EB0(PartC3C00 *self, int flags);
void func_08AC1F08(PartC3C00 *self);
void func_08AC1F48(PartC3C00 *self);

/* Random numbers (a linear congruential generator). */
void func_08AC3C18(void);
unsigned int func_08AC3C54(void);
float func_08AC3C88(void);
float func_08AC3CD0(void);
float func_08AC3D20(float low, float high);
int func_08AC3D5C(int low, int high);
int func_08AC3E5C(float low, float high, int arg2, int arg3);
void func_08AC3E2C(int low, int high, int arg2, int arg3);
void func_08AE7F64(void);

/* PartCD0C8 */
PartCD0C8 *func_08ACCD78(PartCD0C8 *self);
void func_08ACCE58(PartCD0C8 *self);
unsigned char func_08ACD0BC(PartCD0C8 *self);

/* PartCEE28 */
PartCEE28 *func_08ACEE28(PartCEE28 *self);
void func_08ACEE90(PartCEE28 *self);
void func_08ACEEA8(PartCEE28 *self);
int func_08ACEEC4(PartCEE28 *self);
unsigned char func_08ACF05C(PartCEE28 *self);
int func_08ACEFD8(PartCEE28 *self);

/* PartD9580 and PartDB2A0 */
void *func_08AD29D8(void *object);
void func_08AD2A10(void *object, int flags);
void func_08AD2F38(void *object);
void func_08AD7790(void *object);
PartD9580 *func_08AD7838(PartD9580 *self);
void func_08AD7888(PartD9580 *self, int flags);
void func_08AD7E48(PartD9580 *self);
void func_08AD8284(PartD9580 *self, int arg);
void func_08AD8DA8(PartD9580 *self);
void func_08AD968C(PartDB2A0 *self, int flags);
void func_08ADA06C(PartDB2A0 *self);
void func_08ADAD54(PartDB2A0 *self);
void func_08AD2FA8(void *object);
void func_08AD30CC(void *object);
void func_08AD3234(void *object);

/* PartE7B50 */
void func_08AE0690(PartE7B50 *self, int flags);
void func_08AE08DC(PartE7B50 *self);
void func_08AFCB44(void *object);
void func_08AE9648(ListHead *list);
void func_08AE9978(ListHead *list);
void func_08ADFB7C(void *object);
int func_08ADFDDC(void *object);
void func_08ADFCC4(void *object);
void func_08AE1270(PartE7B50 *self, int phase);
int func_08AE37B0(PartE7B50 *self, int id);
void func_08AE36D0(PartE7B50 *self);
void func_08AE36EC(PartE7B50 *self);
void func_08AE36F4(PartE7B50 *self);
int func_08AE3C70(void);

/* ManagerE7CA0 */
void func_08AFE308(ManagerE7CA0 *manager);
void func_08AFE300(void);
void func_08AFDFB0(ManagerE7CA0 *manager);
void func_08AFDE5C(ManagerE7CA0 *manager, int arg1, int arg2);
void func_08AFE168(ManagerE7CA0 *manager, int arg);
void func_08ADFA28(void);
void func_08ADFA54(void);
void func_08AE7FA0(void);

/* D_09B6B830 */
void func_08AEAD68(void *object);
void func_08AEAD7C(void *object);
void func_08AEAF28(void *object);

/* PartFB558 (base with its vtable at field_10) */
void func_08AF3E2C(void *object, int flags);


/* PartB54C0 and neighbours */
void func_08AB54C0(PartB54C0 *self);
unsigned char func_08AB5558(PartB54C0 *self);
PartB7F18 *func_08AB7F18(PartB7F18 *self);

/* PartC6098 */
void func_08AC6098(PartC6098 *self);
void func_08AC60BC(PartC6098 *self);
void func_08AC61AC(PartC6098 *self, int index, int arg2, int arg3);
void func_08AC6218(PartC6098 *self);

/* PartC8F00 */
PartC8F00 *func_08AC8F00(PartC8F00 *self);
void func_08AC8F30(PartC8F00 *self, int flags);
void func_08AC8F84(PartC8F00 *self);
void func_08AC8FC8(PartC8F00 *self);
int func_08AC9468(PartC8F00 *self);
int func_08AC9230(PartC8F00 *self);
void func_08AC949C(void);

/* NodeD2ABC */
void func_08AD2ABC(NodeD2ABC *self, unsigned char flag);
void func_08AD2AE4(NodeD2ABC *self);

/* ObjD2988, ObjD29B0 */
ObjD2988 *func_08AD1FA8(ObjD2988 *self, int value);
ObjD29B0 *func_08AD2424(ObjD29B0 *self, int value);

/* Globals D_089D4928..D_089D4934 */
void func_08AD28C4(void);
void func_08AD7724(void);
void func_08AD2848(void);
void func_08AD76A0(void);

/* PartE7B50 (more) */
void func_08AE0AB0(PartE7B50 *self);
void func_08AE1040(PartE7B50 *self);
void func_08AE1300(PartE7B50 *self);
void func_08AE2014(PartE7B50 *self, int arg, int *value);
void func_08AE2148(PartE7B50 *self);
void func_08AE3700(PartE7B50 *self, int value, int index);
void func_08AE3AA4(int index);
void func_08AE3AD0(int index);
void func_08AE3AFC(int index);
void func_08AE9638(ListHead *list);
int func_08AE96D4(ListHead *list);

/* PartF2CBC */
void func_08AF2CBC(PartF2CBC *self, int arg1, int arg2, const char *name,
                   int arg4, int arg5, int arg6);
void func_08AF306C(PartF2CBC *self);
void func_08AF324C(PartF2CBC *self);
void func_08AF359C(PartF2CBC *self);
void func_08AF614C(void *object, int arg);
void func_08AF62F8(void *object);

/* PartFB558 and derived classes */
PartFB558 *func_08AF3E08(PartFB558 *self);
void func_08AF2D5C(PartFB5A0 *self, int flags);
void func_08AF2DB8(PartFB5E8 *self, int flags);
void func_08AF2E14(PartFB630 *self, int flags);
void func_08AF3AD8(ObjFB520 *self, int flags);

/* PartF3D40, PartF4458 */
void func_08AF3D40(PartF3D40 *self);
void func_08AF3D6C(PartF3D40 *self);
void func_08AF4458(PartF4458 *self);
void func_08AF4634(PartF4458 *self);
void func_08AF4670(PartF4458 *self);
void func_08AF4694(PartF4458 *self);
void func_08AF46D0(PartF4458 *self);
void func_08AF4AAC(PartF4458 *self);
void func_08AF4C7C(PartF4458 *self);
void func_08AF4CB8(PartF4458 *self);
void func_08AF4CDC(PartF4458 *self);

/* Empty functions and plain forwarders */
void func_08AECED4(void);
void func_08AECEDC(void);

#ifdef __cplusplus
}
#endif

#endif
