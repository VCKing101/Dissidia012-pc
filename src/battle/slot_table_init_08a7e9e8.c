typedef struct SlotEntry7E9E8 {
    int field_00;
    int field_04;
    int field_08[3];
} SlotEntry7E9E8;

typedef struct SlotTable7E9E8 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    int field_10;
    int field_14;
    int field_18;
    unsigned char field_1C;
    char pad_01D[0x3];
    int field_20;
    unsigned char field_24;
    char pad_025[0x3];
    SlotEntry7E9E8 entries[5];
    unsigned char field_8C;
    unsigned char field_8D;
    unsigned char field_8E;
    char pad_08F[0x1];
    int field_90;
    int field_94;
    int field_98;
    float field_9C;
} SlotTable7E9E8;

void func_08A7E9E8(SlotTable7E9E8 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = -1;
    self->field_14 = -1;
    self->field_18 = -1;
    self->field_1C = 0;
    self->field_20 = 0;
    self->field_24 = 0;
    {
        int i;
        for (i = 0; i < 5; i++) {
            SlotEntry7E9E8 *entry = &self->entries[i];
            entry->field_00 = -1;
            entry->field_04 = -1;
            {
                int j;
                for (j = 0; j < 3; j++) {
                    entry->field_08[j] = -1;
                }
            }
        }
    }
    self->field_8C = 0;
    self->field_8D = 0;
    self->field_8E = 0;
    self->field_90 = -1;
    self->field_94 = -1;
    self->field_98 = -1;
    self->field_9C = 0.0f;
}
