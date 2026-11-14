#include "menu/auto_region4.h"

/* ScreenB01CF8: state is 3 or 4. */
int func_08AF9F1C(ScreenB01CF8 *self)
{
    int result;
    int state;

    state = ((ScreenB01CF8X *)self)->work->state;
    result = 0;
    if ((state == 4) || (state == 3)) {
        result = 1;
    }
    return result;
}
