#include "battle/auto_region4.h"

typedef struct WaitC0A00 {
    char pad_00[0x14];
    unsigned char field_14;
    char pad_15[0x2B];
    void *field_40;
} WaitC0A00;

int func_08ABE588(void *object);

/* While field_14 is 1, clears it once func_08ABE588 reports 0 for the
 * object at field_40. */
void func_08AC0A00(WaitC0A00 *self)
{
    if (self->field_14 == 1 && func_08ABE588(self->field_40) == 0) {
        self->field_14 = 0;
    }
}
