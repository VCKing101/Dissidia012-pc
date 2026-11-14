struct RoundState {
    unsigned char active;
    char pad_01[0x3];
    int id;
    float time;
    int field_0C;
    int field_10;
    int counts[2][4];
    char pad_34[0xD8];
    int first;
    int second;
    int field_114;
    int field_118;
    unsigned char marks[2];
    char pad_11E[0x2];
    int values[2];
};

extern "C" {

/* Clears the round state. */
void func_08A0ECB0(RoundState *self)
{
    self->active = 0;
    self->id = -1;
    self->time = 0.0f;
    self->field_0C = 0;
    self->field_10 = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            self->counts[i][j] = 0;
        }
    }
    self->first = -1;
    self->second = -1;
    self->field_114 = 0;
    self->field_118 = 0;
    for (int i = 0; i < 2; i++) {
        *((unsigned char *)self + 0x11C + i) = 0; /* marks[i], addressed base first */
        self->values[i] = 0;
    }
}

}
