typedef struct FloatSlot3 {
    unsigned char active;
    int field_04;
    int field_08;
    float value;
} FloatSlot3;

int func_08ABA120(FloatSlot3 *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->field_08 = 1;
    self->value = 0.0f;
    return 0;
}
