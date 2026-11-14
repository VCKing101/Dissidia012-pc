#include "battle/auto_region3.h"

void *func_088D0700(void *loader, char arg1, int id, int slot);

/* Loads the models at field_220 (0x246) and field_224 (0x248) when they are
 * missing. */
void func_08A3E874(Model3E874 *self)
{
    void *model;

    if (self->field_220 == 0) {
        model = func_088D0700(D_089D7830, 1, 0x246, 0x16);
        self->field_220 = model;
    }
    if (self->field_224 == 0) {
        model = func_088D0700(D_089D7830, 1, 0x248, 0x17);
        self->field_224 = model;
    }
}
