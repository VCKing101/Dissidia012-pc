#include "extra/data_install.h"

/* Import from scesupPreAcc, NID 0xA0EAF444. Its public name is unknown. */
extern u32 func_89C5D40(void);
/* Import from scesupPreAcc, NID 0x110E318B. Argument meanings unknown. */
extern u32 func_89C5D10(u32 first, u32 second, u32 third, u32 fourth);
extern u64 D_089DE788;
extern u32 D_089D5FC0;
extern u32 D_089D5FC4;
extern u32 D_089D5FB4;
extern u32 D_089DE798;
extern u32 func_886F3C0(u32 context, u32 path);
extern void func_886FA1C(u32 context);
/* Import from scesupPreAcc, NID 0x2EC3F4D9. */
extern u32 func_89C5D28(u32 path);
extern void func_089DD99C(DataInstallOwner *owner, u32 argument);
extern u32 func_886FA84(u32 context);
extern u32 func_886FA78(u32 context);
extern void func_886F5D4(u32 context, u32 buffer);
/* Byte-fill and byte-copy helpers return the destination address. */
extern u32 func_8935828(u32 destination, u32 value, u32 size);
extern u32 func_8935524(u32 destination, u32 source, u32 size);

void func_089DDB88(DataInstallOwner *owner, u32 argument) {
    owner->state->flag_459E = 0;
    owner->state->transfer_offset = D_089DE788;
    owner->state->field_4598 = func_89C5D28(D_089DE798);
    if (owner->state->field_4598 != 0) {
        owner->state->flag_459E = 1;
    } else {
        func_089DD99C(owner, argument);
    }
}

u32 func_089DDC10(DataInstallOwner *owner) {
    u32 status;
    owner->state->flag_459E = 0;
    owner->state->transfer_offset = D_089DE788;
    status = func_89C5D10(D_089D5FC0, D_089D5FC4,
                         owner->state->field_4578, owner->state->field_457C);
    owner->state->field_4598 = status;
    if (owner->state->field_4598 != 0) {
        owner->state->flag_459E = 1;
    }
    return status;
}

u32 func_089DDC98(DataInstallOwner *owner) {
    u32 status;
    owner->state->flag_459E = 0;
    status = func_89C5D40();
    owner->state->field_4598 = status;
    if (owner->state->field_4598 != 0) {
        owner->state->flag_459E = 1;
    }
    return status;
}

void func_089DDCE4(DataInstallOwner *owner) {
    owner->state->flag_459E = 0;
    owner->state->field_4598 = func_886F3C0(D_089D5FB4, D_089DE798);
    if (owner->state->field_4598 == 0) {
        func_886FA1C(D_089D5FB4);
    } else if (owner->state->field_4598 != DATA_INSTALL_ERROR_NOT_FOUND) {
        owner->state->flag_459E = 1;
    }
}

void func_089DDD68(DataInstallOwner *owner, u8 clear) {
    u32 buffer;
    owner->state->flag_459E = 0;
    owner->state->field_4590 = func_886FA84(D_089D5FB4);
    buffer = owner->state->buffer_address;
    if (clear) {
        func_8935828(buffer, 0, owner->state->field_4590);
    } else {
        u32 source = func_886FA78(D_089D5FB4);
        func_8935524(buffer, source, owner->state->field_4590);
        func_886F5D4(D_089D5FB4, owner->state->buffer_address);
    }
}
