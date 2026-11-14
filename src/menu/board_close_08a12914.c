#include "menu/agent29_types.h"

/* Screen15A88: once the menu root's lists are ready and the help window
   has closed, closes the board (func_08A12E34) and moves to state 0xF. */
extern void func_08A1E168(MenuRoot *root);
extern int func_08A1E1A8(MenuRoot *root);
extern int func_08885938(void *object);
extern void func_08885988(void *object);
extern int func_08885608(void *object, char value);
extern void func_08A12E34(Screen15A88 *self);

void func_08A12914(Screen15A88 *self)
{
    func_08A1E168(D_08A23FF0);
    if (func_08A1E1A8(D_08A23FF0) == 0) {
        return;
    }
    if (func_08885938(D_089D69E8) != 0) {
        return;
    }
    func_08885988(D_089D69E8);
    if (func_08885608(D_089D69E8, 1) == 0) {
        return;
    }
    func_08A12E34(self);
    self->work->state = 0xF;
}
