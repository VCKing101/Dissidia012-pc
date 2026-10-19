#ifndef BATTLE_OCT13_SOURCE_VIEWS_H
#define BATTLE_OCT13_SOURCE_VIEWS_H

/* Minimum-offset views for this source batch. Owning class identities and
 * complete object extents remain unknown. No inheritance or runtime objects. */
typedef struct BattleCurveView {
    unsigned char field_00;
    unsigned char padding_01[3];
    unsigned int field_04;
    void *field_08;
    int field_0C;
    int field_10;
    float field_14;
    float field_18;
} BattleCurveView;
typedef struct BattleResource18View { unsigned char padding[0x18]; void *field_18; } BattleResource18View;
typedef struct BattleResource04View { unsigned char padding[4]; void *field_04; } BattleResource04View;
typedef struct BattleOct13ResetView {
    unsigned char field_00;
    unsigned char padding_01[3];
    void *field_04;
    int field_08;
    int field_0C;
    void *field_10;
    void *field_14;
} BattleOct13ResetView;
typedef struct BattleOct13BufferView {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    unsigned char padding_08[0x9C];
    unsigned char field_A4[0x30];
} BattleOct13BufferView;

#endif
