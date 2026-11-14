typedef struct CountStats {
    char pad_00[0x14];
    short count;
} CountStats;

typedef struct CountUnit {
    char pad_000[0x52C];
    CountStats *stats;
} CountUnit;

typedef struct CountOwner {
    char pad_00[0x10];
    CountUnit *unit;

    static int clamp(int value, int min, int max)
    {
        if (value < min) {
            return min;
        }
        if (value > max) {
            return max;
        }
        return value;
    }
} CountOwner;

extern "C" {

extern int func_08AE0ADC(CountOwner *self, CountUnit *unit);
extern void func_08AE3710(CountOwner *self, int kind, int delta, int shown, int flags, float time);

/* Refresh the unit's count (0-9999) and report the change. */
void func_08AE0BA8(CountOwner *self)
{
    CountStats *stats = self->unit->stats;
    int value = func_08AE0ADC(self, self->unit);
    short old = stats->count;
    stats->count = CountOwner::clamp(value, 0, 9999);
    {
        int delta = stats->count - old;
        func_08AE3710(self, 6, delta, delta, 0, 0.0f);
    }
}

}
