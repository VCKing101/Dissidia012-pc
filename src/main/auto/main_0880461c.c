#include "main/auto_region1.h"

/* Calls func_08804498 in mode 1, or in mode 2 (clearing bit 0 of flags_384)
 * while field_BC is positive. */
void func_0880461C(void *self, Chara *chara)
{
    int mode;

    mode = 1;
    if (0.0f < chara->field_BC) {
        mode = 2;
        chara->flags_384 = chara->flags_384 & ~1u;
    }
    func_08804498(self, chara, mode);
}
