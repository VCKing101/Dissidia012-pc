#include "menu/agent29_types.h"

/* MenuRoot: sets up its task (id 0x11179) and its three callbacks
   (function, owner pairs at +0x15C, +0x164 and +0x170), and resets the
   request block D_09B6D840. */
typedef struct RootRequest29 {
    int state;
    int field_04;
    int value;
    char pad_0C[0x4];
    int field_10;
    int field_14;
    char pad_18[0x1];
    unsigned char field_19;
    char pad_1A[0x2];
    int field_1C;
    unsigned char field_20;
} RootRequest29;

extern RootRequest29 D_09B6D840;
extern char D_08A23410[];
extern char D_08A23458[];
extern char D_08A234A0[];

MenuRoot *func_08A1D364(MenuRoot *self)
{
    RootRequest29 *request;

    func_08864FF0((MenuTask *)&self->pad_00[0x100], 1, 0x11179, 0x4C);
    WORK_INT(self, 0x15C) = (int)D_08A23410;
    WORK_INT(self, 0x160) = (int)self;
    WORK_INT(self, 0x164) = (int)D_08A23458;
    WORK_INT(self, 0x168) = (int)self;
    WORK_INT(self, 0x170) = (int)D_08A234A0;
    WORK_INT(self, 0x174) = (int)self;
    D_09B6D840.state = 0;
    request = &D_09B6D840;
    request->field_1C = 0;
    request->field_04 = -1;
    request->value = 0;
    request->field_19 = 1;
    request->field_10 = 0;
    request->field_14 = 0;
    request->field_20 = 0;
    return self;
}
