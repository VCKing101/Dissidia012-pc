typedef struct ScoreState {
    unsigned char active;
    char pad_01[0xF];
    int field_10;
    int field_14;
    int field_18;
    char pad_1C[0x2C];
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
} ScoreState;

/* Clears the scores; -1 inactive. */
int func_08AC119C(ScoreState *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_4C = 0;
    self->field_48 = 0;
    self->field_50 = 0;
    self->field_58 = 0;
    self->field_54 = 0;
    self->field_5C = 0;
    return 0;
}
