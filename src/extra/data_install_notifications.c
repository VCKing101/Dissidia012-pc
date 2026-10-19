#include "extra/data_install.h"

/* These callbacks publish a value followed by a byte flag. Preserve volatile
 * accesses; the field meanings remain provisional until callers are traced. */
void func_089DDE10(u32 unused, u32 value, void *userdata) {
    volatile DataInstallState *state = (volatile DataInstallState *)userdata;
    state->field_4594 = value;
    state->flag_459D = 1;
}

void func_089DDE20(u32 unused, u32 value, void *userdata) {
    volatile DataInstallState *state = (volatile DataInstallState *)userdata;
    state->field_4590 = value;
    state->flag_459C = 1;
}
