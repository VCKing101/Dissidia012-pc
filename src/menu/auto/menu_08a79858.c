#include "menu/auto_region3.h"

extern int *D_08A6D7E0;

/* Step79858: runs func_08A6FB68 if *D_08A6D7E0 is set, then func_088F69F8; state 1. */
void func_08A79858(Step79858 *self)
{
    if (*D_08A6D7E0 != 0) {
        func_08A6FB68();
    }
    func_088F69F8();
    self->state = 1;
}
