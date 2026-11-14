struct StepFlagsObj {
    char pad_00[0x20];
    unsigned short field_20;

    static bool has_flag(StepFlagsObj *obj, int mask)
    {
        return (obj->field_20 & mask) != 0;
    }
};

struct StepCtxB {
    char pad_0000[0x88];
    int field_88;
    char pad_8C[0x488 - 0x8C];
    int field_488;
    char pad_48C[0x8F4 - 0x48C];
    StepFlagsObj *field_8F4;
    char pad_8F8[0xA8C - 0x8F8];
    int field_A8C;
    char pad_A90[0x3238 - 0xA90];
    unsigned int field_3238;
    char pad_323C[0x33C8 - 0x323C];
    int field_33C8;
};

struct StepOwner {
    char pad_00[0x14C];
    StepCtxB *field_14C;
};

extern "C" {

void func_088D3F00(void *a, void *b, short *table);
void func_088D44EC(void *a, void *b, unsigned short *table);
void func_088D4B74(void *a, char flag);
extern short D_08A0A5AA;
extern unsigned short D_08A0A642;

void func_089F99CC(StepOwner *self)
{
    StepCtxB *ctx = self->field_14C;
    if ((ctx->field_3238 & 0x100) == 0) {
        return;
    }
    func_088D3F00(&ctx->field_88, &ctx->field_488, &D_08A0A5AA);
    func_088D44EC(&self->field_14C->field_488, &self->field_14C->field_A8C, &D_08A0A642);
    func_088D4B74(&self->field_14C->field_33C8, 0);
    ctx = self->field_14C;
    if (!StepFlagsObj::has_flag(ctx->field_8F4, 0x20)) {
        return;
    }
    ctx->field_3238 = ctx->field_3238 | 0x200;
}

}
