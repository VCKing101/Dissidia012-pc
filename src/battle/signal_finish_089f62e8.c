typedef struct SignalFinish {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    char pad_0C[0x4];
    char field_10[0x20];
    unsigned char pending;
    char pad_31[0x3];
    float field_34;
    int field_38;
    int field_3C;
    float field_40;
} SignalFinish;

extern void func_089F7E40(void *sub);
extern void func_08889F70(int parity, int value);

int func_089F62E8(SignalFinish *self)
{
    if (!self->active) {
        return -1;
    }
    if (!self->pending) {
        return -2;
    }
    func_089F7E40(self->field_10);
    func_08889F70(self->field_08, self->field_04 + 3);
    self->field_38 = 0;
    self->field_3C = 0;
    self->field_40 = 0.0f;
    self->pending = 0;
    self->field_34 = 0.0f;
    return 0;
}
