typedef struct SideStart {
    char pad_000[0x8];
    int state;
    char pad_00C[0x1];
    unsigned char ready;
    char pad_00E[0x2];
    float elapsed;
    int unit_id;
    char pad_018[0x5C];
    char field_74[0x7C];
    char field_F0[0x1C4];
    int field_2B4;
    int field_2B8;
} SideStart;

extern int func_089E99C0(int side, int unit_id);
extern void func_089F84E0(void *request);
extern int func_089F85B0(void *request, int unit_id, int side);
extern void func_08A020C8(void *sub);

int func_08A126A8(SideStart *self)
{
    if (self->ready == 1) {
        int side = func_089E99C0((self->field_2B4 + self->field_2B8) % 2, self->unit_id);
        func_089F84E0(self->field_F0);
        func_089F85B0(self->field_F0, self->unit_id, side);
        func_08A020C8(self->field_74);
        self->state = 5;
        self->elapsed = 0.0f;
    }
    return 0;
}
