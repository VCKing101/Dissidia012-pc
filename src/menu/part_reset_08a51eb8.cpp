/* Resets the part, then runs its virtual steps 9, 8, 7 and 0. */
#include "menu/agent12_types.h"

extern "C" void func_08A5224C(A12Part24 *self);

extern "C" {
void func_08A51EB8(A12Part24 *self)
{
    func_08A5224C(self);
    self->v9();
    self->v8();
    self->v7();
    self->v0();
}
}
