typedef struct A6Sum {
    char pad_00[0xD0];
    int field_D0;
    char pad_D4[0x28];
    int field_FC;
} A6Sum;

extern int D_08A61DD0[];

int func_08A61810(A6Sum *self) {
    int i = 0;
    int sum = 0;
    for (i = 0; i < self->field_D0; i++) {
        sum += D_08A61DD0[i];
    }
    sum += self->field_FC;
    int max = 11;
    if (sum < 0 || max < sum) {
        sum = 0;
    }
    return sum;
}
