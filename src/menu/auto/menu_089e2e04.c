#include "menu/auto_region1.h"

/* ScreenE75C4: when handle 0x46 shows something, copies its matrix and
 * colour onto text 0x50. */
void func_089E2E04(ScreenE75C4 *self)
{
    MenuModel *model;
    float matrix[16];
    unsigned int color;

    model = self->work->handles[0x46];
    if ((model->field_0C != -1) && (self->work->texts[0x50] != 0)) {
        func_088DA0A8(model, 0, matrix, &color);
        func_0887B778(self->work->texts[0x50], matrix);
        func_0887B828(self->work->texts[0x50], &color);
    }
}
