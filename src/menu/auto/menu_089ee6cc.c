#include "menu/auto_region1.h"

/* ObjEE228: calls func_089EE700, then reports bit 0x20 of the third model. */
unsigned char func_089EE6CC(ObjEE228 *self)
{
    func_089EE700();
    return (self->models[2]->field_20 & 0x20) != 0;
}
