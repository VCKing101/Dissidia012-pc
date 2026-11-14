typedef struct Agent4Guarded {
    unsigned char unknown_0000[0x560];
    union {
        unsigned int flags;
        struct {
            unsigned int active : 1;
        } bits;
    } state;
} Agent4Guarded;

void func_088160D8(Agent4Guarded *self);

int func_08815D90(Agent4Guarded *self) {
    if (!self->state.bits.active) {
        return -1;
    }
    self->state.flags &= ~4;
    func_088160D8(self);
    return 0;
}

int func_08815DD4(Agent4Guarded *self) {
    if (!self->state.bits.active) {
        return -1;
    }
    self->state.flags |= 4;
    func_088160D8(self);
    return 0;
}
