typedef struct FindUnit {
    char pad_000[0x8];
    unsigned short field_08;
    unsigned short field_0A;
    char pad_00C[0x240];
} FindUnit;

typedef struct FindState {
    char pad_0000[0x2BB8];
    FindUnit field_2BB8[0x1A];
    char pad_6770[0x6778 - (0x2BB8 + 0x1A * 0x24C)];
    int field_6778;
    short field_677C[1];
} FindState;

typedef struct FindOwner {
    char pad_000[0x154];
    FindState *field_154;
} FindOwner;

int func_08A1FAE4(FindOwner *self, int id)
{
    FindState *state = self->field_154;
    int i;
    int result = -1;
    FindUnit *units = state->field_2BB8;
    for (i = 0; i < state->field_6778; i++) {
        FindUnit *unit = &units[state->field_677C[i]];
        if (unit->field_0A == id && (unit->field_08 & 1) != 0) {
            result = i;
            break;
        }
    }
    return result;
}
