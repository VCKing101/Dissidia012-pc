extern "C" {
extern void func_0887B1E4(void *clock);
extern void func_08935828(void *dst, int value, int size);

typedef struct Stopwatch {
    int field_00;
    int field_04;
    char clock[8];              /* 0x08 */
    char marks[8];              /* 0x10 */
    int field_18;
    unsigned char running;      /* 0x1C */

    static Stopwatch *construct(Stopwatch *self)
    {
        self->field_00 = 0;
        self->field_04 = 0;
        func_0887B1E4(self->clock);
        self->field_18 = 0;
        self->running = 0;
        func_08935828(self->marks, 0, sizeof(self->marks));
        return self;
    }
} Stopwatch;

Stopwatch *func_0891E300(Stopwatch *self)
{
    return Stopwatch::construct(self);
}
}
