#include "battle/auto_region4.h"

/* PartF2CBC: once field_71 and field_73 are set, calls func_088EC67C, sets
 * field_5C to 3 and, in mode 5, calls func_08AF614C on field_24. */
void func_08AF324C(PartF2CBC *self)
{
    if (self->field_71 == 1 && self->field_73 == 1) {
        func_088EC67C(self->field_04);
        self->field_5C = 3;
        if (self->field_64 == 5 && self->field_24 != 0) {
            func_08AF614C(self->field_24, 0x1C20);
        }
    }
}
