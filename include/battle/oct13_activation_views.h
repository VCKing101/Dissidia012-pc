#ifndef BATTLE_OCT13_ACTIVATION_VIEWS_H
#define BATTLE_OCT13_ACTIVATION_VIEWS_H

#include "battle/runtime_helpers.h"

/* Offset-only views for the second October 13 activation/cleanup group.
 * Complete classes, owner lifetimes and gameplay roles remain unresolved. */
typedef struct BattleOct13Peer198 {
    unsigned char padding_00[0x16C];
    void *field_16C;
    unsigned char padding_170[0x28];
    void *field_198;
} BattleOct13Peer198;
typedef struct BattleOct13Peer1CC { unsigned char padding[0x1CC]; void *field_1CC; } BattleOct13Peer1CC;
typedef struct BattleOct13Prefix04 { unsigned char field_00; unsigned char padding_01[3]; void *field_04; } BattleOct13Prefix04;
typedef struct BattleOct13Bind14 {
    unsigned char field_00;
    unsigned char padding_01[0xB];
    unsigned int field_0C;
    void *field_10;
    void *field_14;
} BattleOct13Bind14;
typedef struct BattleOct13Bind18 {
    unsigned char field_00;
    unsigned char padding_01[0xF];
    unsigned int field_10;
    void *field_14;
    void *field_18;
} BattleOct13Bind18;
typedef struct BattleOct13FourResources {
    unsigned char field_00;
    unsigned char padding_01[3];
    float field_04;
    void *field_08;
    void *field_0C;
    void *field_10;
    void *field_14;
    unsigned int field_18;
    void *field_1C;
    void *field_20;
} BattleOct13FourResources;


extern void *D_089D7830;
void *func_088D09D8(void *manager, int flags, int kind);
void func_088DA060(void *record);
void *func_088908E0(void);
int func_0887AC70(void *block, void *buffer, int count, void *provider);
int func_0887AFB0(void *block);
void func_0887AD1C(void *block);
void func_089E3300(void *object);
int func_089E3A0C(void *object, unsigned int index, void *resource_a, void *resource_b);
void func_089E7200(void *object);
void func_089E1268(void *object);

int func_089E3358(void *object, BattleOct13Peer198 *peer);
int func_089E7258(void *object, BattleOct13Peer1CC *peer, void *shared);
int func_089ED064(void *object);
int func_089E12A8(void *object, unsigned int index, void *resource, void *shared);
int func_089E0D70(void *object, void *resource);
int func_089E74A8(void *object, unsigned int index, void *resource, void *shared);
void func_089E0D54(void *object);
int func_089E0BE8(void *object, void *buffer, int count);
void func_089E0C6C(void *object);
void func_089E9024(void *object);
void func_089E8FF8(void *object);

#endif
