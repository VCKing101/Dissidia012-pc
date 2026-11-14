typedef struct PartyMember {
    int words[0x5B7];
} PartyMember;

typedef struct PartyState {
    char pad_000[0x5A4];
    int field_5A4;
    char pad_5A8[0xA58 - 0x5A8];
    PartyMember field_A58[6];
} PartyState;

typedef struct PartyOwner {
    char pad_0000[0x7CFC];
    PartyState *field_7CFC;
} PartyOwner;

extern int func_08AB16A8(PartyMember *member);

int func_08AAB8F0(PartyOwner *self)
{
    PartyState *state = self->field_7CFC;
    int index = state->field_5A4;
    if (index < 0 || index >= 6) {
        return 0;
    }
    if (index == 5) {
        return 0;
    }
    if (func_08AB16A8(&state->field_A58[index]) >= 0) {
        return 1;
    }
    return 0;
}
