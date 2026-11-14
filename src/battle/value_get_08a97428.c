typedef struct ValueHolder {
    unsigned char active;
    char pad_01[0xB];
    int value;
} ValueHolder;

int func_08A97428(ValueHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    return self->value;
}
