typedef struct FloatSlot3 {
    unsigned char active;
    int field_04;
    int field_08;
    float value;
} FloatSlot3;

void func_08ABA098(FloatSlot3 *self)
{
    self->active = 1;
    self->field_04 = 0;
    self->field_08 = 0;
    self->value = 0.0f;
}
