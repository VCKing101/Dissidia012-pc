typedef struct SetupHandle {
    char pad_00[0xE];
    short field_0E;
} SetupHandle;

typedef struct SetupCtx {
    char pad_000[8];
    unsigned short field_08;
    char pad_0A[2];
    int field_0C;
    SetupHandle *field_10;
    char pad_14[0x490 - 0x14];
    char field_490[0x4C];
    char field_4DC[0x53C - 0x4DC];
    int field_53C;
    char pad_540[0x574 - 0x540];
    int field_574;
    int field_578;
    int field_57C;
} SetupCtx;

typedef struct SetupOwner {
    char pad_000[0x584];
    SetupCtx *field_584;
} SetupOwner;

extern void func_088D3B88(int a, void *b, unsigned short *table, int c, int d);
extern void func_088D3CF0(int a, void *b, unsigned short *table);
extern void func_088D3F7C(int a, void *b, unsigned short *table, int c);
extern void func_088D428C(void *a, void *b, unsigned short *table, char flag);
extern void func_088D46E4(void *a, int b, int c, int d, int e, int f);
extern void func_08AB2A30(SetupOwner *self);
extern unsigned short D_08AB2F58;
extern unsigned short D_08AB2F70;
extern unsigned short D_08AB2FA0;
extern unsigned short D_08AB2FD0;

void func_08AB2C5C(SetupOwner *self)
{
    SetupCtx *ctx;
    func_088D3F7C(self->field_584->field_574, self->field_584->field_490, &D_08AB2FA0, 0);
    ctx = self->field_584;
    func_088D3B88(ctx->field_0C, &ctx->field_10, &D_08AB2F58, ctx->field_08, 0x120);
    func_088D3CF0(self->field_584->field_0C, &self->field_584->field_10, &D_08AB2F70);
    self->field_584->field_10->field_0E = 2;
    func_088D428C(&self->field_584->field_10, self->field_584->field_490, &D_08AB2FD0, 1);
    func_088D46E4(&self->field_584->field_53C, self->field_584->field_57C, 0, 1, 0, -1);
    func_08AB2A30(self);
}
