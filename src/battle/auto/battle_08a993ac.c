#include "battle/auto_region4.h"

/* Part9A1A0: clears field_02 of every node on the list at field_64, then
 * empties the list fields. */
void func_08A993AC(Part9A1A0 *self)
{
    Node9A1A0 *node;

    for (node = self->field_64; node != 0; node = node->next) {
        node->field_02 = 0;
    }
    self->field_5C = 0;
    self->field_58 = 0;
    self->field_60 = 0;
    self->field_68 = 0;
    self->field_64 = 0;
    self->field_6C = 0;
}
