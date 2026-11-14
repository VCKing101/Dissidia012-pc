#include "menu/auto_region1.h"

/* ObjEE850: calls func_089EEC64, then reports bit 0x20 of the sixth model. */
unsigned char func_089EEC30(ObjEE850 *self)
{
    func_089EEC64();
    return (self->models[5]->field_20 & 0x20) != 0;
}
