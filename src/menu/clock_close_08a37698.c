#include "menu/agent36_types.h"

/* Clock: once both message box checks pass and the session link is idle,
 * closes the session: releases the link, frees the block and resets the
 * table (state 3). */

extern void *D_089D6C30;
int func_0889F280(void *net);
int func_088857DC(void *box);
int func_08885854(void *box);
void func_08A367CC(ClockScreen36 *self);
void func_08A36838(ClockScreen36 *self);
void func_088A3EF8(ClockLink36 *link);
void func_088A3C48(void *table);

void func_08A37698(ClockScreen36 *self)
{
    if (func_088857DC(D_089D69E8) == 0 || func_08885854(D_089D69E8) == 0) {
        return;
    }
    if (self->work->field_D74 == 0 && func_0889F280(D_089D6C30) == 0) {
        func_08A367CC(self);
        func_08A36838(self);
        {
            int none = 0;
            self->work->link->field_31 = none;
        }
        func_088A3EF8(self->work->link);
        func_0885C538(D_089D5EC0->heap, self->work->block, 0);
        func_088A3C48(self->work->table);
        self->work->state = 3;
    }
}
