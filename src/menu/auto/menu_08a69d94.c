#include "menu/auto_region2.h"

/* Panel6B170: starts its layout script, then opens the panel base. */
void func_08A69D94(Panel6B170 *self, int arg1, int arg2, int arg3)
{
    func_08A5BCF0(&self->layout);
    func_08A51E64(&self->base, arg1, arg2, arg3);
}
