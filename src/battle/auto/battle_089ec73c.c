#include "battle/auto_region1.h"

/* Moves to state 5 unless field_2D is set; a set field_2D is cleared instead. */
int func_089EC73C(PartECED0 *self)
{
    if (!self->field_2D) {
        self->state = 5;
    } else {
        self->field_2D = 0;
    }
    return 0;
}
