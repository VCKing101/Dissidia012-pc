typedef struct Agent4Task {
    unsigned char unknown_0000[0x20];
    union {
        unsigned int flags;
        struct {
            unsigned int active : 1;
        } bits;
    } state;
} Agent4Task;

int func_08822D80(Agent4Task *self);
void func_08823410(Agent4Task *self);

int func_088223F8(Agent4Task *self) {
    if (!self->state.bits.active) {
        return -1;
    }
    if (func_08822D80(self) == 1) {
        return 0;
    }
    func_08823410(self);
    return 0;
}
