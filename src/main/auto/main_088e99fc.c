#include "main/auto_region4.h"

/* Frees the block of D_089D7E88. */
void func_088E99FC(void)
{
    Obj9D7E88 *self;

    self = D_089D7E88;
    func_08882158(D_089D7E88->block);
    self->block = 0;
}
