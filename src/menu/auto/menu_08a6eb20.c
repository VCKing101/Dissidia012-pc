#include "menu/auto_region2.h"

/* Obj6EB20 constructor: builds its ten elements. */
Obj6EB20 *func_08A6EB20(Obj6EB20 *self)
{
    func_08993560(self->elems, 10, sizeof(Elem6EAF8), func_08A6EAF8);
    return self;
}
