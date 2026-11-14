#include "battle/auto_region3.h"

typedef struct Handles37988 {
    char pad_00[0xAC];
    void *handles[15];
} Handles37988;

typedef struct Owner37988 {
    char pad_00[0xC];
    int field_0C;
    char pad_10[4];
    Handles37988 *field_14;
} Owner37988;

/* Frees the 15 handles of the table at field_14 when field_0C is set. */
void func_08A37988(Owner37988 *self)
{
    int i;

    if (self->field_0C != 0) {
        for (i = 0; i < 15; i++) {
            if (self->field_14->handles[i] != 0) {
                func_088DA060(self->field_14->handles[i]);
                self->field_14->handles[i] = 0;
            }
        }
    }
}
