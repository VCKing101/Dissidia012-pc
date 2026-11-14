#include "battle/auto_region4.h"

typedef struct WatchDC5C0 {
    char pad_00[0xD0];
    int field_D0;
} WatchDC5C0;

void func_08ADC770(WatchDC5C0 *self, int *value, int kind);

/* Calls func_08ADC770 (kind 4) when field_D0 equals *value and the global
 * field_26C is 0, then copies field_26C into field_D0. */
void func_08ADC5C0(WatchDC5C0 *self, int *value)
{
    int current;

    current = ((Global405E0 *)D_08A405E0)->field_26C;
    if (self->field_D0 == *value && current == 0) {
        func_08ADC770(self, value, 4);
    }
    self->field_D0 = current;
}
