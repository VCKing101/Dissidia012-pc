typedef struct ModeCtx {
    char pad_0000[0x1B7C];
    unsigned int field_1B7C;
} ModeCtx;

typedef struct ModeOwner {
    char pad_000[0x46AC];
    ModeCtx *field_46AC;
} ModeOwner;

extern void func_08AF6E10(int target, int value);
extern unsigned int func_08AF96B0(void);
extern int D_08B01CF8;

void func_08ADB6A8(ModeOwner *self)
{
    int value = 0x15;
    if ((self->field_46AC->field_1B7C & 6) != 0) {
        value = func_08AF96B0() != 0 ? 0x17 : 0x16;
    }
    func_08AF6E10(D_08B01CF8, value);
}
