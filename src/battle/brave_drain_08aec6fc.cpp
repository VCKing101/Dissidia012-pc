typedef struct DrainStatus {
    int pad_00;
    short percent;
} DrainStatus;

typedef struct BraveGauge {
    char pad_00[0x18];
    float value;

    static float clamp(float value, float min, float max)
    {
        if (value < min) {
            return min;
        }
        if (value > max) {
            return max;
        }
        return value;
    }
} BraveGauge;

typedef struct BraveUnit {
    char pad_000[0x52C];
    BraveGauge *gauge;
} BraveUnit;

typedef struct DrainTarget {
    void *unit;
} DrainTarget;

typedef struct DrainEvent {
    int pad_00;
    DrainTarget *target;
} DrainEvent;

typedef struct DrainOwner {
    char pad_00[0x8];
    BraveUnit *unit;
} DrainOwner;

extern "C" {

extern void *func_08835778(void *unit);
extern DrainStatus *func_088BF4CC(void *status, int id);
extern void func_0883F2A0(void *status, int id);

/* Drain the owner's gauge by status 0x9F's percentage of 10000, then clear the status. */
void func_08AEC6FC(DrainOwner *self, DrainEvent *event)
{
    void *status = func_08835778(event->target->unit);
    DrainStatus *drain = func_088BF4CC(status, 0x9F);
    if (drain != 0) {
        short *percent = &drain->percent;
        int amount = *percent * 10000 / 100;
        BraveGauge *gauge = self->unit->gauge;
        gauge->value = BraveGauge::clamp(gauge->value + (float)-amount, 0.0f, 10000.0f);
        func_0883F2A0(status, 0x9F);
    }
}

}
