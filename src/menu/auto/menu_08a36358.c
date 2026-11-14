#include "menu/auto_region1.h"

/* Screen38EA8: lays out rows, first adding two fixed rows unless field_DB5
 * is clear or force is set. */
void func_08A36358(Screen38EA8 *self, unsigned short *rows, int force)
{
    if ((self->work->field_DB5 == 0) || (force != 0)) {
        func_08A41454(D_08A457EC, rows);
    } else {
        rows[0] = 0x16;
        rows[1] = 0xC;
        rows[2] = 9;
        rows[3] = 3;
        rows[4] = 0x15;
        rows[5] = 0xC;
        rows[6] = 9;
        rows[7] = 4;
        func_08A41454(D_08A457EC, rows + 8);
    }
}
