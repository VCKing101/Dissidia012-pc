#include "menu/auto_region4.h"

/* ScreenB01CF8: clears its texts and resets the panel. */
void func_08AF7ECC(ScreenB01CF8X *self)
{
    WorkB01CF8X *work;
    int i;

    i = 0;
    work = self->work;
    do {
        work->texts[i] = 0;
        i = i + 1;
        work = self->work;
    } while (i < 0x5D);
    if (work->panel != 0) {
        func_08AF08F0(work->panel);
    }
}
