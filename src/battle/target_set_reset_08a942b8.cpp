typedef struct TargetSet {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    float field_0C;
    float field_10;
    int field_14;
    int targets[3];
} TargetSet;

extern "C" void func_08A942B8(TargetSet *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = -1.0f;
    self->field_10 = 0.0f;
    self->field_14 = -1;
    for (int i = 0; i < 3; i++) {
        self->targets[i] = -1;
    }
}
