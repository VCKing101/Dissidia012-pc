typedef struct FloatSlot {
    unsigned char active;
    float value;
} FloatSlot;

void func_08AB3AD0(FloatSlot *self)
{
    self->active = 0;
    self->value = 0.0f;
}
