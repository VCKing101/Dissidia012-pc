typedef struct CommandState {
    unsigned char field_00;
    unsigned char field_01;
    char pad_02[0x2];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    unsigned char field_14;
    char pad_15[0x3];
    int field_18;
    float field_1C;
    unsigned char field_20;
    char pad_21[0x3];
    float field_24;
    unsigned char field_28;
    char pad_29[0x1F7];
    void *field_220;
} CommandState;

void func_08A872E8(CommandState *self)
{
    self->field_00 = 0;
    self->field_01 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = -1;
    self->field_1C = 0.0f;
    self->field_20 = 0;
    self->field_24 = 0.0f;
    self->field_28 = 0;
    self->field_220 = 0;
}
