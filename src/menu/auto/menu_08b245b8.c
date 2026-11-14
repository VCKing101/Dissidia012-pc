#include "menu/auto_region4.h"

/* ScreenB25044: closes and releases its messages. */
void func_08B245B8(ScreenB25044 *self)
{
    WorkB25044 *work;

    func_08B249CC();
    func_08885220(D_089D69E8);
    func_088D3A04(&self->work->layout);
    work = self->work;
    if (work->messages != 0) {
        func_0887B1F4(work->messages, 3);
        self->work->messages = 0;
        work = self->work;
    }
    work->state = 4;
}
