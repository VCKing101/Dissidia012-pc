#include "menu/agent29_types.h"

/* MenuRoot: stops its task once the save is ready (func_088AC9A8) and the
   shared object is idle. */
extern int func_0887E718(void *object);

void func_08A1B8C8(MenuRoot *self)
{
    if (func_088AC9A8(D_089D712C) == 0) {
        return;
    }
    if (func_0887E718(D_089D6998) != 0) {
        return;
    }
    func_08865158((MenuTask *)&self->pad_00[0x100]);
}
