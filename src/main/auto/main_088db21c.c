#include "main/auto_region4.h"

/* Obj8DA8E8: if field_0C holds id, replaces it with the two shorts and sets flag 4. */
void func_088DB21C(Obj8DA8E8 *self, int id, short high, short low)
{
    if (self->field_0C.id == id) {
        self->field_0C.half[0] = low;
        self->field_0C.half[1] = high;
        self->flags = self->flags | 4;
    }
}
