typedef struct UnitFlags {
    char pad_00[0x2];
    unsigned char bits;
} UnitFlags;

typedef struct GuardUnit {
    char pad_000[0x5B0];
    UnitFlags flags;
} GuardUnit;

typedef struct GuardTask {
    GuardUnit *unit;
    int state;
    char pad_08[0x18];
    void *effect_20;
    void *effect_24;
} GuardTask;

extern void *D_08A62280;
extern void func_08A620B0(void *sound, int id);
extern void func_088DA060(void *effect);

void func_08ACA8B4(GuardTask *self)
{
    if (self->state == 5) {
        UnitFlags *flags = &self->unit->flags;
        flags->bits &= ~8;
        func_08A620B0(D_08A62280, 2);
    }
    if (self->effect_20 != 0) {
        func_088DA060(self->effect_20);
        self->effect_20 = 0;
    }
    if (self->effect_24 != 0) {
        func_088DA060(self->effect_24);
        self->effect_24 = 0;
    }
}
