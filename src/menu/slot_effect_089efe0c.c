#include "menu/agent29_types.h"

/* Screen0B6A0: applies the effect of slot `id` by its kind: sets page
   flags, opens a sub-page or returns the page to go to (0 when none),
   then records the slot's value at +0x3450. */
extern int func_089EF880(Screen0B6A0 *self, int id);
extern int func_089EF8BC(Screen0B6A0 *self, int id);
extern void func_089FC590(Screen0B6A0 *self, int value);
extern void func_089FC91C(Screen0B6A0 *self);
extern void func_089F2334(Screen0B6A0 *self, int arg);

int func_089EFE0C(Screen0B6A0 *self, char id)
{
    int page = 0;

    switch (func_089EF880(self, id)) {
    case 0xB:
        self->work->flags |= 0x20;
        break;
    case 0xC:
        self->work->flags |= 0x40;
        break;
    case 0xD:
        self->work->flags |= 0x80;
        break;
    case 0xE:
        page = 2;
        break;
    case 0x13:
        page = 1;
        break;
    case 6:
        page = 3;
        break;
    case 1:
        page = 6;
        break;
    case 0x14:
        page = 7;
        break;
    case 0x18:
        self->work->flags |= 0x100000;
        break;
    case 0x15:
        func_089FC590(self, func_089EF8BC(self, id));
        func_089FC91C(self);
        break;
    case 0x17:
        func_089F2334(self, 1);
        break;
    case 0x19:
        page = 10;
        break;
    }
    WORK_INT(self->work, 0x3450) = func_089EF8BC(self, id);
    return page;
}
