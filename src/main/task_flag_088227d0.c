typedef struct Agent4Task {
    unsigned char unknown_0000[0x20];
    union {
        unsigned int flags;
        struct {
            unsigned int active : 1;
        } bits;
    } state;
} Agent4Task;

int func_088227D0(Agent4Task *self) {
    if (!self->state.bits.active) {
        return -1;
    }
    self->state.flags |= 4;
    return 0;
}
