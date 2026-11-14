#include "menu/auto_region4.h"

/* Part4CA98: copies the matrix and colour of its model's entry onto its handle. */
void func_08B4CC90(Part4CA98 *self)
{
    float matrix[16];
    unsigned int color;

    func_088DA0A8(self->model, self->index, matrix, &color);
    func_088D9B84(self->handles[0], matrix);
    func_088D9AF4(self->handles[0], &color);
}
