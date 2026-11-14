typedef struct CursorPair {
    short field_00;
    short field_02;
    int field_04;
} CursorPair;

typedef struct CursorMarks {
    char pad_00000[0x26D70];
    CursorPair field_26D70[1];
} CursorMarks;

typedef struct CursorCtx {
    char pad_0000[0x272E];
    unsigned short field_272E;
    char pad_2730[0x38A0 - 0x2730];
    CursorMarks field_38A0;
} CursorCtx;

typedef struct CursorOwner {
    char pad_00[0x14C];
    CursorCtx *field_14C;
} CursorOwner;

extern int func_089FB91C(CursorOwner *self, short index);
extern void func_089FD2E8(CursorOwner *self, short a, short b);

void func_089FD380(CursorOwner *self)
{
    CursorCtx *ctx = self->field_14C;
    CursorMarks *marks = &ctx->field_38A0;
    int index = func_089FB91C(self, ctx->field_272E);
    func_089FD2E8(self, marks->field_26D70[index].field_00 + 1, marks->field_26D70[index].field_02);
}
