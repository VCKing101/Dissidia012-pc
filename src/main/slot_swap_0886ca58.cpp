struct Owner0886CA58 {
    unsigned char unknown_000[0x94];
    int current[2];
    int field_09C[2];
    unsigned char unknown_0A4[0x7C];
    int previous[2];
    int state;
};

extern "C" {
void func_0886CA58(Owner0886CA58 *self) {
    if (self->state != 0) {
        return;
    }
    self->state = 2;
    for (int i = 0; i < 2; i++) {
        int value = self->current[i];
        self->current[i] = 0;
        self->previous[i] = value;
        self->field_09C[i] = 0;
    }
}
}
