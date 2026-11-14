typedef struct ValueHolder {
    unsigned char active;
    char pad_01[0xF];
    int value;
} ValueHolder;

int func_08A9746C(ValueHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    return self->value;
}
