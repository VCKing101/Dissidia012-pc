#ifndef DISSIDIA_BATTLE_AUTO_REGION2_H
#define DISSIDIA_BATTLE_AUTO_REGION2_H

/* Classes behind the automatically matched functions in battle
 * 0x089FAC9C..0x08A152D0 (readability pass, region 2).
 *
 * Same conventions as auto_region1.h, which this header builds on: "Part"
 * structs keep their vtable pointer after their data, "Unit" structs derive
 * from the class built by func_08A157D0 (or func_089F12F0) and keep the vtable
 * at offset 0, and each struct is named after the address of its vtable. A
 * part's first byte is cleared by its reset and tested first by its getters,
 * so it is called `active`. Objects of classes outside this region are byte
 * arrays named after the function that builds them.
 *
 * Every unit here resets field_08 to -1, and its virtual "step" functions set
 * field_08 (to -1, 2, 3 or 8) once a member reports that it has finished.
 *
 * Some constructors, destructors and getters of these parts (func_08A01788,
 * func_08A08178, func_08A10AC8 ...) are already declared in auto_region1.h
 * with `void *` parameters; their definitions keep that signature. */

#include "battle/auto_region1.h"

#ifdef __cplusplus
extern "C" {
#endif

#define REGION2_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define REGION2_ASSERT(name, cond) typedef char region2_assert_##name[(cond) ? 1 : -1]
#define REGION2_SIZE(type, size) REGION2_ASSERT(size_##type, sizeof(type) == (size))
#define REGION2_AT(type, field, offset) \
    REGION2_ASSERT(at_##type##_##field, REGION2_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ parts */

/* 0x28 bytes. Reset func_08A01818, release func_08A01844. func_08A01C74
 * signals field_08 when field_04 and field_08 are set. */
typedef struct Part01DA8 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    void *field_08;
    unsigned char field_0C;
    char pad_0D[3];
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    unsigned char field_20;
    char pad_21[3];
    void *vtable;
} Part01DA8;
REGION2_SIZE(Part01DA8, 0x28);
REGION2_AT(Part01DA8, vtable, 0x24);

/* 0x24 bytes. Reset func_08A01E60, release func_08A01E88. */
typedef struct Part02198 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    void *vtable;
} Part02198;
REGION2_SIZE(Part02198, 0x24);

/* 0x2C bytes. Reset func_08A10B58, release func_08A10B88. */
typedef struct Part10F98 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    unsigned char field_24;
    char pad_25[3];
    void *vtable;
} Part10F98;
REGION2_SIZE(Part10F98, 0x2C);
REGION2_AT(Part10F98, field_24, 0x24);

/* 0x34 bytes. Reset func_08A0821C, release func_08A08224. A wrapper whose
 * methods forward to its Part10F98. */
typedef struct Part08388 {
    unsigned char active;
    char pad_01[3];
    Part10F98 field_04;
    void *vtable;
} Part08388;
REGION2_SIZE(Part08388, 0x34);

/* 0x14 bytes. Reset func_08A11038, release func_08A1104C. */
typedef struct Part11298 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    void *vtable;
} Part11298;
REGION2_SIZE(Part11298, 0x14);

/* 0x14 bytes. Reset func_08A11338, release func_08A11350. */
typedef struct Part11640 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} Part11640;
REGION2_SIZE(Part11640, 0x14);

/* 0x14 bytes. Reset func_08A0D750, release func_08A0D768. */
typedef struct Part0D948 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} Part0D948;
REGION2_SIZE(Part0D948, 0x14);

/* 0x40 bytes. Reset func_08A0845C, release func_08A08480 frees field_04. */
typedef struct Part08880 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    PartF81A8 field_08;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    unsigned char field_38;
    char pad_39[3];
    void *vtable;
} Part08880;
REGION2_SIZE(Part08880, 0x40);
REGION2_AT(Part08880, field_38, 0x38);

/* 0x144 bytes. Reset func_08A11704, release func_08A11724 frees field_04. */
typedef struct Part11A58 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    PartF81A8 field_08;
    char pad_28[0x108];
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    void *vtable;
} Part11A58;
REGION2_SIZE(Part11A58, 0x144);
REGION2_AT(Part11A58, field_130, 0x130);

/* 0x98 bytes. Reset func_08A0E43C, release func_08A0E4A8. */
typedef struct Part0EB78 {
    char pad_00[8];
    PartF81A8 field_08;
    char pad_28[0x6C];
    void *vtable;
} Part0EB78;
REGION2_SIZE(Part0EB78, 0x98);

/* 0x68 bytes. Reset func_08A12C5C, release func_08A12CBC. */
typedef struct Part13268 {
    char pad_00[8];
    PartF81A8 field_08;
    char pad_28[0x3C];
    void *vtable;
} Part13268;
REGION2_SIZE(Part13268, 0x68);

/* 0x10C bytes. Reset func_089FC068, release func_089FC0D8. */
typedef struct PartFC658 {
    char pad_00[0x108];
    void *vtable;
} PartFC658;
REGION2_SIZE(PartFC658, 0x10C);

/* 0x1C bytes. Reset func_089FC6F8, release func_089FC71C. */
typedef struct PartFCB00 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    float field_0C;
    int field_10;
    int field_14;
    void *vtable;
} PartFCB00;
REGION2_SIZE(PartFCB00, 0x1C);

/* 0x34 bytes. Reset func_089FDEB0, release func_089FDEF8. */
typedef struct PartFE400 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08[3];
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    void *vtable;
} PartFE400;
REGION2_SIZE(PartFE400, 0x34);

/* 0x3C bytes. Reset func_08A00738, release func_08A00784. */
typedef struct Part00CE0 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08[4];
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    void *vtable;
} Part00CE0;
REGION2_SIZE(Part00CE0, 0x3C);

/* 0x30 bytes. Reset func_08A08948, release func_08A08990. */
typedef struct Part08E98 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08[2];
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    void *vtable;
} Part08E98;
REGION2_SIZE(Part08E98, 0x30);

/* 0xB0 bytes (arrays of it use that stride). Release func_08A03944. */
typedef struct Part048C8 {
    char pad_00[0xAC];
    void *vtable;
} Part048C8;
REGION2_SIZE(Part048C8, 0xB0);

/* 0xA8 bytes. Release func_08A049EC. */
typedef struct Part05658 {
    char pad_00[0xA4];
    void *vtable;
} Part05658;
REGION2_SIZE(Part05658, 0xA8);

/* 0x34 bytes (arrays of it use that stride). Release func_08A06264. */
typedef struct Part06B08 {
    char pad_00[0x30];
    void *vtable;
} Part06B08;
REGION2_SIZE(Part06B08, 0x34);

/* 0x388 bytes. Reset func_08A06D0C, release func_08A06D38. */
typedef struct Part08158 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    unsigned char field_10;
    unsigned char field_11;
    char pad_12[2];
    int field_14;
    int field_18;
    int field_1C;
    unsigned char field_20[8];          /* built by func_08A15968 */
    unsigned char field_28[0xC];        /* built by func_08A15540 */
    Part11640 field_34;
    PartF12D8 field_48;
    PartF5E58 field_5C;
    Part06B08 field_164[2];
    Part10F98 field_1CC[2];
    Part048C8 field_224[2];
    void *vtable;
} Part08158;
REGION2_SIZE(Part08158, 0x388);
REGION2_AT(Part08158, field_34, 0x34);
REGION2_AT(Part08158, field_5C, 0x5C);
REGION2_AT(Part08158, field_164, 0x164);
REGION2_AT(Part08158, field_1CC, 0x1CC);
REGION2_AT(Part08158, field_224, 0x224);

/* 0x12C bytes. Reset func_08A0ECB0, release func_08A0ED3C. */
typedef struct Part0F8D8 {
    char pad_00[0x34];
    PartF8670 field_34[2];
    Part08388 field_A4[2];
    char pad_10C[0x1C];
    void *vtable;
} Part0F8D8;
REGION2_SIZE(Part0F8D8, 0x12C);
REGION2_AT(Part0F8D8, field_A4, 0xA4);

/* ------------------------------------------------------------ units */

/* Built by func_089FB630. Reset func_089FB70C, release func_089FB73C. */
typedef struct UnitFBE98 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    unsigned char field_18[0x78];       /* built by func_08A21478 */
    PartF8670 field_90;
    Part08388 field_C8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
} UnitFBE98;
REGION2_SIZE(UnitFBE98, 0x10C);
REGION2_AT(UnitFBE98, field_C8, 0xC8);

/* Built by func_089FCB30. Reset func_089FCC20, release func_089FCC4C. */
typedef struct UnitFD348 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part01DA8 field_14;
    unsigned char field_3C[0x30];       /* built by func_08A16758 */
    PartFE400 field_6C;
    PartFCB00 field_A0;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
} UnitFD348;
REGION2_SIZE(UnitFD348, 0xCC);
REGION2_AT(UnitFD348, field_A0, 0xA0);

/* Built by func_089FD430. Reset func_089FD520, release func_089FD54C. */
typedef struct UnitFDCA8 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part01DA8 field_14;
    unsigned char field_3C[0x30];       /* built by func_08A16758 */
    PartFE400 field_6C;
    PartFC658 field_A0;
    int field_1AC;
    int field_1B0;
    int field_1B4;
    int field_1B8;
} UnitFDCA8;
REGION2_SIZE(UnitFDCA8, 0x1BC);
REGION2_AT(UnitFDCA8, field_A0, 0xA0);

/* Built by func_089FE410; same members as UnitFDCA8. Reset func_089FE500,
 * release func_089FE52C. */
typedef struct UnitFEC98 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part01DA8 field_14;
    unsigned char field_3C[0x30];       /* built by func_08A16758 */
    PartFE400 field_6C;
    PartFC658 field_A0;
    int field_1AC;
    int field_1B0;
    int field_1B4;
    int field_1B8;
} UnitFEC98;
REGION2_SIZE(UnitFEC98, 0x1BC);

/* Built by func_089FEE10. Reset func_089FEF50, release func_089FEF88. */
typedef struct UnitFFAE0 {
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
    Part11298 field_F4;
    unsigned char field_108[0x30];      /* built by func_08A16758 */
    PartFE400 field_138;
    PartFC658 field_16C;
    int field_278;
    int field_27C;
    int field_280;
    int field_284;
} UnitFFAE0;
REGION2_SIZE(UnitFFAE0, 0x288);
REGION2_AT(UnitFFAE0, field_CC, 0xCC);
REGION2_AT(UnitFFAE0, field_F4, 0xF4);
REGION2_AT(UnitFFAE0, field_16C, 0x16C);

/* Built by func_089FFC88. Reset func_089FFD78, release func_089FFDB0. */
typedef struct Unit005E8 {
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
    Part01DA8 field_20;
    unsigned char field_48[0x30];       /* built by func_08A16758 */
    PartF8670 field_78;
    PartF63E0 field_B0;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
} Unit005E8;
REGION2_SIZE(Unit005E8, 0x108);
REGION2_AT(Unit005E8, field_B0, 0xB0);

/* Built by func_08A00D18; same members as UnitFA2C8. Reset func_08A00E1C,
 * release func_08A00E48. */
typedef struct Unit01680 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part01DA8 field_14;
    unsigned char field_3C[0x30];       /* built by func_08A16758 */
    Part02198 field_6C;
    Part00CE0 field_90;
    Part08388 field_CC;
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
} Unit01680;
REGION2_SIZE(Unit01680, 0x110);
REGION2_AT(Unit01680, field_CC, 0xCC);

/* Built by func_08A021B8 on the UnitF13D8 base. Reset func_08A02258,
 * release func_08A02288. */
typedef struct Unit036D8 {
    UnitF13D8 base;
    unsigned char field_04;
    char pad_05[3];
    int field_08[2];
    int field_10;
    int field_14;
} Unit036D8;
REGION2_AT(Unit036D8, field_14, 0x14);

/* Built by func_08A08EA8. Reset func_08A08F98, release func_08A08FC4. */
typedef struct Unit097B0 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part01DA8 field_14;
    unsigned char field_3C[0x30];       /* built by func_08A16758 */
    Part08E98 field_6C;
    Part08880 field_9C;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
} Unit097B0;
REGION2_SIZE(Unit097B0, 0xEC);
REGION2_AT(Unit097B0, field_9C, 0x9C);

/* Built by func_08A09898; same members as UnitF97A8. Reset func_08A09998,
 * release func_08A099C8. */
typedef struct Unit0A208 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    PartF8E08 field_18;
    PartF8670 field_A8;
    Part08388 field_E0[2];
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
} Unit0A208;
REGION2_SIZE(Unit0A208, 0x158);
REGION2_AT(Unit0A208, field_E0, 0xE0);

/* Built by func_08A0A3A8. Reset func_08A0A484, release func_08A0A4B8. */
typedef struct Unit0AE30 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part10F98 field_14;
    PartF65E0 field_40;
    Part08388 field_978;
    int field_9AC;
    int field_9B0;
    int field_9B4;
    int field_9B8;
    int field_9BC;
    int field_9C0;
} Unit0AE30;
REGION2_SIZE(Unit0AE30, 0x9C4);
REGION2_AT(Unit0AE30, field_978, 0x978);

/* Built by func_08A0AF00. Reset func_08A0AFF0, release func_08A0B020. */
typedef struct Unit0BAF0 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part10F98 field_14;
    Part01DA8 field_40;
    unsigned char field_68[0x30];       /* built by func_08A16758 */
    Part08388 field_98;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
} Unit0BAF0;
REGION2_SIZE(Unit0BAF0, 0xE0);
REGION2_AT(Unit0BAF0, field_98, 0x98);

/* Built by func_08A0BBD8. Reset func_08A0BCA0, release func_08A0BCD0. */
typedef struct Unit0C540 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part10F98 field_14;
    Part08388 field_40;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
} Unit0C540;
REGION2_SIZE(Unit0C540, 0x88);

/* Built by func_08A0C628. Reset func_08A0C704, release func_08A0C730. */
typedef struct Unit0CE20 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part01DA8 field_14;
    unsigned char field_3C[0x30];       /* built by func_08A16758 */
    Part10F98 field_6C;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
} Unit0CE20;
REGION2_SIZE(Unit0CE20, 0xA8);

/* Built by func_08A0CEE0. Reset func_08A0CFA8, release func_08A0CFD4. */
typedef struct Unit0D608 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part01DA8 field_14;
    unsigned char field_3C[0x30];       /* built by func_08A16758 */
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
} Unit0D608;
REGION2_SIZE(Unit0D608, 0x7C);

/* Built by func_08A0D960. Reset func_08A0DA50, release func_08A0DA84. */
typedef struct Unit0E2E0 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    int field_0C;
    unsigned char field_10;
    unsigned char field_11;
    char pad_12[2];
    float field_14;
    Part01DA8 field_18;
    unsigned char field_40[0x30];       /* built by func_08A16758 */
    Part02198 field_70;
    Part0D948 field_94;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
} Unit0E2E0;
REGION2_SIZE(Unit0E2E0, 0xB8);
REGION2_AT(Unit0E2E0, field_94, 0x94);

/* Built by func_08A0F978. Reset func_08A0FA2C, release func_08A0FA50. */
typedef struct Unit0FF40 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    Part0F8D8 field_10;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
} Unit0FF40;
REGION2_SIZE(Unit0FF40, 0x14C);
REGION2_AT(Unit0FF40, field_13C, 0x13C);

/* Built by func_08A0FFF0. Reset func_08A100CC, release func_08A100FC. */
typedef struct Unit10A10 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    Part01DA8 field_14;
    unsigned char field_3C[0x30];       /* built by func_08A16758 */
    Part08388 field_6C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
} Unit10A10;
REGION2_SIZE(Unit10A10, 0xB4);

/* Built by func_08A11A68. Reset func_08A11BA8, release func_08A11BE0.
 * field_2B4 and field_2B8 are passed to func_08A22E2C and friends (both are
 * checked to be 0 or 1 there). */
typedef struct Unit12A88 {
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
    unsigned char field_44[0x30];       /* built by func_08A16758 */
    Part02198 field_74;
    Part10F98 field_98;
    Part10F98 field_C4;
    PartF8670 field_F0;
    char pad_128[8];
    Part11A58 field_130;
    char pad_274[0xC];
    Part08388 field_280;
    int field_2B4;
    int field_2B8;
    int field_2BC;
    int field_2C0;
} Unit12A88;
REGION2_SIZE(Unit12A88, 0x2C4);
REGION2_AT(Unit12A88, field_F0, 0xF0);
REGION2_AT(Unit12A88, field_130, 0x130);
REGION2_AT(Unit12A88, field_280, 0x280);

/* Built by func_08A13278. Reset func_08A133C8, release func_08A13418. */
typedef struct Unit14458 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    char pad_14[0x10];
    Part01DA8 field_24;
    unsigned char field_4C[0x30];       /* built by func_08A16758 */
    Part02198 field_7C;
    PartF8670 field_A0[2];
    Part13268 field_110;
    PartF63E0 field_178;
    Part08388 field_1C0;
    int field_1F4;
    int field_1F8;
} Unit14458;
REGION2_AT(Unit14458, field_A0, 0xA0);
REGION2_AT(Unit14458, field_178, 0x178);
REGION2_AT(Unit14458, field_1F4, 0x1F4);

/* Built by func_08A145E0. Reset func_08A1470C, release func_08A14744. */
typedef struct Unit15448 {
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
    unsigned char field_44[0x30];       /* built by func_08A16758 */
    Part02198 field_74;
    Part10F98 field_98;
    PartF8670 field_C4;
    PartF63E0 field_FC;
    Part08388 field_144;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
} Unit15448;
REGION2_SIZE(Unit15448, 0x188);
REGION2_AT(Unit15448, field_144, 0x144);

/* ------------------------------------------------------------ vtables */

extern unsigned char D_089FBE98[];
extern unsigned char D_089FC658[];
extern unsigned char D_089FCB00[];
extern unsigned char D_089FD348[];
extern unsigned char D_089FDCA8[];
extern unsigned char D_089FE400[];
extern unsigned char D_089FEC98[];
extern unsigned char D_089FFAE0[];
extern unsigned char D_08A005E8[];
extern unsigned char D_08A00CE0[];
extern unsigned char D_08A01680[];
extern unsigned char D_08A01DA8[];
extern unsigned char D_08A02198[];
extern unsigned char D_08A036D8[];
extern unsigned char D_08A048C8[];
extern unsigned char D_08A05658[];
extern unsigned char D_08A06B08[];
extern unsigned char D_08A08158[];
extern unsigned char D_08A08388[];
extern unsigned char D_08A08880[];
extern unsigned char D_08A08E98[];
extern unsigned char D_08A097B0[];
extern unsigned char D_08A0A208[];
extern unsigned char D_08A0AE30[];
extern unsigned char D_08A0BAF0[];
extern unsigned char D_08A0C540[];
extern unsigned char D_08A0CE20[];
extern unsigned char D_08A0D608[];
extern unsigned char D_08A0D948[];
extern unsigned char D_08A0E2E0[];
extern unsigned char D_08A0EB78[];
extern unsigned char D_08A0F8D8[];
extern unsigned char D_08A0FF40[];
extern unsigned char D_08A10A10[];
extern unsigned char D_08A10F98[];
extern unsigned char D_08A11298[];
extern unsigned char D_08A11640[];
extern unsigned char D_08A11A58[];
extern unsigned char D_08A12A88[];
extern unsigned char D_08A13268[];
extern unsigned char D_08A14458[];
extern unsigned char D_08A15448[];

/* ------------------------------------------------------------ functions */

/* Region 1 classes: more methods. */
void func_089FAC9C(UnitFB568 *self, int flags);
void func_089FAD64(UnitFB568 *self);
int func_089FB4B0(UnitFB568 *self);
int func_089FB548(UnitFB568 *self);
unsigned char func_089F1298(PartF12D8 *self);
void func_089F58FC(PartF5E58 *self);
void func_089F5B5C(PartF5E58 *self);
void func_089F5B84(PartF5E58 *self);
void func_089F62E8(PartF63E0 *self);
void func_089F854C(PartF8670 *self);

/* UnitFBE98 */
UnitFBE98 *func_089FB630(UnitFBE98 *self);
void func_089FB684(UnitFBE98 *self, int flags);
void func_089FB70C(UnitFBE98 *self);
void func_089FB73C(UnitFBE98 *self);
int func_089FBDFC(UnitFBE98 *self);
int func_089FBE74(UnitFBE98 *self);

/* PartFC658 */
PartFC658 *func_089FBFD8(PartFC658 *self);
void func_089FC010(PartFC658 *self, int flags);
void func_089FC068(PartFC658 *self);
void func_089FC0D8(PartFC658 *self);

/* PartFCB00 */
PartFCB00 *func_089FC668(PartFCB00 *self);
void func_089FC6A0(PartFCB00 *self, int flags);
void func_089FC6F8(PartFCB00 *self);
void func_089FC71C(PartFCB00 *self);

/* UnitFD348 */
UnitFD348 *func_089FCB30(UnitFD348 *self);
void func_089FCB8C(UnitFD348 *self, int flags);
void func_089FCC20(UnitFD348 *self);
void func_089FCC4C(UnitFD348 *self);
int func_089FD308(UnitFD348 *self);

/* UnitFDCA8 */
UnitFDCA8 *func_089FD430(UnitFDCA8 *self);
void func_089FD48C(UnitFDCA8 *self, int flags);
void func_089FD520(UnitFDCA8 *self);
void func_089FD54C(UnitFDCA8 *self);
int func_089FDC64(UnitFDCA8 *self);

/* PartFE400 */
PartFE400 *func_089FDE20(PartFE400 *self);
void func_089FDE58(PartFE400 *self, int flags);
void func_089FDEB0(PartFE400 *self);
void func_089FDEF8(PartFE400 *self);

/* UnitFEC98 */
UnitFEC98 *func_089FE410(UnitFEC98 *self);
void func_089FE46C(UnitFEC98 *self, int flags);
void func_089FE500(UnitFEC98 *self);
void func_089FE52C(UnitFEC98 *self);
int func_089FEC54(UnitFEC98 *self);

/* UnitFFAE0 */
UnitFFAE0 *func_089FEE10(UnitFFAE0 *self);
void func_089FEE8C(UnitFFAE0 *self, int flags);
void func_089FEF50(UnitFFAE0 *self);
void func_089FEF88(UnitFFAE0 *self);
int func_089FFA84(UnitFFAE0 *self);

/* Unit005E8 */
Unit005E8 *func_089FFC88(Unit005E8 *self);
void func_089FFCE4(Unit005E8 *self, int flags);
void func_089FFD78(Unit005E8 *self);
void func_089FFDB0(Unit005E8 *self);
int func_08A005A8(Unit005E8 *self);

/* Part00CE0 (constructor func_08A006A8, destructor func_08A006E0) */
void func_08A00738(Part00CE0 *self);
void func_08A00784(Part00CE0 *self);

/* Unit01680 */
Unit01680 *func_08A00D18(Unit01680 *self);
void func_08A00D7C(Unit01680 *self, int flags);
void func_08A00E1C(Unit01680 *self);
void func_08A00E48(Unit01680 *self);
int func_08A015C4(Unit01680 *self);
int func_08A0165C(Unit01680 *self);

/* Part01DA8 (constructor func_08A01788, destructor func_08A017C0,
 * getter func_08A01A9C) */
void func_08A01818(Part01DA8 *self);
void func_08A01844(Part01DA8 *self);
void *func_08A01AC0(Part01DA8 *self);
int func_08A01C74(Part01DA8 *self, int mode);

/* Part02198 (constructor func_08A01DD0, destructor func_08A01E08) */
void func_08A01E60(Part02198 *self);
void func_08A01E88(Part02198 *self);
unsigned char func_08A020A4(Part02198 *self);

/* Unit036D8 */
Unit036D8 *func_08A021B8(Unit036D8 *self);
void func_08A021F4(Unit036D8 *self, int flags);
void func_08A02258(Unit036D8 *self);
void func_08A02288(Unit036D8 *self);

/* Part048C8, Part05658, Part06B08 */
Part048C8 *func_08A03880(Part048C8 *self);
void func_08A038B8(Part048C8 *self, int flags);
void func_08A03944(Part048C8 *self);
void func_08A04960(Part05658 *self, int flags);
void func_08A049EC(Part05658 *self);
Part06B08 *func_08A06198(Part06B08 *self);
void func_08A061D0(Part06B08 *self, int flags);
void func_08A06264(Part06B08 *self);

/* Part08158 */
Part08158 *func_08A06B70(Part08158 *self);
void func_08A06C18(Part08158 *self, int flags);
void func_08A06D0C(Part08158 *self);
void func_08A06D38(Part08158 *self);
void *func_08A074FC(Part08158 *self);
unsigned char func_08A0775C(Part08158 *self);
int func_08A07C6C(Part08158 *self);
void *func_08A15968(void *object);
void func_08A159A0(void *object, int flags);
void *func_08A15540(void *object);
void func_08A15578(void *object, int flags);

/* Part08388 (constructor func_08A08178, destructor func_08A081B8) */
void func_08A0821C(Part08388 *self);
void func_08A08224(Part08388 *self);
void func_08A08334(Part08388 *self);
void func_08A08350(Part08388 *self);
/* Returns what func_08A10F74 reports for the member. */
unsigned char func_08A0836C(Part08388 *self);

/* Part08880 */
Part08880 *func_08A083B8(Part08880 *self);
void func_08A083F8(Part08880 *self, int flags);
void func_08A0845C(Part08880 *self);
void func_08A08480(Part08880 *self);

/* Part08E98 */
Part08E98 *func_08A088B8(Part08E98 *self);
void func_08A088F0(Part08E98 *self, int flags);
void func_08A08948(Part08E98 *self);
void func_08A08990(Part08E98 *self);

/* Unit097B0 */
Unit097B0 *func_08A08EA8(Unit097B0 *self);
void func_08A08F04(Unit097B0 *self, int flags);
void func_08A08F98(Unit097B0 *self);
void func_08A08FC4(Unit097B0 *self);
int func_08A0976C(Unit097B0 *self);

/* Unit0A208 */
Unit0A208 *func_08A09898(Unit0A208 *self);
void func_08A098FC(Unit0A208 *self, int flags);
void func_08A09998(Unit0A208 *self);
void func_08A099C8(Unit0A208 *self);
int func_08A0A148(Unit0A208 *self);
int func_08A0A1E8(Unit0A208 *self);

/* Unit0AE30 */
Unit0AE30 *func_08A0A3A8(Unit0AE30 *self);
void func_08A0A3FC(Unit0AE30 *self, int flags);
void func_08A0A484(Unit0AE30 *self);
void func_08A0A4B8(Unit0AE30 *self);
int func_08A0AD30(Unit0AE30 *self);
int func_08A0AE10(Unit0AE30 *self);

/* Unit0BAF0 */
Unit0BAF0 *func_08A0AF00(Unit0BAF0 *self);
void func_08A0AF5C(Unit0BAF0 *self, int flags);
void func_08A0AFF0(Unit0BAF0 *self);
void func_08A0B020(Unit0BAF0 *self);
int func_08A0B9D0(Unit0BAF0 *self);
int func_08A0BAD0(Unit0BAF0 *self);

/* Unit0C540 */
Unit0C540 *func_08A0BBD8(Unit0C540 *self);
void func_08A0BC24(Unit0C540 *self, int flags);
void func_08A0BCA0(Unit0C540 *self);
void func_08A0BCD0(Unit0C540 *self);
int func_08A0C42C(Unit0C540 *self);
int func_08A0C44C(Unit0C540 *self);
int func_08A0C51C(Unit0C540 *self);

/* Unit0CE20 */
Unit0CE20 *func_08A0C628(Unit0CE20 *self);
void func_08A0C67C(Unit0CE20 *self, int flags);
void func_08A0C704(Unit0CE20 *self);
void func_08A0C730(Unit0CE20 *self);
int func_08A0CDDC(Unit0CE20 *self);

/* Unit0D608 */
Unit0D608 *func_08A0CEE0(Unit0D608 *self);
void func_08A0CF2C(Unit0D608 *self, int flags);
void func_08A0CFA8(Unit0D608 *self);
void func_08A0CFD4(Unit0D608 *self);
int func_08A0D5C4(Unit0D608 *self);

/* Part0D948 */
Part0D948 *func_08A0D6C0(Part0D948 *self);
void func_08A0D6F8(Part0D948 *self, int flags);
void func_08A0D750(Part0D948 *self);
void func_08A0D768(Part0D948 *self);
unsigned char func_08A0D888(Part0D948 *self);

/* Unit0E2E0 */
Unit0E2E0 *func_08A0D960(Unit0E2E0 *self);
void func_08A0D9BC(Unit0E2E0 *self, int flags);
void func_08A0DA50(Unit0E2E0 *self);
void func_08A0DA84(Unit0E2E0 *self);
int func_08A0E16C(Unit0E2E0 *self);

/* Part0EB78 (constructor func_08A0E398, destructor func_08A0E3D8) */
void func_08A0E43C(Part0EB78 *self);
void func_08A0E4A8(Part0EB78 *self);

/* Part0F8D8 */
Part0F8D8 *func_08A0EBB0(Part0F8D8 *self);
void func_08A0EC18(Part0F8D8 *self, int flags);
void func_08A0ECB0(Part0F8D8 *self);
void func_08A0ED3C(Part0F8D8 *self);

/* Unit0FF40 */
Unit0FF40 *func_08A0F978(Unit0FF40 *self);
void func_08A0F9BC(Unit0FF40 *self, int flags);
void func_08A0FA2C(Unit0FF40 *self);
void func_08A0FA50(Unit0FF40 *self);
int func_08A0FF20(Unit0FF40 *self);

/* Unit10A10 */
Unit10A10 *func_08A0FFF0(Unit10A10 *self);
void func_08A10044(Unit10A10 *self, int flags);
void func_08A100CC(Unit10A10 *self);
void func_08A100FC(Unit10A10 *self);
int func_08A10954(Unit10A10 *self);
int func_08A109EC(Unit10A10 *self);

/* Part10F98 (constructor func_08A10AC8, destructor func_08A10B00) */
void func_08A10B58(Part10F98 *self);
void func_08A10B88(Part10F98 *self);
int func_08A10C88(Part10F98 *self);
int func_08A10DB0(Part10F98 *self);
unsigned char func_08A10F74(Part10F98 *self);

/* Part11298 */
Part11298 *func_08A10FA8(Part11298 *self);
void func_08A10FE0(Part11298 *self, int flags);
void func_08A11038(Part11298 *self);
void func_08A1104C(Part11298 *self);
unsigned char func_08A11270(Part11298 *self);

/* Part11640 */
Part11640 *func_08A112A8(Part11640 *self);
void func_08A112E0(Part11640 *self, int flags);
void func_08A11338(Part11640 *self);
void func_08A11350(Part11640 *self);
int func_08A113A8(Part11640 *self);

/* Part11A58 */
Part11A58 *func_08A11660(Part11A58 *self);
void func_08A116A0(Part11A58 *self, int flags);
void func_08A11704(Part11A58 *self);
void func_08A11724(Part11A58 *self);
unsigned char func_08A11A34(Part11A58 *self);

/* Unit12A88 */
Unit12A88 *func_08A11A68(Unit12A88 *self);
void func_08A11AE4(Unit12A88 *self, int flags);
void func_08A11BA8(Unit12A88 *self);
void func_08A11BE0(Unit12A88 *self);
int func_08A1260C(Unit12A88 *self);
int func_08A12668(Unit12A88 *self);
int func_08A12870(Unit12A88 *self);
int func_08A128D8(Unit12A88 *self);
int func_08A12A44(Unit12A88 *self);

/* Part13268 */
Part13268 *func_08A12BB8(Part13268 *self);
void func_08A12BF8(Part13268 *self, int flags);
void func_08A12C5C(Part13268 *self);
void func_08A12CBC(Part13268 *self);

/* Unit14458 */
Unit14458 *func_08A13278(Unit14458 *self);
void func_08A132FC(Unit14458 *self, int flags);
void func_08A133C8(Unit14458 *self);
void func_08A13418(Unit14458 *self);
int func_08A13F64(Unit14458 *self);
int func_08A13FC0(Unit14458 *self);
int func_08A142A4(Unit14458 *self);
int func_08A14410(Unit14458 *self);

/* Unit15448 */
Unit15448 *func_08A145E0(Unit15448 *self);
void func_08A14654(Unit15448 *self, int flags);
void func_08A1470C(Unit15448 *self);
void func_08A14744(Unit15448 *self);
int func_08A150E8(Unit15448 *self);
int func_08A15144(Unit15448 *self);
int func_08A152D0(Unit15448 *self);

/* Classes outside this region. */
void *func_08A21478(void *object);
void func_08A214B0(void *object, int flags);
int func_08A16C58(void *object);

/* Battle queries on (field_2B4, field_2B8)-style pairs, both 0 or 1. */
int func_08A22E2C(void *out, int arg1, int arg2, int arg3);
char func_08A22FE0(int arg1, int arg2, int arg3);
char func_08A23054(int arg1, int arg2, int arg3);

#ifdef __cplusplus
}
#endif

#endif
