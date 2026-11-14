typedef struct StateReset50024 {
    int field_00;
    int field_04;
    int field_08;
    char pad_00C[0x24];
    float field_30;
    int field_34;
} StateReset50024;

extern void func_08A4FF90(StateReset50024 *self);

void func_08A50024(StateReset50024 *self)
{
    func_08A4FF90(self);
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_30 = 0.0f;
    self->field_34 = 0;
}
