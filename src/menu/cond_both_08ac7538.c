typedef struct CondCtx {
    char pad_000[0xBF8];
    int field_BF8;
} CondCtx;

typedef struct CondOwner {
    char pad_000[0xC18];
    CondCtx *field_C18;
} CondOwner;

extern char func_088F1540(void);

int func_08AC7538(CondOwner *self)
{
    if (self->field_C18->field_BF8 <= 0) {
        if (func_088F1540() > 0) {
            return 1;
        }
    }
    return 0;
}
