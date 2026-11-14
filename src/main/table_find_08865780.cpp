#include "main/agent2_types.h"

extern "C" int func_08935970(void *, void *);

extern "C" int func_08865780(Agent2Table *self, void *key) {
    for (int index = 0; index < self->field_0018; index++) {
        if (!func_08935970(self->field_0014[index], key)) return index;
    }
    return -1;
}
