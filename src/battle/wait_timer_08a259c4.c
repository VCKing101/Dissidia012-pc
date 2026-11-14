typedef struct WaitTimer259C4 {
    char pad_000[0x4];
    int state;
    float elapsed;
    char pad_00C[0x8];
    char field_14[0xAC];
    int field_C0;
} WaitTimer259C4;

extern void func_089F854C(void *sub);
extern void func_08889F28(short kind, int value);
extern void func_08889F70(int kind, int value);

int func_08A259C4(WaitTimer259C4 *self, float dt)
{
    self->elapsed += dt;
    if (!(self->elapsed < 18.0f)) {
        func_089F854C(self->field_14);
        func_08889F28(2, self->field_C0 + 1);
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

int func_08A25A34(WaitTimer259C4 *self, float dt)
{
    self->elapsed += dt;
    if (!(self->elapsed < 18.0f)) {
        func_08889F70(2, self->field_C0 + 1);
        self->state = 3;
        self->elapsed = 0.0f;
    }
    return 0;
}
