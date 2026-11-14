#include "menu/auto_region1.h"

/* ObjEE850 constructor. */
ObjEE850 *func_089EE850(ObjEE850 *self)
{
    self->field_1C = 0;
    self->state = 0;
    func_08935828(self->models, 0, sizeof(self->models));
    func_08935828(&self->field_18, 0, sizeof(self->field_18));
    return self;
}
