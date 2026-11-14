typedef struct A6Sum3 {
    char pad_000[0xD34];
    int field_D34;
    char pad_D38[0x28];
    int field_D60;
} A6Sum3;

extern int D_08A636D8[];

int func_08A62FE4(A6Sum3 *self) {
    int i;
    int sum = 0;
    for (i = 0; i < self->field_D34; i++) {
        sum += D_08A636D8[i];
    }
    return sum + self->field_D60;
}
