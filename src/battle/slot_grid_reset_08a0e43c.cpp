typedef struct SlotGridB {
    unsigned char active;
    char pad_01[0x3];
    void *field_04;
    char pad_08[0x20];
    void *heads[4];
    void *cells[4][4];
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
} SlotGridB;

extern "C" {

void func_08A0E43C(SlotGridB *self)
{
    self->active = 0;
    self->field_04 = 0;
    for (int i = 0; i < 4; i++) {
        self->heads[i] = 0;
        for (int j = 0; j < 4; j++) {
            self->cells[i][j] = 0;
        }
    }
    self->field_78 = -1;
    self->field_7C = 0;
    self->field_80 = 0;
    self->field_84 = -1;
    self->field_88 = -1;
    self->field_8C = 0;
    self->field_90 = 0;
}

}
