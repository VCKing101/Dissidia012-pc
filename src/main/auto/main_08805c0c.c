#include "main/auto_region1.h"

/* Makes self the only listed chara with bit 0x100 of flags_580 set. */
void func_08805C0C(Chara *self)
{
    Chara *chara;

    chara = 0;
    while (chara = func_08842048(chara), chara != 0) {
        chara->flags_580 = chara->flags_580 & ~0x100u;
    }
    self->flags_580 = self->flags_580 | 0x100;
}
