typedef struct ValueHolder {
    unsigned char active;
    char pad_01[0x97];
    int value;
} ValueHolder;

int func_08A87B38(ValueHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    return self->value;
}
