typedef struct StoreCtx {
    char pad_0000[0x7CC0];
    unsigned int field_7CC0;
    char pad_7CC4[0x7CD8 - 0x7CC4];
    int field_7CD8;
} StoreCtx;

typedef struct StoreOwner {
    char pad_0000[0x7CFC];
    StoreCtx *field_7CFC;
} StoreOwner;

extern int func_08A9D0E0(void);

void func_08AAAF00(StoreOwner *self)
{
    if ((self->field_7CFC->field_7CC0 & 2) == 0) {
        return;
    }
    self->field_7CFC->field_7CD8 = func_08A9D0E0();
}
