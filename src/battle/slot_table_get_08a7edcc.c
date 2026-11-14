typedef struct SlotEntry7EDCC {
    int field_00;
    int field_04;
    int field_08[3];
} SlotEntry7EDCC;

typedef struct SlotTable7EDCC {
    unsigned char active;
    char pad_001[0x27];
    SlotEntry7EDCC entries[5];
} SlotTable7EDCC;

extern int D_08A7FFE0;

int func_08A7EDCC(SlotTable7EDCC *self)
{
    SlotEntry7EDCC *entries;
    if (!self->active) {
        return -1;
    }
    entries = self->entries;
    return entries[D_08A7FFE0].field_00;
}
