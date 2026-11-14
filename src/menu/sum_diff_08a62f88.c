typedef struct A6Sum2 {
    char pad_000[0xD90];
    int field_D90;
} A6Sum2;

extern int D_08A63708[];
int func_08A62FE4(A6Sum2 *self);

int func_08A62F88(A6Sum2 *self) {
    int i;
    int sum = 0;
    for (i = 0; i < self->field_D90; i++) {
        sum += D_08A63708[i];
    }
    return func_08A62FE4(self) - sum;
}
