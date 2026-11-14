typedef struct CounterHolder {
    unsigned char active;
    char pad_01[0x2B];
    int value;
} CounterHolder;

int func_08A97698(CounterHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->value++;
    return 0;
}
