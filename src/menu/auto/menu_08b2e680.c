#include "menu/auto_region4.h"

/* Obj2D604: what the current state allows (-1 nothing, 0 back, 1 on,
 * 2 on and finished). */
int func_08B2E680(Obj2D604 *self)
{
    Work2D604 *work;
    int state;
    int result;

    work = self->work;
    state = work->state;
    result = -1;
    if (state == 8) {
        result = 0;
    }
    else if (state == 2) {
        result = 1;
        if (work->cursor.index == 5) {
            result = 2;
        }
        else if (work->field_62C.rows[work->cursor.index].field_00 == 0) {
            result = 2;
        }
    }
    else if (state == 3) {
        result = 1;
    }
    else if (state == 4) {
        result = 1;
        if ((work->field_6C4 != 0) && (func_08A81204(work->field_6C4) == 0)) {
            result = 2;
        }
    }
    return result;
}
