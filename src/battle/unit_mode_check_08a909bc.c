typedef struct ModeUnit {
    char pad_000[0x134];
    int mode;
} ModeUnit;

typedef struct ModeTask {
    void *vtable;
    unsigned char bound;
    char pad_05[0x3];
    ModeUnit *unit;
} ModeTask;

/* True when the unit's mode is neither 0 nor 1. */
unsigned char func_08A909BC(ModeTask *self)
{
    int result = 0;
    int mode = self->unit->mode;

    if (mode != 1 && mode != 0) {
        result = 1;
    }
    return result;
}
