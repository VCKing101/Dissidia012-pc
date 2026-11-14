#include "main/auto_region2.h"

/* The low 28 bits of the object's field_08 (0 without an object). */
unsigned int func_088716C8(void *self, Obj8716C8 *object)
{

    if (object == 0) {
        return 0;
    } else {
        return object->field_08 & 0xfffffff;
    }
}
