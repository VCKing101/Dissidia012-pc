typedef struct CounterHolder {
    unsigned char active;
    char pad_01[0xB];
    int value;
} CounterHolder;

int func_08A97404(CounterHolder *self, int value)
{
    if (self->active == 0) {
        return -1;
    }
    self->value = value;
    return 0;
}
