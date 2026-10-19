#include "extra/data_install.h"

extern u32 func_89A5490(u64 *capacity_kib, u64 *available_kib);
extern u32 D_089D5FB4;
extern u32 D_089DE798;
extern u32 func_886F378(u32 context, u32 path);

/* Address names are provisional until callers establish their semantics. */
u32 func_089DD5F8(u32 unused0, u32 unused1, u32 flags, void *userdata) {
    volatile DataInstallState *state = (volatile DataInstallState *)userdata;
    if (flags & DATA_INSTALL_POWER_SUSPENDING) {
        state->flag_45A1 = 1;
    }
    if (flags & DATA_INSTALL_POWER_RESUME_COMPLETE) {
        state->flag_45A1 = 0;
    }
    return 0;
}

u32 func_089DD62C(u32 unused0, u32 unused1, u32 event, void *userdata) {
    volatile DataInstallState *state = (volatile DataInstallState *)userdata;
    if (event == 2) {
        state->flag_459F = 1;
    }
    return 0;
}

void func_089DD648(DataInstallOwner *owner, u32 first, u32 second) {
    owner->state->buffer_address = first;
    owner->state->buffer_capacity_bytes = second;
}

void func_089DD65C(DataInstallOwner *owner, u32 first, u32 second) {
    owner->state->field_4578 = first;
    owner->state->field_457C = second;
}

u64 func_089DD670(DataInstallOwner *owner) {
    owner->state->flag_459E = 0;
    owner->state->field_4598 = func_89A5490(&owner->state->capacity_kib,
                                        &owner->state->available_kib);
    if (owner->state->field_4598 != 0) {
        owner->state->flag_459E = 1;
    }
    return owner->state->capacity_kib;
}

u32 func_089DD6D4(DataInstallOwner *owner) {
    owner->state->flag_459E = 0;
    owner->state->field_4598 = func_886F378(D_089D5FB4, D_089DE798);
    if (owner->state->field_4598 == 0) {
        return 1;
    }
    if (owner->state->field_4598 != DATA_INSTALL_ERROR_NOT_FOUND) {
        owner->state->flag_459E = 1;
    }
    return 0;
}

