#include "menu/auto_region2.h"

/* Item583C0: copies out its three bytes. */
void func_08A566B8(Item583C0 *self, char *out1, char *out2, char *out3)
{
    *out1 = self->field_238;
    *out2 = self->field_239;
    *out3 = self->field_23A;
}
