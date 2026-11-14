typedef struct ClampRange {
    short field_00;
    short field_02;
} ClampRange;

typedef struct ClampState {
    char pad_00[0x68];
    ClampRange *field_68;
} ClampState;

typedef struct ClampOwner {
    char pad_000[0x14C];
    ClampState *field_14C;
} ClampOwner;

short func_089F0454(ClampOwner *self, short base, short delta)
{
    ClampRange *range = self->field_14C->field_68;
    short value = base + delta;
    if (value < 1) {
        value = 1;
    }
    if (value < range->field_00) {
        value = range->field_00;
    }
    if (range->field_02 < value) {
        value = range->field_02;
    }
    return value;
}
