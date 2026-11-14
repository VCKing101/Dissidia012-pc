#include "main/auto_region4.h"

/* Obj8DA8E8: the same for field_10 and flag 8. */
void func_088DB254(Obj8DA8E8 *self, int id, short high, short low)
{
    if (self->field_10.id == id) {
        self->field_10.half[1] = high;
        self->field_10.half[0] = low;
        self->flags = self->flags | 8;
    }
}
