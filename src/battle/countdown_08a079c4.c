typedef struct Countdown {
    char pad_000[0x4];
    int done;
    int field_08;
    int field_0C;
} Countdown;

extern void func_089F0E5C(int value);

int func_08A079C4(Countdown *self)
{
    if (self->done == 0) {
        if (self->field_08 > 0) {
            self->field_08--;
        } else {
            func_089F0E5C(self->field_0C);
            self->done = 1;
        }
    }
    return 0;
}
