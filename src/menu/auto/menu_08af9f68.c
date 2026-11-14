#include "menu/auto_region4.h"

/* ScreenB01CF8: state is 1 or 7. */
int func_08AF9F68(ScreenB01CF8X *self)
{
    int result;
    int state;

    state = self->work->state;
    result = 0;
    if ((state == 1) || (state == 7)) {
        result = 1;
    }
    return result;
}
