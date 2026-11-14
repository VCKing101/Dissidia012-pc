#include "menu/auto_region1.h"

/* Screen0B6A0: hides the nine handles from index on. */
void func_089F5990(Screen0B6A0 *self, int index)
{
    func_088D9CFC(self->work->handles[index]);
    func_088D9CFC(self->work->handles[index + 1]);
    func_088D9CFC(self->work->handles[index + 2]);
    func_088D9CFC(self->work->handles[index + 6]);
    func_088D9CFC(self->work->handles[index + 5]);
    func_088D9CFC(self->work->handles[index + 4]);
    func_088D9CFC(self->work->handles[index + 3]);
    func_088D9CFC(self->work->handles[index + 7]);
    func_088D9CFC(self->work->handles[index + 8]);
}
