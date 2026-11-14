#include "menu/auto_region2.h"

extern const short D_08A455F0[];

/* MenuRoot2: starts the D_089D7F90 player with script D_08A455F0. */
void func_08A42B08(MenuRoot2 *self)
{
    func_088FD928(D_089D7F90, self->heap, 7, 0x11, D_08A455F0, 0x4033, self->work->messages, 7, 0, 0,
                  0, 0);
}
