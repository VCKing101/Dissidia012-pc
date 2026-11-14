typedef struct Unit5C58 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    int field_14;
    char pad_018[0x130];
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
} Unit5C58;

void func_08A15C58(Unit5C58 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_148 = -1;
    self->field_14C = -1;
    self->field_150 = 0;
    self->field_154 = 0;
}
