typedef struct TableCtx {
    char pad_000[0x400];
    char field_400[1];
    char pad_401[0x884 - 0x401];
    int field_884;
} TableCtx;

typedef struct TableOwner {
    char pad_000[0x14C];
    TableCtx *field_14C;
} TableOwner;

extern void func_088D3AE8(void *a, int b, int c, int d);
extern unsigned short D_08AC4416[];

void func_08ABD1F4(TableOwner *self)
{
    func_088D3AE8(self->field_14C->field_400, 0x33, D_08AC4416[self->field_14C->field_884], 0x35);
}
