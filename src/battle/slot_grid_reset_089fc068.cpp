typedef struct SlotGrid13 {
    unsigned char active;
    char pad_01[0x3];
    void *field_04;
    void *heads[4];
    void *cells[4][13];
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
} SlotGrid13;

extern "C" {

void func_089FC068(SlotGrid13 *self)
{
    self->active = 0;
    self->field_04 = 0;
    for (int i = 0; i < 4; i++) {
        self->heads[i] = 0;
        for (int j = 0; j < 13; j++) {
            self->cells[i][j] = 0;
        }
    }
    self->field_E8 = -1;
    self->field_EC = 0;
    self->field_F0 = 0;
    self->field_F4 = -1;
    self->field_F8 = -1;
    self->field_FC = 0;
    self->field_100 = 0;
    self->field_104 = 0;
}

}
