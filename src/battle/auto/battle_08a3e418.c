#include "battle/auto_region3.h"

void *func_088DAA40(void *model, int index, int arg);
void *func_088DAA68(void *model);
int func_08A3DFD0(Model3E874 *self);

/* Marks the object ready and takes five handles from the model at field_224. */
void func_08A3E418(Model3E874 *self)
{
    void *handle;

    self->field_250 = 1;
    handle = func_088DAA68(self->field_224);
    self->field_228[8] = handle;
    handle = func_088DAA40(self->field_224, 9, 0);
    self->field_228[6] = handle;
    handle = func_088DAA40(self->field_224, 2, 0);
    self->field_228[7] = handle;
    handle = func_088DAA40(self->field_224, 10, 0);
    self->field_228[4] = handle;
    handle = func_088DAA40(self->field_224, 8, 0);
    self->field_228[5] = handle;
    func_08A3DFD0(self);
}
