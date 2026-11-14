struct Agent4Task22D08 {
    unsigned char unknown_0000[0x20];
    union {
        unsigned int flags;
        struct {
            unsigned int active : 1;
        } bits;
    } state;
    unsigned char unknown_0024[0xD];
    unsigned char field_31;
};

extern "C" {
int func_08822D08(Agent4Task22D08 *self, int value) {
    if (!self->state.bits.active) {
        return -1;
    }
    self->field_31 = value;
    return 0;
}

bool func_08822D30(Agent4Task22D08 *self) {
    unsigned int flags = self->state.flags;
    if (!(flags & 1)) {
        return false;
    }
    return (flags & 4) != 0;
}

bool func_08822D58(Agent4Task22D08 *self) {
    unsigned int flags = self->state.flags;
    if (!(flags & 1)) {
        return false;
    }
    return (flags & 8) != 0;
}

bool func_08822D80(Agent4Task22D08 *self) {
    unsigned int flags = self->state.flags;
    if (!(flags & 1)) {
        return false;
    }
    return (flags & 0x10) != 0;
}
}
