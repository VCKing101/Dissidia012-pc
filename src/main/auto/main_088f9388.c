#include "main/auto_region4.h"

/* Obj8F9388: constructor. */
Obj8F9388 *func_088F9388(Obj8F9388 *self)
{
    int i;

    func_088CB9E8();
    for (i = 0; i < 0x14; i++) {
        self->field_CE4[i] = 0;
    }
    for (i = 0; i < 0x80; i++) {
        self->field_D34[i] = 0;
    }
    return self;
}
