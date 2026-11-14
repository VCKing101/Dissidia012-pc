typedef struct CounterHolder {
    unsigned char active;
    char pad_01[0x2B];
    int value;
} CounterHolder;

int func_08A97674(CounterHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->value = 0;
    return 0;
}
