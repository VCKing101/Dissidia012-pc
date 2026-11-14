struct Owner08896BB8 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int ids[9];

    static Owner08896BB8 *init(Owner08896BB8 *self) {
        self->field_00 = 0;
        self->field_04 = 0;
        self->field_08 = 0;
        self->field_0C = -1;
        for (int i = 0; i < 9; i++) {
            self->ids[i] = -1;
        }
        return self;
    }
};

extern "C" {
Owner08896BB8 *func_08896BB8(Owner08896BB8 *self) {
    return Owner08896BB8::init(self);
}
}
