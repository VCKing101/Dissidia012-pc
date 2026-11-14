#include "menu/auto_region1.h"

/* ObjEE228 constructor. */
ObjEE228 *func_089EE228(ObjEE228 *self)
{
    func_08993560(self->field_10, 3, sizeof(ElemEDDD8), func_089EDDD8);
    self->field_58 = 0;
    self->field_5C = 0;
    self->state = 0;
    func_08935828(self->models, 0, sizeof(self->models));
    func_08935828(&self->field_0C, 0, sizeof(self->field_0C));
    return self;
}
