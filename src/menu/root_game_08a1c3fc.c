#include "menu/agent29_types.h"

/* MenuRoot: raises flag 1 (+0x8E60) and starts the chosen game mode
   (D_09B6D840 +8): modes below 8 directly, the others through the game
   object and func_08A207B0; state 3. */
typedef struct RootRequest29 {
    int field_00;
    int field_04;
    int value;
} RootRequest29;

extern RootRequest29 D_09B6D840;
extern void func_08900B7C(void *game);
extern void func_08A207B0(MenuRoot *self, int arg);

void func_08A1C3FC(MenuRoot *self)
{
    WORK_INT(self->work, 0x8E60) |= 1;
    if ((&D_09B6D840)->value < 8) {
        func_08900BE4(D_089D8140);
    } else {
        func_08900B7C(D_089D8140);
        func_08A207B0(self, 0);
    }
    WORK_INT(self->work, 0x6968) = 3;
}
