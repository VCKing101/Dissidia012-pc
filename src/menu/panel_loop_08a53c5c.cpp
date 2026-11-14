struct A6Screen2 {
    char pad_00[0xE0];
    char field_E0[2][0x88];
};

extern "C" {
void func_08A52E90(char *p);

void func_08A53C5C(A6Screen2 *self) {
    for (int i = 0; i < 2; i++) {
        func_08A52E90(self->field_E0[i]);
    }
}
}
