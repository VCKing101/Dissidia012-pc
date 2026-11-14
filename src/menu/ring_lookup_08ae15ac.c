typedef struct RingState {
    char pad_000[0x5CC];
    int field_5CC;
    char pad_5D0[4];
    short field_5D4[0x28];
    short field_624[0x28];
} RingState;

typedef struct RingOwner {
    char pad_000[0x850];
    RingState *field_850;
} RingOwner;

short func_08AE15AC(RingOwner *self, int id)
{
    RingState *state = self->field_850;
    int count = state->field_5CC;
    int i;
    for (i = 0; i < count; i++) {
        if (id == state->field_5D4[i]) {
            return state->field_624[i];
        }
    }
    return 0;
}
