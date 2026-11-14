#include "menu/auto_region3.h"

/* PanelC18: copies handle 0x54's matrix and colour to text 0x14. */
void func_08AC7620(PanelC18 *self)
{
    MenuModel *model;
    float matrix[16];
    unsigned int color;

    model = self->work->handles[0x54];
    if ((model->field_0C != -1) && (self->work->texts[0x14] != 0)) {
        func_088DA0A8(model, 0, matrix, &color);
        func_0887B778(self->work->texts[0x14], matrix);
        func_0887B828(self->work->texts[0x14], &color);
    }
}
