#ifndef DISSIDIA_BATTLE_AGENT1_TYPES_H
#define DISSIDIA_BATTLE_AGENT1_TYPES_H

/* Partial layouts recovered while matching battle overlay functions.
 * Names are descriptive guesses; unknown fields are named by offset. */

typedef struct BattleModelRef {
    unsigned char padding_00[0xC];
    short field_0C;
} BattleModelRef;

typedef struct BattleModelHandle {
    unsigned char padding_00[0x20];
    unsigned short field_20;
} BattleModelHandle;

typedef struct BattleRecord54 {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    unsigned char field_1C[7];
    unsigned char field_23[3];
    unsigned char padding_26[2];
    BattleModelHandle *field_28;
    void *field_2C;
    BattleModelRef *field_30;
    void *field_34[4];
    int field_44;
    struct BattleRecordOwner *field_48;
    int field_4C;
    void *vtable;
} BattleRecord54;

typedef struct BattleRecordOwner {
    unsigned char padding_000[0x174];
    void *field_174;
} BattleRecordOwner;


/* Subobject at +0x0C of BattleRecordPool. */
typedef struct BattleModelSet {
    unsigned char field_00;
    unsigned char padding_01[3];
    BattleModelRef *field_04;
    void *field_08[3];
    void *vtable;
} BattleModelSet;

typedef struct BattlePoolNode {
    BattleRecord54 *data;
    struct BattlePoolNode *next;
} BattlePoolNode;

typedef struct BattleListLink {
    struct BattleListLink *next;
} BattleListLink;

/* Object built by func_089DDFC8: three fixed records, a 14-entry ring of
 * free records and an intrusive active list. */
typedef struct BattleRecordPool {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    int field_08;
    unsigned char field_0C[0x18];
    BattleRecord54 field_24[3];
    BattlePoolNode nodes[14];
    BattleRecord54 records[14];
    BattlePoolNode *field_628;
    BattlePoolNode *field_62C;
    BattleListLink *field_630;
    int field_634;
    int field_638;
    void *vtable;
} BattleRecordPool;

/* 0x1C-byte element at +0x0C of BattleMotionSet (ctor 0x089E2A78). */
typedef struct BattleMotionLink {
    unsigned char field_00;
    unsigned char padding_01[3];
    float field_04;
    float field_08;
    float field_0C;
    int field_10;
    int field_14;
    void *vtable;
} BattleMotionLink;

typedef struct BattleMotionEntry {
    int id;
    float value;
} BattleMotionEntry;

/* 0x68-byte element at +0x44 of BattleMotionSet (ctor 0x089E2220). */
typedef struct BattleMotionTrack {
    unsigned char field_00;
    unsigned char padding_01[3];
    BattleMotionEntry entries[10];
    unsigned char padding_54[8];
    int field_5C;
    int field_60;
    void *vtable;
} BattleMotionTrack;

/* Object built by func_089E1D00. */
typedef struct BattleMotionSet {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    void *field_08;
    BattleMotionLink field_0C[2];
    BattleMotionTrack field_44[2];
    unsigned char field_114[3];
    unsigned char field_117;
    void *vtable;
} BattleMotionSet;

typedef struct BattleFlagByte {
    unsigned char padding_00[0x14];
    unsigned char field_14;
} BattleFlagByte;

typedef struct BattlePairActor {
    unsigned char padding_000[0x16C];
    BattleFlagByte *field_16C;
    unsigned char padding_170[0x28];
    BattleFlagByte *field_198;
} BattlePairActor;

/* 0xD8-byte element at +0x08 of BattlePairOwner (dtor 0x089E3920). */
typedef struct BattlePairSlot {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    unsigned char field_08;
    unsigned char field_09;
    unsigned char padding_0A[2];
} BattlePairSlot;

/* 0x34-byte object (ctor 0x089E3F08). */
typedef struct BattleBindingSet {
    unsigned char field_00;
    unsigned char padding_01[3];
    void *field_04;
    int field_08;
    void *field_0C;
    void *field_10[4][2];
    void *vtable;
} BattleBindingSet;

/* 0x34-byte object (ctor 0x089E44C0). */
typedef struct BattleTriggerSet {
    unsigned char field_00;
    unsigned char padding_01[3];
    void *field_04;
    void *field_08;
    void *field_0C[4];
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    void *vtable;
} BattleTriggerSet;

typedef struct BattlePairEntry {
    unsigned char field_00;
    unsigned char padding_01[7];
    BattleTriggerSet field_08;
    BattleTriggerSet field_3C;
    BattleBindingSet field_70;
    unsigned char padding_A4[0x18];
    BattlePairSlot field_BC[2];
    void *vtable;
} BattlePairEntry;

/* Subobject at +0x18 of the battle setup root (dtor 0x089E3258). */
typedef struct BattlePairOwner {
    unsigned char field_00;
    unsigned char padding_01[3];
    BattlePairActor *field_04;
    BattlePairEntry field_08[2];
    unsigned char field_1B8[3];
    unsigned char field_1BB;
    unsigned char field_1BC;
    unsigned char padding_1BD[3];
    void *vtable;
} BattlePairOwner;

/* 0x2C-byte object (ctor 0x089E4B90). */
typedef struct BattleEffectSlot {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    void *field_08;
    void *field_0C;
    void *field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    void *vtable;
} BattleEffectSlot;

/* 0xDC-byte object (dtor 0x089E5270). */
typedef struct BattleEffectOwner {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    int field_08;
    int field_0C;
    unsigned char field_10[0x30];
    unsigned char field_40[0x18];
    BattleEffectSlot field_58;
    unsigned char field_84[0x4C];
    BattleModelHandle *field_D0;
    void *field_D4;
    void *vtable;
} BattleEffectOwner;

/* 0x18-byte object at +0x40 of BattleEffectOwner (ctor 0x089E5A18). */
typedef struct BattleEffectBind {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    void *vtable;
} BattleEffectBind;

/* 0x30-byte object at +0x10 of BattleEffectOwner (ctor 0x089E60A8). */
typedef struct BattleEffectModel {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    float field_14;
    float field_18;
    void *field_1C;
    void *field_20;
    void *field_24[2];
    void *vtable;
} BattleEffectModel;

/* Subobject at +0xA8C of the battle setup root (dtor 0x089E6928). */
typedef struct BattleSlotGate {
    unsigned char field_00;
    unsigned char field_01[4];
    unsigned char field_05;
    unsigned char padding_06[2];
    BattleMotionSet field_08;
    unsigned char field_124[4];
} BattleSlotGate;

#endif
