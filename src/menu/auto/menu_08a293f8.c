#include "menu/auto_region1.h"

/* DelegateBoard: calls the board's func_08A292EC. */
void func_08A293F8(DelegateBoard *self)
{
    func_08A292EC(self->owner);
}
