#ifndef DISSIDIA_BATTLE_STATE_PREFIX_H
#define DISSIDIA_BATTLE_STATE_PREFIX_H

/* Partial structures describe only the fields touched by these routines.
 * Their owning classes and field meanings remain unknown. Padding is preserved. */
typedef struct BattleStatePrefix8 {
    unsigned char field_00;
    unsigned char padding_01[3];
    unsigned int field_04;
} BattleStatePrefix8;

typedef struct BattleStatePrefix12 {
    unsigned char field_00;
    unsigned char padding_01[3];
    unsigned int field_04;
    unsigned int field_08;
} BattleStatePrefix12;

#endif
