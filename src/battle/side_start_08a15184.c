typedef struct SideStart15184 {
    char pad_000[0x8];
    int state;
    char pad_00C[0x1];
    unsigned char ready;
    char pad_00E[0x2];
    float elapsed;
    int unit_id;
    char pad_018[0x5C];
    char field_74[0x24];
    char field_98[0x2C];
    char field_C4[0xB4];
    int field_178;
    int field_17C;
} SideStart15184;

extern int func_089E99C0(int side, int unit_id);
extern void func_089F84E0(void *request);
extern int func_089F85B0(void *request, int unit_id, int side);
extern void func_08A020C8(void *sub);
extern void func_08A10DB0(void *timer);

int func_08A15184(SideStart15184 *self)
{
    if (self->ready == 1) {
        int side = func_089E99C0((self->field_178 + self->field_17C) % 2, self->unit_id);
        func_089F84E0(self->field_C4);
        func_089F85B0(self->field_C4, self->unit_id, side);
        func_08A020C8(self->field_74);
        func_08A10DB0(self->field_98);
        self->state = 5;
        self->elapsed = 0.0f;
    }
    return 0;
}
