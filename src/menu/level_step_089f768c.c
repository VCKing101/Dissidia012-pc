typedef struct LevelState {
    char pad_0000[0x2731];
    signed char field_2731;
    char pad_2732[0x31DD - 0x2732];
    signed char field_31DD;
} LevelState;

typedef struct LevelOwner {
    char pad_000[0x14C];
    LevelState *field_14C;
} LevelOwner;

void func_089F768C(LevelOwner *self, int delta)
{
    self->field_14C->field_31DD = self->field_14C->field_2731;
    self->field_14C->field_2731 = self->field_14C->field_2731 + delta;
    if (self->field_14C->field_2731 < 1) {
        self->field_14C->field_2731 = 1;
    } else if (self->field_14C->field_2731 > 8) {
        self->field_14C->field_2731 = 8;
    }
}
