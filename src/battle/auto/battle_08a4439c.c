#include "battle/auto_region3.h"

typedef struct Handles4439C {
    char pad_00[0x48];
    void *field_48[2];
    void *field_50[14];
} Handles4439C;

typedef struct Owner4439C {
    char pad_00[0xC];
    int field_0C;
    char pad_10[4];
    Handles4439C *field_14;
} Owner4439C;

void func_0887BA5C(void *handle);

/* Frees the two handles at field_48 of the table at field_14, then, when
 * field_0C is set, the fourteen at field_50. */
void func_08A4439C(Owner4439C *self)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (self->field_14->field_48[i] != 0) {
            func_0887BA5C(self->field_14->field_48[i]);
            self->field_14->field_48[i] = 0;
        }
    }
    if (self->field_0C != 0) {
        for (i = 0; i < 14; i++) {
            if (self->field_14->field_50[i] != 0) {
                func_088DA060(self->field_14->field_50[i]);
                self->field_14->field_50[i] = 0;
            }
        }
    }
}
