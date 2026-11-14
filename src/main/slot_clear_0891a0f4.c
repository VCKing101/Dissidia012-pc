typedef struct SlotInfo {
    short first;
    short second;
    short count;
    short field_06;
    int field_08;
    int field_0C;
} SlotInfo;

void func_0891A0F4(SlotInfo *slot)
{
    slot->first = -1;
    slot->second = -1;
    slot->field_08 = 0;
    slot->field_0C = 0;
    slot->count = 0;
}
