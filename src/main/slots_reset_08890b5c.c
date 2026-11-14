typedef struct Owner08890B5C {
    unsigned char field_00;
    unsigned char unknown_01[3];
    int field_04;
    unsigned char field_08;
    unsigned char unknown_09[3];
    int pair[2];
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
} Owner08890B5C;

void func_08890B5C(Owner08890B5C *self) {
    int none = -1;
    int i;
    self->field_00 = 0;
    self->field_04 = none;
    self->field_08 = 0;
    for (i = 0; i < 2; i++) {
        self->pair[i] = none;
    }
    self->field_14 = none;
    self->field_18 = none;
    self->field_1C = none;
    self->field_20 = none;
}
