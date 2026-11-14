typedef struct TableCtx {
    char pad_000[0x400];
    char field_400[1];
    char pad_401[0xF28 - 0x401];
    int field_F28;
} TableCtx;

typedef struct TableOwner {
    char pad_000[0xDB30];
    TableCtx *field_DB30;
} TableOwner;

extern void func_088D3AE8(void *a, int b, int c, int d);
extern unsigned short D_08ADA26C[];

void func_08ACA640(TableOwner *self)
{
    func_088D3AE8(self->field_DB30->field_400, 0x46, D_08ADA26C[self->field_DB30->field_F28], 0x1F);
}
