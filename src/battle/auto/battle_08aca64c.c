#include "battle/auto_region4.h"

typedef struct PhaseCA64C {
    char pad_00[4];
    int phase;
    char pad_08[0x14];
    char field_1C;
} PhaseCA64C;

/* Sets field_1C once the phase is 5. */
void func_08ACA64C(PhaseCA64C *self)
{
    if (self->phase == 5) {
        self->field_1C = 1;
    }
}
