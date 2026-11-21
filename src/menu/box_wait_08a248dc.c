#include "menu/agent36_types.h"

/* Screen28560: once the menu root is ready and no message box is open,
 * asks the box; on an answer the screen goes to state 3. */
void func_08A248DC(Screen28560x *self)
{
    func_08A1E168(D_08A23FF0);
    if (func_08A1E1A8(D_08A23FF0) == 0) {
        return;
    }
    if (func_08A20378(D_08A23FF0) == 0) {
        return;
    }
    if (func_08885938(D_089D69E8) != 0) {
        return;
    }
    func_08885988(D_089D69E8);
    if (func_08885608(D_089D69E8, 1) != 0) {
        self->work->state = 3;
    }
}
