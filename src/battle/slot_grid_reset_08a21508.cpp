typedef struct SlotGridD {
    unsigned char active;
    char pad_01[0x3];
    void *field_04;
    void *heads[4];
    void *mids[4];
    void *cells[4][3];
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
} SlotGridD;

extern "C" {

void func_08A21508(SlotGridD *self)
{
    self->active = 0;
    self->field_04 = 0;
    for (int i = 0; i < 4; i++) {
        self->heads[i] = 0;
        self->mids[i] = 0;
        for (int j = 0; j < 3; j++) {
            self->cells[i][j] = 0;
        }
    }
    self->field_58 = -1;
    self->field_5C = 0;
    self->field_60 = 0;
    self->field_64 = -1;
    self->field_68 = -1;
    self->field_6C = 0;
    self->field_70 = 0;
}

}
