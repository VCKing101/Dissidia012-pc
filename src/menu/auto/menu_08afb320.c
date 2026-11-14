#include "menu/auto_region4.h"

extern const short D_08B00F22[];

/* ScreenB01CF8: releases texts 0x42..0x5C and replays the layout. */
void func_08AFB320(ScreenB01CF8X *self)
{
    WorkB01CF8X *work;
    void *text;
    int i;

    i = 0x42;
    work = self->work;
    do {
        text = work->texts[i];
        if (text != 0) {
            func_0887BA5C(text);
            self->work->texts[i] = 0;
            work = self->work;
        }
        i = i + 1;
    } while (i < 0x5D);
    /* layout and handles as byte offsets: &work->layout / work->handles of
     * this reassigned local cost an extra register copy */
    func_088D3C24((char *)work + 0x1C, (char *)work + 0x41C, D_08B00F22, 0x120);
    func_08AF70E4((ScreenB01CF8 *)self);
}
