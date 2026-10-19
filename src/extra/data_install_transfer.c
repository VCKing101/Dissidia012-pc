#include "extra/data_install.h"

extern u64 D_089DE788;
extern u32 D_089D5FB4;
extern u32 func_886FF7C(u64 offset, u32 buffer, u32 size);
extern void func_089DE6F4(u32 value, void *userdata);
extern u32 func_8870000(u32 context, u32 entry, u64 offset, u32 buffer,
                      u32 size, void (*callback)(u32, void *), void *userdata);

u8 func_089DDF24(DataInstallOwner *owner) {
    owner->state->flag_459E = 0;
    if (owner->state->flag_459C && owner->state->field_4590 == 0) {
        owner->state->field_4598 = 0x80000014U;
        owner->state->flag_459E = 1;
    }
    return owner->state->flag_459C;
}

u32 func_089DDF6C(DataInstallOwner *owner) {
    owner->state->flag_459E = 0;
    owner->state->field_4598 = func_886FF7C(D_089DE788, owner->state->buffer_address,
                                         owner->state->field_4590);
    if ((s32)owner->state->field_4598 < 0) {
        owner->state->flag_459E = 1;
    }
    owner->state->transfer_offset += (owner->state->field_4598 + 0x1FFF) & ~0x1FFFU;
    return owner->state->field_4598;
}

u32 func_089DE020(DataInstallOwner *owner) {
    owner->state->flag_459E = 0;
    owner->state->flag_459D = 0;
    owner->state->field_4594 = 0;
    owner->state->field_4598 = func_8870000(D_089D5FB4,
        owner->state->entries[owner->state->current_entry_index], owner->state->transfer_offset,
        owner->state->buffer_address, owner->state->field_4590,
        func_089DE6F4, owner->state);
    if ((s32)owner->state->field_4598 < 0) {
        owner->state->flag_459E = 1;
    }
    return owner->state->field_4598;
}

u32 func_089DE0B8(DataInstallOwner *owner) {
    owner->state->flag_459E = 0;
    if (owner->state->flag_459D) {
        owner->state->field_4598 = owner->state->field_4594;
        if (owner->state->field_4594 == 0) {
            owner->state->field_4598 = 0x80000015U;
        }
        if ((s32)owner->state->field_4598 < 0) {
            owner->state->flag_459E = 1;
        }
        owner->state->transfer_offset += (owner->state->field_4594 + 0x1FFF) & ~0x1FFFU;
        owner->state->current_entry_index++;
        return 1;
    }
    return 0;
}

void func_089DE164(DataInstallOwner *owner, s32 index, u32 value) {
    if (index < 0 || index >= 4) {
        return;
    }
    owner->state->field_4580[index] = value;
}
