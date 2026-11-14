#include "menu/auto_region2.h"

/* MenuRoot2: copies handle 0x14's matrix and colour to text 6 while both exist. */
void func_08A416DC(MenuRoot2 *self)
{
    MenuModel *model;
    float matrix[16];
    unsigned int color;

    model = self->work->handles[0x14];
    if (model->field_0C != -1 && self->work->texts[6] != 0) {
        func_088DA0A8(model, 0, matrix, &color);
        func_0887B778(self->work->texts[6], matrix);
        func_0887B828(self->work->texts[6], &color);
    }
}
