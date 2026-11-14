#include "menu/auto_region1.h"

/* ScreenE75C8: plays motion 0x22 on field_468 (direction < 0) or 0x1F on
 * field_46C. */
void func_08A2A000(ScreenE75C8 *self, int direction)
{
    if (direction < 0) {
        func_088D9F5C(self->work->field_468, 0x22, 1);
    } else {
        func_088D9F5C(self->work->field_46C, 0x1F, 1);
    }
}
