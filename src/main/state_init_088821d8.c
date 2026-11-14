typedef struct State088821D8 {
    unsigned char unknown_0000[0x2000];
    int field_2000;
    int field_2004;
    int field_2008;
} State088821D8;

State088821D8 *func_088821D8(State088821D8 *self) {
    int one = 1;
    self->field_2000 = 0;
    self->field_2004 = one;
    self->field_2008 = one;
    return self;
}
