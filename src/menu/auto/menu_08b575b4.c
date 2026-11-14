#include "menu/auto_region4.h"

/* Copies the matrix and colour of source's entry index onto model. */
void func_08B575B4(void *self, MenuModel *model, void *source, int index)
{
    float matrix[16];
    unsigned int color;

    func_088DA0A8(source, index, matrix, &color);
    func_088D9B84(model, matrix);
    func_088D9B48(model, color);
}
