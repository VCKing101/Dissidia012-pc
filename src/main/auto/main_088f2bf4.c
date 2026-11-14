#include "main/auto_region4.h"

/* Raises field_156C to value. */
void func_088F2BF4(Obj8F2B84 *self, int value)
{
    if (self->field_156C < value) {
        self->field_156C = value;
    }
}
