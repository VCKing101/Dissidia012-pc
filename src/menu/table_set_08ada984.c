typedef struct TableCtx {
    char pad_000[0x400];
    char field_400[1];
    char pad_401[0x1b98 - 0x401];
    int field_1b98;
} TableCtx;

typedef struct TableOwner {
    char pad_000[0x46AC];
    TableCtx *field_46AC;
} TableOwner;

extern void func_088D3AE8(void *a, int b, int c, int d);
extern unsigned short D_08ADFAA6[];

void func_08ADA984(TableOwner *self)
{
    func_088D3AE8(self->field_46AC->field_400, 0x22, D_08ADFAA6[self->field_46AC->field_1b98], 0x1F);
}
