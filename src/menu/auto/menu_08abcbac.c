#include "menu/auto_region3.h"

extern const short D_08AC431A[];
extern const unsigned short D_08AC4336[];
extern const unsigned short D_08AC436E[];

/* ScreenC4560: opens the window with text value and its cursor. */
void func_08ABCBAC(ScreenC4560 *self, int value)
{
    void *text;
    unsigned int *flags;
    MenuModel **handles;

    handles = self->work->handles;
    func_088D3B88(&self->work->layout, handles, D_08AC431A, 0, 0x120);
    func_088D3CF0(&self->work->layout, handles, D_08AC4336);
    ((MenuModelView *)self->work->handles[0x1F])->field_0E = 4;
    if (self->work->texts[0x14] != 0) {
        func_0887BA5C(self->work->texts[0x14]);
        self->work->texts[0x14] = 0;
    }
    text = func_08ABBC4C(self, 0, value, 0, 0);
    self->work->texts[0x14] = text;
    func_088D428C(handles, self->work->texts, D_08AC436E, 1);
    func_088D46E4(&self->work->cursor, 1, 0, 1, 0, -1);
    flags = &self->work->cursor.flags;
    *flags = *flags | 1;
    func_08ABCCDC(self);
    self->work->field_B0C = 0;
    self->work->field_B44 = 0;
    self->work->flags = self->work->flags | 10;
}
