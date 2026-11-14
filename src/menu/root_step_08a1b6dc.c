#include "menu/agent29_types.h"

/* MenuRoot: runs the step D_08A23A84[+0xD88] with +0xD8C and keeps its
   result (+0x67B0); state 4. */
typedef int (*RootStep29)(int arg);
extern RootStep29 D_08A23A84[];

void func_08A1B6DC(MenuRoot *self)
{
    WORK_INT(self->work, 0x67B0) = D_08A23A84[WORK_INT(self->work, 0xD88)](WORK_INT(self->work, 0xD8C));
    WORK_INT(self->work, 0xD7C) = 4;
}
