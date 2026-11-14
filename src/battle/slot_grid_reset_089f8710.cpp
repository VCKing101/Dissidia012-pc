typedef struct SlotGrid {
    unsigned char active;
    char pad_01[0x3];
    void *field_04;
    void *heads[4];
    void *cells[4][4];
    void *tails[4];
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
} SlotGrid;

extern "C" {

void func_089F8710(SlotGrid *self)
{
    self->active = 0;
    self->field_04 = 0;
    for (int i = 0; i < 4; i++) {
        self->heads[i] = 0;
        for (int j = 0; j < 4; j++) {
            self->cells[i][j] = 0;
        }
        self->tails[i] = 0;
    }
    self->field_68 = -1;
    self->field_6C = 0;
    self->field_70 = 0;
    self->field_74 = -1;
    self->field_78 = 0;
    self->field_7C = 0;
    self->field_80 = 0;
    self->field_84 = 0;
    self->field_88 = 0;
}

}
