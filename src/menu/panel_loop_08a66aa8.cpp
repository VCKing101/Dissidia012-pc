struct A6Block {
    char data[0x8A4];
};

struct A6Blocks {
    char pad_00[0x188];
    A6Block field_188[2];
};

extern "C" {
void func_08A63C74(A6Block *b);

void func_08A66AA8(A6Blocks *self) {
    for (int i = 0; i < 2; i++) {
        func_08A63C74(&self->field_188[i]);
    }
}
}
