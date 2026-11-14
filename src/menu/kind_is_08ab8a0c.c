typedef struct KindCtx {
    char pad_000[0x8B0];
    int field_8B0;
} KindCtx;

typedef struct KindOwner {
    char pad_000[0x138];
    KindCtx *field_138;
} KindOwner;

extern char func_088F45DC(int value);
extern int func_08AB9B84(KindOwner *self, int value);

unsigned char func_08AB8A0C(KindOwner *self)
{
    int value = func_08AB9B84(self, self->field_138->field_8B0);
    int kind = func_088F45DC(value);
    if (kind != 0x13) {
        return 0;
    }
    return 1;
}
