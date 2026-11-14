typedef struct Agent4Guarded {
    unsigned char unknown_0000[0x374];
    int field_0374;
    unsigned char unknown_0378[0x1D8];
    unsigned char field_0550[0x10];
    unsigned int flags;
} Agent4Guarded;

void func_089A3868(void *slot, int value);

int func_08815C78(Agent4Guarded *self, int value) {
    if (!(self->flags & 1)) {
        return -1;
    }
    self->field_0374 = value;
    return 0;
}

int func_08815CA0(Agent4Guarded *self, int value) {
    if (!(self->flags & 1)) {
        return -1;
    }
    func_089A3868(self->field_0550, value);
    return 0;
}
