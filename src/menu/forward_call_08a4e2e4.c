typedef struct ForwardCtx {
    char pad_000[0xC];
    int field_0C;
    char pad_10[0x6FE0 - 0x10];
    int field_6FE0;
} ForwardCtx;

typedef struct ForwardOwner {
    char pad_00[0x5C];
    ForwardCtx *field_5C;
} ForwardOwner;

extern void func_08A68494(int target, int p1, int p2, int p3, int p4, int ctx_value, int p5, int p6,
                          int p7, int p8);

void func_08A4E2E4(ForwardOwner *self, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8)
{
    func_08A68494(self->field_5C->field_6FE0, p1, p2, p3, p4, self->field_5C->field_0C, p5, p6, p7, p8);
}
