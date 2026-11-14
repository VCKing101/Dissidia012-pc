typedef struct Owner088898C8 {
    unsigned char unknown_000[0x744];
    int values[3];
    unsigned char enabled[3];
} Owner088898C8;

void func_088898C8(Owner088898C8 *self) {
    int i;
    for (i = 0; i < 3; i++) {
        self->values[i] = 0;
        *((unsigned char *)self + 0x750 + i) = 1;
    }
}
