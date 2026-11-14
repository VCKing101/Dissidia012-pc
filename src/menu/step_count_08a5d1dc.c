typedef struct A6Step {
    char pad_00[0x18];
    int field_18;
    char pad_1C[0xC];
    int field_28;
} A6Step;

extern int D_08A4FFF4;
void func_08A4DE34(int obj);

void func_08A5D1DC(A6Step *self) {
    if (self->field_18 > 1) {
        func_08A4DE34(D_08A4FFF4);
        self->field_28 = 0;
    } else {
        self->field_18 = self->field_18 + 1;
    }
}
