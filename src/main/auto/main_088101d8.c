#include "main/auto_region1.h"

/* Steps the member part and counts field_B8 down by dt. */
void func_088101D8(float dt, Obj80FB6C *self)
{
    func_0886F08C(self->field_9C);
    func_0886AB1C(self->field_98, self->field_9C, 1);
    self->field_B8 = self->field_B8 - dt;
}
