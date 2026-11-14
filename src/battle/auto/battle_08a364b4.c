#include "battle/auto_region3.h"

typedef struct Handles364B4 {
    char pad_00[0x88];
    void *handles[8];
} Handles364B4;

typedef struct Owner364B4 {
    char pad_00[0xC];
    int field_0C;
    char pad_10[4];
    Handles364B4 *field_14;
} Owner364B4;

/* Frees the 8 handles of the table at field_14 when field_0C is set. */
void func_08A364B4(Owner364B4 *self)
{
    int i;

    if (self->field_0C != 0) {
        for (i = 0; i < 8; i++) {
            if (self->field_14->handles[i] != 0) {
                func_088DA060(self->field_14->handles[i]);
                self->field_14->handles[i] = 0;
            }
        }
    }
}
