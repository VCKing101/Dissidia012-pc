#include "menu/auto_region1.h"

/* ObjC1E0: copies the matrix and colour of source's entry 9 onto its model. */
void func_089EAE70(ObjC1E0 *self, void *source)
{
    float matrix[16];
    unsigned int color;

    if (self->model != 0) {
        func_088DA0A8(source, 9, matrix, &color);
        func_088D9B84(self->model, matrix);
        func_088D9B48(self->model, color);
    }
}
