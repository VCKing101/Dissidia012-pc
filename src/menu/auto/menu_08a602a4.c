#include "menu/auto_region2.h"

/* Panel60670: opens with messages and count, state 1. */
void func_08A602A4(Panel60670 *self, MenuModel *model, int arg2, void *messages, int count, int arg5)
{
    self->messages = messages;
    self->field_30 = count;
    func_08A60570(self);
    func_08A60310(self, messages, count, arg5);
    self->state = 1;
}
