#include "menu/auto_region4.h"

/* ScreenB1CB00: hides both rows of five and shows the picked one. */
void func_08B0DFD0(ScreenB1CB00 *self)
{
    WorkB1CB00 *work;
    int i;

    i = 0;
    work = self->work;
    do {
        func_088D9CFC(work->handles[i + 0xF8]);
        func_088D9CFC(self->work->handles[i + 0xFD]);
        i = i + 1;
        work = self->work;
    } while (i < 5);
    func_088D9CD8(work->handles[work->field_24814 + 0xF8]);
}
