#include "main/auto_region4.h"

/* Obj9CC308: resets, stores value and becomes active. */
unsigned int func_088DF11C(Obj9CC308 *self, unsigned char value)
{
    func_088DF100(self);
    self->field_01 = value;
    self->active = 1;
    return 0;
}
