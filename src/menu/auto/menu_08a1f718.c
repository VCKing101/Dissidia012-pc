#include "menu/auto_region1.h"

/* Menu root: address of field_140's field_2C90, 0 without field_140. */
int *func_08A1F718(MenuRoot *self)
{
    int *result;

    result = 0;
    if (self->field_140 != 0) {
        result = &self->field_140->field_2C90;
    }
    return result;
}
