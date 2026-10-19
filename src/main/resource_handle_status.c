#include "main/resource_descriptor.h"

/* A sentinel handle reports 1 directly; otherwise preserve the native result. */
int func_0889F980(MainResourceHandlePrefix *self) {
    if (self->field_0000 != -1) return func_088A159C(D_089D6C88, self->field_0000);
    return 1;
}
