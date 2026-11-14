#include "menu/agent29_types.h"

/* Screen0B6A0: scores an event: picks its bonus kind from the event's
   kind and flags; the same kind again doubles the bonus (+0x272C),
   another kind starts from its base value in the bonus table (+0x5C),
   capped at the table's first value; the bonus is added to the score
   (+0x27A4, at most 99999999) and the texts redrawn. */
typedef struct BonusTable29 {
    short max;
    short field_02;
    short base[9];
} BonusTable29;

typedef struct ScoreEvent29 {
    unsigned char kind;
    char pad_01[0x3];
    unsigned int flags;
} ScoreEvent29;

extern void func_089F3CC8(Screen0B6A0 *self);

void func_08A01CEC(Screen0B6A0 *self, ScoreEvent29 *event)
{
    short bonus = -1;

    switch (event->kind) {
    case 1:
        if (event->flags & 2) {
            bonus = 4;
        } else if (event->flags & 4) {
            bonus = 6;
        } else if (event->flags & 8) {
            bonus = 7;
        } else if (event->flags & 0x800) {
            bonus = 8;
        } else {
            bonus = 5;
        }
        break;
    case 2:
        if (event->flags & 0x1000) {
            bonus = 3;
        } else if (event->flags & 0x2000) {
            bonus = 2;
        } else {
            bonus = 0;
        }
        break;
    case 3:
        bonus = 1;
        break;
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    }
    if (bonus != -1) {
        if (bonus != WORK_SHORT(self->work, 0x272A)) {
            WORK_SHORT(self->work, 0x272A) = bonus;
            WORK_SHORT(self->work, 0x272C) = ((BonusTable29 *)WORK_INT(self->work, 0x5C))->base[WORK_SHORT(self->work, 0x272A)];
        } else {
            WORK_SHORT(self->work, 0x272C) *= 2;
        }
        if (WORK_SHORT(self->work, 0x272C) > ((BonusTable29 *)WORK_INT(self->work, 0x5C))->max) {
            WORK_SHORT(self->work, 0x272C) = ((BonusTable29 *)WORK_INT(self->work, 0x5C))->max;
        }
        WORK_INT(self->work, 0x27A4) += WORK_SHORT(self->work, 0x272C);
        if (WORK_INT(self->work, 0x27A4) > 99999999) {
            WORK_INT(self->work, 0x27A4) = 99999999;
        }
        func_089F3CC8(self);
    }
}
