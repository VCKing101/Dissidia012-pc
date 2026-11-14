typedef struct Agent4Bits {
    unsigned int enabled : 1;
    unsigned int value : 16;
    unsigned int mode : 2;
    unsigned int unused : 13;
    float field_04;
} Agent4Bits;

void func_08819F00(Agent4Bits *self);

int func_08819F1C(Agent4Bits *self, int value) {
    func_08819F00(self);
    if (value < 0) {
        return -1;
    }
    self->value = value;
    self->enabled = 1;
    return 0;
}
