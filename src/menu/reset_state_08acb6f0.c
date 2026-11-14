typedef struct ResetCtx {
    char pad_0000[0x400];
    char field_400[1];
    char pad_401[0xF80 - 0x401];
    int field_F80;
    char pad_F84[0xF8C - 0xF84];
    int field_F8C;
    char pad_F90[0x13F8 - 0xF90];
    int field_13F8;
    char pad_13FC[0x1400 - 0x13FC];
    unsigned short field_1400;
} ResetCtx;

typedef struct ResetOwner {
    char pad_000[0xDB30];
    ResetCtx *field_DB30;
} ResetOwner;

extern void func_088D3CF0(ResetCtx *a, void *b, unsigned short *table);
extern void func_08AD1DA8(ResetOwner *self, char flag);
extern void func_08AD4870();
extern unsigned short D_08AD993C;

void func_08ACB6F0(ResetOwner *self)
{
    func_08AD4870();
    self->field_DB30->field_F80 = 0;
    self->field_DB30->field_F8C = 0;
    func_08AD1DA8(self, 0);
    func_088D3CF0(self->field_DB30, self->field_DB30->field_400, &D_08AD993C);
    self->field_DB30->field_13F8 = 0;
    self->field_DB30->field_1400 = 0xFFFF;
}
