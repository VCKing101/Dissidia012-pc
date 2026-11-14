/* Clears the ten slots: no id, all flags and the counter zero. */
typedef struct ClearSlot {
    signed char field_00;
    unsigned char field_01[14];
    char pad_0F;
    short field_10;
} ClearSlot;

void func_08A7085C(ClearSlot *slots)
{
    int i;
    for (i = 0; i < 10; i++) {
        ClearSlot *slot = &slots[i];
        slot->field_00 = -1;
        slot->field_01[0] = 0;
        slot->field_01[1] = 0;
        slot->field_01[2] = 0;
        slot->field_01[3] = 0;
        slot->field_01[4] = 0;
        slot->field_01[5] = 0;
        slot->field_01[6] = 0;
        slot->field_01[7] = 0;
        slot->field_01[8] = 0;
        slot->field_01[9] = 0;
        slot->field_01[10] = 0;
        slot->field_01[11] = 0;
        slot->field_01[12] = 0;
        slot->field_01[13] = 0;
        slot->field_10 = 0;
    }
}
