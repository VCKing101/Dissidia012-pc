#include "main/auto_region2.h"

/* out = field_330 combined (func_089A3F40) with the vector at +0x30 of the model part. */
void func_08838DCC(ActorView *self, float *out, int part)
{
    char *data;

    data = func_0886B590(self->model, part);
    func_089A3F40(out, self->field_330, data + 0x30);
}
