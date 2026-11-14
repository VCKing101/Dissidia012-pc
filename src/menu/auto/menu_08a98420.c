#include "menu/auto_region3.h"

/* ScreenA3F44: checks and releases, then enters the next state. */
void func_08A98420(ScreenA3F44 *self)
{
    func_08A9A460(self);
    func_08A9A4CC(self);
    self->work->state = self->work->next;
}
