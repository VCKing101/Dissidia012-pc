#include "menu/agent6_types.h"

/* Three optional parts at +0x30; each present one gets its eighth virtual. */
typedef struct PartTriple {
    char pad_00[0x30];
    A6Part *field_30[3];
} PartTriple;

extern "C" {
void func_089EA994(PartTriple *self)
{
    for (int i = 0; i < 3; i++) {
        if (self->field_30[i] != 0) {
            self->field_30[i]->v7();
        }
    }
}
}
