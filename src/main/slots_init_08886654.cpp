extern "C" {
void func_0885EAFC(void *p);
void func_0888F908(void *p);
void func_08935828(void *dst, int value, int size);
}

struct Slot08886654 {
    int field_00;
    int field_04;
};

struct Owner08886654 {
    Slot08886654 slots[10];
    int field_50;
    int field_54;
    unsigned char field_58;
    unsigned char field_59;
    unsigned char unknown_5A[2];
    unsigned char field_5C[0x18];
    unsigned char field_74[0x40];
    int field_B4;
    int field_B8;

    static Owner08886654 *init(Owner08886654 *self) {
        self->field_50 = 0;
        self->field_54 = 0;
        self->field_58 = 0;
        self->field_59 = 0;
        func_0885EAFC(self->field_5C);
        func_0888F908(self->field_74);
        self->field_B4 = 0;
        self->field_B8 = -1;
        func_08935828(&self->slots[0], 0, 8);
        func_08935828(&self->slots[1], 0, 8);
        func_08935828(&self->slots[2], 0, 8);
        func_08935828(&self->slots[3], 0, 8);
        func_08935828(&self->slots[4], 0, 8);
        func_08935828(&self->slots[5], 0, 8);
        func_08935828(&self->slots[6], 0, 8);
        func_08935828(&self->slots[7], 0, 8);
        func_08935828(&self->slots[8], 0, 8);
        func_08935828(&self->slots[9], 0, 8);
        return self;
    }
};



extern "C" {
Owner08886654 *func_08886654(Owner08886654 *self) {
    return Owner08886654::init(self);
}
}
