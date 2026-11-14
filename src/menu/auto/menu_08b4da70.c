#include "menu/auto_region4.h"

extern const short D_08B4EB50[];

void func_08B4DA70(Part4D4DC *self)
{
    float matrix[16];
    unsigned int color;

    func_088DA0A8(self->model, self->index, matrix, &color);
    func_088D9B84(self->handles[0xA], matrix);
    func_088D9AF4(self->handles[0xA], &color);
    func_088D9B84(self->handles[0xB], matrix);
    func_088D9AF4(self->handles[0xB], &color);
    func_088D3F00(0, self->handles, D_08B4EB50);
}
