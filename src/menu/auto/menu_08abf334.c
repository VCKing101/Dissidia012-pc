#include "menu/auto_region3.h"

extern const short D_08AC4452[];

/* ScreenC4560: func_088FD928 on D_089D7F90 with script D_08AC4452. */
void func_08ABF334(ScreenC4560 *self)
{
    func_088FD928(D_089D7F90, self->heap, 0x16, 10, D_08AC4452, 0x2416, self->work->messages, 1, 0, 0,
                  0, 0);
}
