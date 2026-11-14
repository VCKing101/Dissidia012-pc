#include "menu/auto_region1.h"

/* ScreenE75C8: when field_51C shows something, copies its matrix and colour
 * onto the text field_928. */
void func_08A2B3F8(ScreenE75C8 *self)
{
    MenuModel *model;
    float matrix[16];
    unsigned int color;

    model = self->work->field_51C;
    if ((model->field_0C != -1) && (self->work->field_928 != 0)) {
        func_088DA0A8(model, 0, matrix, &color);
        func_0887B778(self->work->field_928, matrix);
        func_0887B828(self->work->field_928, &color);
    }
}
