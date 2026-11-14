#include "main/auto_region4.h"

/* Obj9CC290: state 0 once field_80 reports 1. */
int func_088DE8B8(Obj9CC290 *self)
{
    int result;

    result = func_088DF824(&self->field_80);
    if (result == 1) {
        self->state = 0;
    }
    return 0;
}
