/* Stores the chosen value and writes it into the current group's entry. */
typedef struct SetGroup {
    char pad_00[0xC5];
    unsigned char field_C5;
} SetGroup;

typedef struct SetGroupState {
    char pad_0000[0xC80];
    SetGroup field_C80[4];
    char pad_0F98[0x13E3 - 0xF98];
    signed char field_13E3;
    char pad_13E4[0x2C04 - 0x13E4];
    int field_2C04;
} SetGroupState;

typedef struct SetGroupOwner {
    char pad_000[0x140];
    SetGroupState *field_140;
} SetGroupOwner;

extern void func_08AF3ECC(SetGroupOwner *self);

void func_08AF44AC(SetGroupOwner *self, int value)
{
    SetGroupState *state;
    SetGroup *group;
    self->field_140->field_2C04 = value;
    state = self->field_140;
    group = &state->field_C80[state->field_13E3];
    group->field_C5 = state->field_2C04;
    func_08AF3ECC(self);
}
