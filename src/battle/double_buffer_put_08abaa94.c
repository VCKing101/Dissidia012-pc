typedef struct DoubleBuffer {
    unsigned char active;
    char pad_01[0xB];
    int current;
    int slots[2];
    int shown[2];
} DoubleBuffer;

/* Write value into the back slot and flip. */
int func_08ABAA94(DoubleBuffer *self, int value)
{
    int back;
    if (self->active == 0) {
        return -1;
    }
    back = self->current == 0;
    self->slots[(unsigned char)back] = value;
    self->current = back;
    return 0;
}
