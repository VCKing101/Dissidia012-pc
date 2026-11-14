typedef struct CounterGate {
    unsigned char active;
    char pad_01[0x323];
    int limit;
    int count;
} CounterGate;

extern int func_08A65904(CounterGate *self);

int func_08A65928(CounterGate *self)
{
    if (self->active == 0) {
        return 0;
    }
    if (func_08A65904(self) == 1) {
        return 0;
    }
    return !(self->count < self->limit);
}
