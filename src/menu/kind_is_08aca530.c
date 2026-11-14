typedef struct KindObj {
    char pad_00[0xC];
    short field_0C;
} KindObj;

typedef struct KindCtx {
    char pad_000[0x400];
    KindObj *field_400;
} KindCtx;

typedef struct KindOwner {
    char pad_000[0xDB30];
    KindCtx *field_DB30;
} KindOwner;

unsigned char func_08ACA530(KindOwner *self)
{
    if (self->field_DB30->field_400->field_0C == 0xB) {
        return 1;
    }
    return 0;
}
