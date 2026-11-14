#include "menu/auto_region4.h"

/* Obj2D604 step: polls the grid panel; 1 takes its rows and finishes,
 * 2 takes its rows and finishes in modes 2 and 3 (else back to state 1). */
int func_08B2D654(Obj2D604 *self)
{
    int status;
    int result;
    int mode;

    result = 0;
    status = func_08B2D840(self);
    if (status != 0) {
        if (status == 1) {
            func_08B30804(self->work->field_528, &self->work->field_62C);
            result = 1;
        }
        else if (status == 2) {
            func_08B30804(self->work->field_528, &self->work->field_62C);
            mode = self->work->field_830;
            if ((mode == 2) || (mode == 3)) {
                result = 1;
            }
            else {
                self->work->state = 1;
            }
        }
        func_08B2D860(self);
    }
    return result;
}
