#include "battle/auto_region4.h"

typedef struct SetupEB9F4 {
    char pad_00[8];
    int field_08;
    int field_0C;
    char field_10;
    char field_11;
} SetupEB9F4;

/* Stores `value`, the result of func_088BB708 for `id`, and clears the two
 * flags. */
void func_08AEB9F4(SetupEB9F4 *self, int id, int value)
{
    int result;

    self->field_08 = value;
    result = func_088BB708(id);
    self->field_0C = result;
    self->field_10 = 0;
    self->field_11 = 0;
}
