#include "menu/auto_region1.h"

/* Copies the matrix and colour of source's entry index onto model. */
void func_089ED0D8(void *self, MenuModel *model, void *source, int index)
{
    float matrix[16];
    unsigned int color;

    if (model != 0) {
        func_088DA0A8(source, index, matrix, &color);
        func_088D9B84(model, matrix);
        func_088D9B48(model, color);
    }
}
