#include "menu/auto_region4.h"

/* Net45BB8: takes its data parts down and returns the data block. */
void func_08B45E4C(Net45BB8 *self)
{
    void *heap;

    func_089211C8(self->data->field_1C);
    func_08B4A3FC(&self->data->field_20);
    func_08B48A78(&self->data->field_24);
    func_08B45AD0(&self->data->field_28);
    heap = func_08B42B40();
    func_0885C538(heap, self->data, 0);
    self->data = 0;
    func_08B45E0C(self);
}
