#include "menu/auto_region4.h"

/* ScreenB01CF8: releases the second messages object. */
void func_08AF7BA8(ScreenB01CF8X *self)
{
    WorkB01CF8X *work;
    void *messages;

    work = self->work;
    messages = work->field_8A0;
    if (messages != 0) {
        if (messages != 0) {
            func_0887B1F4(messages, 3);
            self->work->field_8A0 = 0;
            work = self->work;
        }
        work->field_8A4 = -1;
    }
}
