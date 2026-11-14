typedef struct ChainCtx {
    char pad_0000[0x1B7C];
    unsigned int field_1B7C;
} ChainCtx;

typedef struct ChainOwner {
    char pad_000[0x46AC];
    ChainCtx *field_46AC;
} ChainOwner;

extern void func_08ADA924(ChainOwner *self);
extern void func_08ADA9C4(ChainOwner *self);
extern void func_08ADB65C(ChainOwner *self);
extern void func_08ADB6A8(ChainOwner *self);
extern void func_08ADB6F8(ChainOwner *self);
extern void func_08ADB830(ChainOwner *self);

void func_08ADC670(ChainOwner *self)
{
    self->field_46AC->field_1B7C = self->field_46AC->field_1B7C | 2;
    func_08ADB6A8(self);
    func_08ADB65C(self);
    func_08ADB6F8(self);
    func_08ADA924(self);
    func_08ADB830(self);
    func_08ADA9C4(self);
}
