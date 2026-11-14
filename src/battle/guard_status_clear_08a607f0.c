typedef struct StateFlags {
    char pad_00[0x34];
    unsigned int bits;
} StateFlags;

typedef struct FighterState {
    char pad_00[0x48];
    StateFlags flags;
} FighterState;

typedef struct GuardFighter {
    char pad_000[0x314];
    FighterState *state;
    char pad_318[0x6C];
    unsigned int status;
} GuardFighter;

extern void func_08837A5C(GuardFighter *fighter);

void func_08A607F0(void *self, GuardFighter *fighter)
{
    StateFlags *flags = &fighter->state->flags;

    if ((flags->bits & 4) == 0) {
        fighter->status &= ~0x1000;
    }
    func_08837A5C(fighter);
}
