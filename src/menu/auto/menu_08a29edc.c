#include "menu/auto_region1.h"

/* ScreenE75C8: plays motion 0x19 on field_448 (direction < 0) or 0x1C on
 * field_44C. */
void func_08A29EDC(ScreenE75C8 *self, int direction)
{
    if (direction < 0) {
        func_088D9F5C(self->work->field_448, 0x19, 1);
    } else {
        func_088D9F5C(self->work->field_44C, 0x1C, 1);
    }
}
