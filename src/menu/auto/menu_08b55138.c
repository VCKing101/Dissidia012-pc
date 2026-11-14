#include "menu/auto_region4.h"

/* Model550C8: copies the matrix and colour of source's entry index onto its model. */
void func_08B55138(Model550C8 *self, void *source, int index)
{
    float matrix[16];
    unsigned int color;

    func_088DA0A8(source, index, matrix, &color);
    func_088D9B84(self->model, matrix);
    func_088D9B48(self->model, color);
}
