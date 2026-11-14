struct StepCtx {
    char pad_000[0xC];
    int field_0C;
    char pad_10[0x40C - 0x10];
    int field_40C;
    char pad_410[0x890 - 0x410];
    int field_890;
    char pad_894[0xA7C - 0x894];
    int field_A7C;
    unsigned int field_A80;

    static bool has_flag(unsigned int flags, unsigned int mask)
    {
        return (flags & mask) != 0;
    }
};

struct StepOwner {
    char pad_000[0x14C];
    StepCtx *field_14C;
};

extern "C" {

void func_088D3F00(void *a, void *b, short *table);
void func_088D44EC(void *a, void *b, unsigned short *table);
void func_08A4A3C4(StepOwner *self);
void func_08A4B5AC(StepOwner *self);
extern short D_08A4BE74;
extern unsigned short D_08A4BED4;

void func_08A4A1E4(StepOwner *self)
{
    func_088D3F00(&self->field_14C->field_0C, &self->field_14C->field_40C, &D_08A4BE74);
    func_088D44EC(&self->field_14C->field_40C, &self->field_14C->field_890, &D_08A4BED4);
    func_08A4A3C4(self);
    if (!StepCtx::has_flag(self->field_14C->field_A80, 2)) {
        return;
    }
    self->field_14C->field_A7C = 2;
    func_08A4B5AC(self);
}

}
