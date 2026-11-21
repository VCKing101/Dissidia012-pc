#include "menu/agent36_types.h"

/* Menu root: once the network check has finished, stores the loaded save
 * (effect block) in the save data and rebuilds the list. Returns 1 while
 * the check runs, else 0. */
int func_08A1F468(MenuRoot36 *self)
{
    int result = 1;
    int state = func_08A1F550();
    SaveSlot36 info;

    if (state != 0) {
        if (state > 0) {
            if (func_08A1D8E4(self) == 0) {
                func_088F5DCC(&info, &self->effect->save_info);
                if (func_088F5EA0(info.body, self->effect->save_info.body) == 0) {
                    info.flags |= 2;
                }
                func_088F526C(D_089D712C->slots, &info);
                func_08A1C0B8(self);
            }
            func_088ACA10(D_089D712C);
            func_08A1C2F8(self);
        }
        result = 0;
    }
    return result;
}
