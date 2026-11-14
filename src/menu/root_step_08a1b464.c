#include "menu/agent29_types.h"

/* MenuRoot: runs the step D_08A23A84[+0xD84] and keeps its result
   (+0x67B0); state 2. */
typedef int (*RootStep29)(int arg);
extern RootStep29 D_08A23A84[];

void func_08A1B464(MenuRoot *self)
{
    int step = WORK_INT(self->work, 0xD84);
    WORK_INT(self->work, 0x67B0) = D_08A23A84[step](step);
    WORK_INT(self->work, 0xD7C) = 2;
}
