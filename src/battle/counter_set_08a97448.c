typedef struct CounterHolder {
    unsigned char active;
    char pad_01[0xF];
    int value;
} CounterHolder;

int func_08A97448(CounterHolder *self, int value)
{
    if (self->active == 0) {
        return -1;
    }
    self->value = value;
    return 0;
}
