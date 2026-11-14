#include "menu/agent29_types.h"

/* MenuRoot: releases the help window and its 0x1B texts (+0x890); state 9. */
extern void func_08885220(void *object);

void func_08A1B938(MenuRoot *self)
{
    int i;

    func_08885220(D_089D69E8);
    for (i = 0; i < 0x1B; i++) {
        if ((&WORK_INT(self->work, 0x890))[i] != 0) {
            func_0887BA5C((void *)(&WORK_INT(self->work, 0x890))[i]);
            (&WORK_INT(self->work, 0x890))[i] = 0;
        }
    }
    WORK_INT(self->work, 0xD7C) = 9;
}
