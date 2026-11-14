#include "battle/auto_region4.h"

typedef struct PartDFA94 {
    unsigned char field_00[3][0x18];    /* built by func_08ADF428 */
    unsigned char field_48[0x10];
    int field_58;
    int field_5C;
} PartDFA94;

void *func_08ADF428(void *object);

/* Constructor: builds the three 0x18-byte objects at field_00 and clears
 * the rest. */
PartDFA94 *func_08ADFA94(PartDFA94 *self)
{
    func_08993560(self->field_00, 3, 0x18, func_08ADF428);
    self->field_58 = 0;
    self->field_5C = 0;
    func_08935828(self->field_48, 0, 0x10);
    return self;
}
