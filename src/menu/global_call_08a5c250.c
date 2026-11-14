typedef struct A6Args {
    char pad_00[4];
    unsigned char field_04;
    char pad_05[3];
    int field_08;
} A6Args;

extern int D_08A4FFF4;
void func_08A4E3A0(int obj, int a, int b);

void func_08A5C250(A6Args *self) {
    func_08A4E3A0(D_08A4FFF4, self->field_08, self->field_04);
}
