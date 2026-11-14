#include "battle/auto_region5.h"

/* Part4785C: runs func_088D3F00 and func_088D44EC over the handles and
 * models. */
void func_08B47FD4(Part4785C *self)
{
    func_088D3F00(0, self->handles, D_08B484C0);
    func_088D44EC(self->handles, self->models, D_08B484D8);
}
