#include "battle/auto_region4.h"

typedef struct PhaseCAA9C {
    char pad_00[4];
    int phase;
} PhaseCAA9C;

/* Whether the phase is 7. */
unsigned char func_08ACAA9C(PhaseCAA9C *self)
{
    return self->phase == 7;
}
