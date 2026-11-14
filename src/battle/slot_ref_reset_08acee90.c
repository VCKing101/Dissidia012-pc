typedef struct SlotRef {
    unsigned char active;
    int index;
    float value;
} SlotRef;

void func_08ACEE90(SlotRef *self)
{
    self->active = 0;
    self->index = -1;
    self->value = 0.0f;
}
