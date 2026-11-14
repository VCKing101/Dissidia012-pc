#include "battle/auto_region3.h"

typedef struct Handles35460 {
    char pad_00[0x28];
    void *handles[7];
} Handles35460;

typedef struct Owner35460 {
    char pad_00[0xC];
    int field_0C;
    char pad_10[4];
    Handles35460 *field_14;
} Owner35460;

/* Frees the 7 handles of the table at field_14 when field_0C is set. */
void func_08A35460(Owner35460 *self)
{
    int i;

    if (self->field_0C != 0) {
        for (i = 0; i < 7; i++) {
            if (self->field_14->handles[i] != 0) {
                func_088DA060(self->field_14->handles[i]);
                self->field_14->handles[i] = 0;
            }
        }
    }
}
