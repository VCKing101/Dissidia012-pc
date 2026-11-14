#include "menu/auto_region1.h"

/* Screen0B6A0: when handle 2 shows something, copies its matrix and colour
 * onto text 0x70. */
void func_089FAFA4(Screen0B6A0 *self)
{
    MenuModel *model;
    float matrix[16];
    unsigned int color;

    model = self->work->handles[2];
    if ((model->field_0C != -1) && (self->work->texts[0x70] != 0)) {
        func_088DA0A8(model, 0, matrix, &color);
        func_0887B778(self->work->texts[0x70], matrix);
        func_0887B828(self->work->texts[0x70], &color);
    }
}
