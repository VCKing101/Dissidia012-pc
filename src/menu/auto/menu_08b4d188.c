#include "menu/auto_region4.h"

extern const short D_08B4EB30[];

void func_08B4D188(Part4CEDC *self)
{
    float matrix[16];
    unsigned int color;

    func_088DA0A8(self->model, self->index, matrix, &color);
    func_088D9B84(self->handles[2], matrix);
    func_088D9AF4(self->handles[2], &color);
    func_088D3F00(0, self->handles, D_08B4EB30);
}
