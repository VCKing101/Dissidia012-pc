#ifndef BATTLE_DRAWING_STEP_H
#define BATTLE_DRAWING_STEP_H

#include "main/drawing_record.h"

/* Only the state fields touched by the drawing-step routine are modeled. */
typedef struct BattleDrawingStep {
    unsigned char padding_00[4];
    unsigned int field_04;
    unsigned char padding_08[4];
    int field_0C;
    unsigned char padding_10[0xC4];
    MainDrawingRecordPrefix *field_D4;
} BattleDrawingStep;

#endif
