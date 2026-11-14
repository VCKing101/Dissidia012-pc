typedef struct RingEntry {
    int field_00;
    int field_04;
} RingEntry;

typedef struct RingState {
    char pad_000[0x5B8];
    int field_5B8;
    char pad_5BC[0x734 - 0x5BC];
    RingEntry field_734[0x20];
} RingState;

typedef struct RingOwner {
    char pad_000[0x850];
    RingState *field_850;
} RingOwner;

RingEntry *func_08AE211C(RingOwner *self, int id)
{
    RingState *state = self->field_850;
    int count = state->field_5B8;
    int i;
    for (i = 0; i < count; i++) {
        if (id == state->field_734[i].field_00) {
            return &state->field_734[i];
        }
    }
    return 0;
}
