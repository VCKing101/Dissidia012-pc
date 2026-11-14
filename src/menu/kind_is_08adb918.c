typedef struct KindObj {
    char pad_00[0xC];
    short field_0C;
} KindObj;

typedef struct KindCtx {
    char pad_000[0x400];
    KindObj *field_400;
} KindCtx;

typedef struct KindOwner {
    char pad_000[0x46AC];
    KindCtx *field_46AC;
} KindOwner;

unsigned char func_08ADB918(KindOwner *self)
{
    if (self->field_46AC->field_400->field_0C == 1) {
        return 1;
    }
    return 0;
}
