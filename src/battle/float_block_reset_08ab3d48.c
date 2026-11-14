typedef struct FloatBlock {
    unsigned char active;
    float value;
    int field_08;
    int field_0C;
    char data[0x30];
} FloatBlock;

extern void func_08935828(void *dst, int value, int size);

void func_08AB3D48(FloatBlock *self)
{
    self->active = 0;
    self->value = 0.0f;
    self->field_08 = 0;
    self->field_0C = 0;
    func_08935828(self->data, 0, 0x30);
}
