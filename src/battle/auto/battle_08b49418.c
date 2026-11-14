#include "battle/auto_region5.h"

/* Part49220: runs func_088D3F00 and func_088D44EC over the handles and the
 * model. */
void func_08B49418(Part49220 *self)
{
    func_088D3F00(0, self->handles, D_08B49838);
    func_088D44EC(self->handles, self->models, D_08B49870);
}
