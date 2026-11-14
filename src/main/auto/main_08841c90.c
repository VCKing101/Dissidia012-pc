#include "main/auto_region2.h"

/* Shuts the world down: func_088083A8, func_0884E650, then forgets the heap. */
void func_08841C90(WorldView *self)
{
    func_088083A8();
    func_0884E650();
    self->heap = 0;
}
