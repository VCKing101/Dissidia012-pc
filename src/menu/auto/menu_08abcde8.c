#include "menu/auto_region3.h"

/* ScreenC4560: copies handle 0x20's matrix and colour to text 0x15. */
void func_08ABCDE8(ScreenC4560 *self)
{
    MenuModel *model;
    float matrix[16];
    unsigned int color;

    model = self->work->handles[0x20];
    if ((model->field_0C != -1) && (self->work->texts[0x15] != 0)) {
        func_088DA0A8(model, 0, matrix, &color);
        func_0887B778(self->work->texts[0x15], matrix);
        func_0887B828(self->work->texts[0x15], &color);
    }
}
