#include "battle/auto_region3.h"

typedef struct Handles3CF70 {
    char pad_00[0x20];
    void *handles[13];
} Handles3CF70;

typedef struct Owner3CF70 {
    char pad_00[0xC];
    int field_0C;
    char pad_10[4];
    Handles3CF70 *field_14;
} Owner3CF70;

/* Frees the 13 handles of the table at field_14 when field_0C is set. */
void func_08A3CF70(Owner3CF70 *self)
{
    int i;

    if (self->field_0C != 0) {
        for (i = 0; i < 13; i++) {
            if (self->field_14->handles[i] != 0) {
                func_088DA060(self->field_14->handles[i]);
                self->field_14->handles[i] = 0;
            }
        }
    }
}
