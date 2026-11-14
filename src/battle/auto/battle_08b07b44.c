#include "battle/auto_region5.h"

/* Part06984: resets the selection (func_08B0825C), notes whether D_08B05258
 * is in mode 2, and runs func_08B051DC on it. */
void func_08B07B44(Part06984 *self)
{
    Part05240 *manager;

    func_08B0825C(self);
    manager = D_08B05258;
    self->field_3888 = func_08B05008(D_08B05258) == 2;
    func_08B051DC(manager);
}
