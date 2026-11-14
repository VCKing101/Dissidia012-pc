typedef struct UnitState {
    char pad_000[0xD40];
    char flags[0x4];
    int field_D44;
    char pad_D48[0x4];
    signed char field_D4C;
} UnitState;

typedef struct UnitHolder {
    char pad_00[0x4];
    UnitState *state;
} UnitHolder;

typedef struct FlagQuery {
    char pad_00[0x4];
    UnitHolder *holder;
} FlagQuery;

extern "C" unsigned int func_088119B0(void *flags);

struct UnitStateCheck {
    static bool loaded(UnitState *state) { return state->field_D44 != 0; }
    static bool valid(UnitState *state) { return state->field_D4C >= 0; }
};

extern "C" int func_08A82FB0(FlagQuery *self)
{
    UnitState *state = self->holder->state;

    if (state == 0) {
        return 0;
    }
    if (!UnitStateCheck::loaded(state) || !UnitStateCheck::valid(state)) {
        return 0;
    }
    return (func_088119B0(state->flags) & 4) != 0;
}
