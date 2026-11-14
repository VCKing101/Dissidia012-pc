typedef struct ValueHolder {
    unsigned char active;
    char pad_01[0xA3];
    int value;
} ValueHolder;

int func_08A87AF8(ValueHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    return self->value;
}
