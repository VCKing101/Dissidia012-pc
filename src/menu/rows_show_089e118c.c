#include "menu/agent29_types.h"

/* ScreenE75C4: after the cursor and list updates, plays the show motion of
   every row after the current one (rows 4/5 when the row's entry is set,
   7/8 otherwise) and moves to state 2. */
void func_089E118C(ScreenE75C4 *self)
{
    int i;

    func_089E1CA4(self);
    func_089E1DD8(self);
    if (func_089E21C8(self) != 0) {
        for (i = WORK_INT(self->work, 0xA88) + 1; i < 7; i++) {
            if (D_089E7488[D_089E7518[i].entry].enabled != 0) {
                func_088D9F5C(self->work->handles[D_089E7518[i].handle], 4, 1);
                MENU_MODEL_STATE(self->work->handles[D_089E7518[i].handle]) = 5;
            } else {
                func_088D9F5C(self->work->handles[D_089E7518[i].handle], 7, 1);
                MENU_MODEL_STATE(self->work->handles[D_089E7518[i].handle]) = 8;
            }
        }
        self->work->state = 2;
    }
}
