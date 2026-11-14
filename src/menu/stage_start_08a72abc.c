typedef struct A6Stage {
    char pad_00[0xC];
    char field_0C[4];
    char field_10[0xC];
    int field_1C;
    char pad_20[4];
    unsigned char field_24;
} A6Stage;

void func_088D2F68(char *p, int v);
void func_088D2FA0(char *p, int v);

void func_08A72ABC(A6Stage *self) {
    char *a = self->field_0C;
    int one;
    func_088D2F68(a, 0x1F);
    func_088D2FA0(a, 0x22);
    one = 1;
    if (self->field_24 == one) {
        char *b = self->field_10;
        func_088D2F68(b, 3);
        func_088D2FA0(b, 4);
    }
    self->field_1C = one;
}
