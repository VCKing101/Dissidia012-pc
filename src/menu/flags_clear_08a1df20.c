typedef struct ClearCtx {
    char pad_0000[0x6964];
    int field_6964;
    char pad_6968[0x8E60 - 0x6968];
    unsigned int field_8E60;
} ClearCtx;

typedef struct ClearOwner {
    char pad_000[0x154];
    ClearCtx *field_154;
} ClearOwner;

typedef struct ClearGlobal {
    char pad_00[0x18];
    char field_18;
} ClearGlobal;

extern void func_088F7988(int handle);
extern unsigned char func_08A1D8E4(ClearOwner *self);
extern ClearGlobal D_09B6D840;

void func_08A1DF20(ClearOwner *self)
{
    self->field_154->field_8E60 = self->field_154->field_8E60 & ~0x30;
    D_09B6D840.field_18 = 0;
    if (func_08A1D8E4(self) != 0) {
        func_088F7988(self->field_154->field_6964);
    }
}
