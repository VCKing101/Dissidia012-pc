typedef struct CounterHolder {
    unsigned char active;
    char pad_01[0x17];
    int value;
} CounterHolder;

int func_08A974B0(CounterHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->value = 0;
    return 0;
}
