/* Whether the current slot's requirement is met by at least two members. */
typedef struct ReadySlot {
    char pad_00[0x84];
} ReadySlot;

typedef struct ReadyState {
    char pad_0000[0x29CC];
    ReadySlot field_29CC[9];
} ReadyState;

typedef struct ReadyOwner {
    char pad_000[0x14C];
    ReadyState *field_14C;
} ReadyOwner;

extern int func_089F2FF0(ReadyOwner *self);
extern int func_089FC298(ReadyOwner *self);
extern int func_089F4394(ReadyOwner *self, ReadySlot *slot);

int func_089F22A4(ReadyOwner *self)
{
    int result = 0;
    ReadyState *state = self->field_14C;
    ReadySlot *slot = &state->field_29CC[func_089F2FF0(self)];
    int count = func_089FC298(self);
    int need = func_089F4394(self, slot);
    if (count >= 2 && count >= need) {
        result = 1;
    }
    return result;
}
