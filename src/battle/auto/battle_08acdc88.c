#include "battle/auto_region4.h"

typedef struct StateCDC88 {
    char pad_00[8];
    char field_08;
} StateCDC88;

void func_08ACD9F8(StateCDC88 *self);

/* Calls func_08ACD9F8, then clears field_08. */
void func_08ACDC88(StateCDC88 *self)
{
    func_08ACD9F8(self);
    self->field_08 = 0;
}
