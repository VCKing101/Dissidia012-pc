#include "menu/auto_region2.h"

extern const short D_08A5FA68[];

/* Panel5FA00: plays its layout script and, with handle 0xB8, func_08A5F970. */
void func_08A5F364(Panel5FA00 *self)
{
    func_088D3F00(0, self->handles, D_08A5FA68);
    if (self->handles[0xB8] != 0) {
        func_08A5F970(self, 1, 0xB8, self->field_380);
    }
}
