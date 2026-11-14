#include "battle/auto_region4.h"

typedef struct TripleF3B90 {
    int field_00;
    int field_04;
    int field_08;
} TripleF3B90;

/* Constructor of a three-word object: clears it. */
TripleF3B90 *func_08AF3B90(TripleF3B90 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    return self;
}
