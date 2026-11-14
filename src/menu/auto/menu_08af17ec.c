#include "menu/auto_region3.h"

/* ScreenB01CF8: copies field_578's matrix and colour to text field_988. */
void func_08AF17EC(ScreenB01CF8 *self)
{
    MenuModel *model;
    float matrix[16];
    unsigned int color;

    model = self->work->field_578;
    if ((model->field_0C != -1) && (self->work->field_988 != 0)) {
        func_088DA0A8(model, 0, matrix, &color);
        func_0887B778(self->work->field_988, matrix);
        func_0887B828(self->work->field_988, &color);
    }
}
