#include "main/auto_region1.h"

/* Sets bit 7 of flags_580, first taking bit 8 from every other chara when
 * exclusive. */
void func_08806B58(Chara *self, unsigned char exclusive)
{
    if (exclusive != 0) {
        func_08805C0C(self);
    }
    self->flags_580 = self->flags_580 | 0x80;
}
