#include "menu/agent29_types.h"

/* MenuRoot: starts the save (func_088F68E8 on the root work's save block
   at +0x8E50) once, unless the root is busy; gives 1 when it has
   finished (flag 0x20 of +0x8E60). The last argument says whether the
   network is up with +0x7BBC set. */
extern int func_089C5808(void);
extern void func_088F68E8(void *name, void *data, int arg2, int fresh, int arg4, int online);

int func_08A1DF84(MenuRoot *self)
{
    int done = 0;
    int online;
    int fresh;

    if (WORK_INT(self->work, 0x8E60) & 0x20) {
        done = 1;
    } else if ((WORK_INT(self->work, 0x8E60) & 0x10) == 0 && func_08A1D8E4(self) == 0) {
        online = 0;
        if (func_089C5808() == 1 && WORK_BYTE(self->work, 0x7BBC) != 0) {
            online = 1;
        }
        fresh = WORK_BYTE(self->work, 0x8E5F) != 1;
        func_088F68E8(&WORK_BYTE(self->work, 0x8E56), &WORK_BYTE(self->work, 0x8E50), 1, fresh, 0, online);
        WORK_INT(self->work, 0x8E60) |= 0x10;
    }
    return done;
}
