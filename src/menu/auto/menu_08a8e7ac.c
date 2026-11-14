#include "menu/auto_region3.h"

/* View8E3B4: func_088D4F9C on the prompt; sets flag 4. */
void func_08A8E7AC(View8E3B4 *self)
{
    func_088D4F9C(&self->work->prompt);
    self->work->flags = self->work->flags | 4;
}
