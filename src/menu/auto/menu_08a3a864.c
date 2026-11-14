#include "menu/auto_region1.h"

/* Obj3A864 constructor: builds its sixteen elements, then func_08A3A8A8. */
Obj3A864 *func_08A3A864(Obj3A864 *self)
{
    func_08993560(self->field_04, 0x10, sizeof(Elem3A4F0), func_08A3A4F0);
    func_08A3A8A8(self);
    return self;
}
